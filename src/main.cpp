#include <csignal>

#include "ilex.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <regex>

#include "Automata/DFA.h"
#include "Regex/regex.h"

unordered_map<string, string> namedPatterns;

bool keepReading = true;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        ofstream outFile;
        outFile.open("./ilex.yy.cpp");

        return ilex(cin, outFile);
    }

    ifstream inFile;
    inFile.open(argv[1]);

    if (!inFile.is_open()) {
        cout << "File could not be opened. Please try again." << endl;
        return -1;
    }

    ofstream outFile;
    outFile.open("./ilex.yy.cpp");

    if (!outFile.is_open()) {
        cout << "Failure to create output file." << endl;
        return -1;
    }

    return ilex(inFile, outFile);
}

int ilex(istream& in, ostream& out) {
    int (*funArr[3])(istream&, ostream&) = {&definitionsScanner, &rulesScanner, &subroutinesScanner};

    for (auto fun : funArr) {
        switch (fun(in, out)) {
            case 1: {
                cout << "Unexpected end of file" << endl;
                return -1;
            }
            case 2: {
                cout << "Invalid action format" << endl;
                return -1;
            }
            default: {}
        }
    }

    return 0;
}

int definitionsScanner(istream& in, ostream& out) {
    string cur;

    out << "#include<unordered_map>" << endl;
    out << "#include<unordered_set>" << endl;
    out << "#include<string>" << endl;
    out << "#include<iostream>" << endl;
    out << "#include<csignal>" << endl;

    while (getline(in, cur)) {
        if (cur == "%%") {
            return 0;
        }

        if (cur == "%{") {
            while (getline(in, cur)) {
                if (cur == "%}") {
                    break;
                }

                out << cur << endl;
            }
        }
        else {
            if (cur.empty()) {
                continue;
            }

            int i = 0;

            while (i < cur.length() && cur[i] == ' ') {
                i++;
            }

            int firstStart = i;

            while (i < cur.length() && cur[i] != ' ') {
                i++;
            }

            int firstEnd = i;

            while (i < cur.length() && cur[i] == ' ') {
                i++;
            }

            int secondStart = i;

            int j = cur.length() - 1;

            while (j >= 0 && cur[j] == ' ') {
                j--;
            }

            int secondEnd = j;

            string key = cur.substr(firstStart, firstEnd);
            string value = cur.substr(secondStart, secondEnd + 1);

            if (key.length() > 0 && value.length() > 0) {
                namedPatterns[key] = value;
            }
        }
    }

    return 1;
}



int rulesScanner(istream& in, ostream& out) {
    string cur = "";
    int curMachine = 0;
    int secondaryMachine = 0;

    stack<NFA*> nfaStack;
    unordered_set<int> matchStart;
    unordered_set<int> matchEnd;
    unordered_map<int, int> actionToSecondaryMachine;

    while (getline(in, cur)) {
        if (cur.empty()) {
            continue;
        }

        if (cur == "%%") {
            break;
        }

        string regex = "";

        int i = 0;
        bool regexNoAction = true;

        while (i < cur.length()) {
            switch (char ch = cur[i]) {
                case ' ': {
                    i++;
                    break;
                }
                case '|': {
                    regex += ch;

                    if (i + 1 == cur.length() || cur[i + 1] == ' ') {
                        if (!getline(in, cur)) {
                            return 2;
                        }

                        i = 0;
                    }
                    else {
                        i++;
                    }

                    break;
                }
                case '"': {
                    regex += cur[i++];

                    while (i < cur.length() && cur[i] != '"') {
                        regex += cur[i++];
                    }

                    regex += cur[i++];

                    break;
                }
                case '[': {
                    regex += cur[i++];

                    while (i < cur.length() && cur[i] != ']') {
                        regex += cur[i++];
                    }

                    i++;
                    break;
                }
                case '{': {
                    if (i > 0 && cur[i - 1] == ' ' && !regex.empty()) {
                        string action;
                        int leftParenthesisCount = 1;
                        i++;
                        while (true) {
                            if (cur[i] == '}') {
                                leftParenthesisCount--;
                                i++;
                            }

                            if (!leftParenthesisCount) {
                                break;
                            }

                            if (i < cur.length()) {
                                action += cur[i++];
                            }
                            else if (i == cur.length()) {
                                action += '\n';

                                if (!getline(in, cur)) {
                                    return 2;
                                }

                                i = 0;
                            }
                        }

                        if (!regex_search(action, std::regex("[;\n]?[\\s]*return.*;"))) {
                            action += "\nreturn -1;\n";
                        }

                        out << "int action" << curMachine << "() {\n" << action << "\n}" << endl;
                        regexNoAction = false;
                    }
                    else {
                        string inside = "";
                        int j = i + 1;
                        while (j < cur.length() && cur[j] != '}') {
                            if (cur[j] == ' ') {
                                return 2;
                            }

                            inside += cur[j++];
                        }

                        if (namedPatterns.contains(inside)) {
                            cur = cur.substr(0, i) + namedPatterns[inside] + cur.substr(j + 1, cur.length() - j - 1);
                            break;
                        }
                        else {
                            regex += "{" + inside + "}";
                            i = j + 1;
                            break;
                        }
                    }
                    break;
                }
                case '\\': {
                    regex += '\\';

                }
                default: {
                    if (i > 0 && cur[i - 1] == ' ') {
                        string action;

                        while (i < cur.length()) {
                            action += cur[i++];
                        }

                        if (!regex_search(action, std::regex("[;\n]?[\\s]*return.*;"))) {
                            action += "\nreturn -1;\n";
                        }

                        out << "int action" << curMachine << "() {\n" << action << "\n}" << endl;
                        regexNoAction = false;
                    }
                    else {
                        regex += ch;
                        i++;
                    }
                }
            }
        }

        if (!regex.empty()) {
            if (regex[0] == '^') {
                matchStart.insert(curMachine);
                regex = regex.substr(1, regex.length() - 1);
            }

            if (regex[regex.length() - 1] == '$') {
                matchEnd.insert(curMachine);
                regex = regex.substr(0, regex.length() - 1);
            }
        }

        if (regexNoAction && !regex.empty()) {
            out << "// " << regex << endl;
            out << "int action" << curMachine << "() {return -1;}" << endl;
        }

        NFA* nfa = regex_parse(regex);

        for (NFAState* state : nfa->states) {
            state->actionNum = curMachine;
        }

        nfaStack.push(nfa);
        curMachine++;
    }

    NFA* nfa = regex_parse(".");

    for (NFAState* state : nfa->states) {
        state->actionNum = curMachine;
    }
    out << "int action" << curMachine++ << "() {return -1;}" << endl;
    nfaStack.push(nfa);

    while (nfaStack.size() > 1) {
        NFA* nfa1 = nfaStack.top();
        nfaStack.pop();
        NFA* nfa2 = nfaStack.top();
        nfaStack.pop();

        nfaStack.push(nfa_union(nfa1, nfa2));
    }

    DFA* dfa;

    if (!nfaStack.empty()) {
        dfa = new DFA(nfaStack.top());
    }
    else {
        return 2;
    }

    for (NFAState* s : nfaStack.top() -> states) delete s;

    delete nfaStack.top();

    out << "struct DFAState {\n"
           "\tint matchStart\n"
           "\tint matchEnd\n"
           "\tint matchStartAndEnd\n"
           "\tint actionNum\n"
           "\t"

    out << "struct DFA {" << endl;
    out << "\t"

    out << "int yywrap();" << endl;

    out << "int yylex() {" << endl;


    if (!dfa_serialize(dfa, out)) {
        return 2;
    }
    out << string("    std::string input;\n") +
            "    while (getline(std::cin, input) && _yyKeepReading) {\n" +
            "        int i = 0;\n" +
            "        int curStart = 0;\n" +
            "        while (i < input.length()) {\n" +
            "            int cur = START;\n" +
            "            curStart = i;\n" +
            "            int(*mostRecentAction)() = nullptr;\n" +
            "            int mostRecentActionLength = -1;\n" +
            "            while (i < input.length()) {\n" +
            "                char ch = input[i];\n" +
            "                if (acceptedStates[cur] && (matchStart[cur] ? curStart == 0 : true) && (matchEnd[cur] ? i == input.length() : true)) {\n" +
            "                   mostRecentAction = stateToActionMap[cur];\n" +
            "                   mostRecentActionLength = i;\n" +
            "                }\n" +
            "                if (transitions[cur].contains(ch)) {\n" +
            "                    cur = transitions[cur].at(ch);\n" +
            "                }\n" +
            "                else {\n" +
            "                   break;\n" +
            "                }\n" +
            "                i++;\n" +
            "            }\n" +
            "            if (acceptedStates[cur] && (matchStart[cur] ? curStart == 0 : true) && (matchEnd[cur] ? i == input.length() : true)) {\n" +
            "               mostRecentAction = stateToActionMap[cur];\n" +
            "               mostRecentActionLength = i;\n" +
            "            }\n" +
            "            int res = -2;\n" +
            "            if (mostRecentAction != nullptr) {\n" +
            "                res = mostRecentAction();\n" +
            "            }\n" +
            "            if (res != -2) {\n" +
            "                i = mostRecentActionLength;\n" +
            "                continue;\n" +
            "            }\n" +
            "            else {\n" +
            "                break;\n" +
            "            }\n" +
            "            return -1;\n" +
            "        }\n" +
            "    }\n" +
            "   return yywrap();\n" +
            "}\n";

    delete dfa;
    return 0;
}

int subroutinesScanner(istream& in, ostream& out) {
    string cur;
    while (getline(in, cur)) {
        out << cur << endl;
    }

    return 0;
}