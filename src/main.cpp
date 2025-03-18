#include <csignal>

#include "ilex.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <regex>
#include <unordered_map>

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

int ilex(istream& in, ofstream& out) {
    int (*funArr[3])(istream&, ofstream&) = {&definitionsScanner, &rulesScanner, &subroutinesScanner};

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

int definitionsScanner(istream& in, ofstream& out) {
    string cur;
    out << "#include<iostream>" << endl;
    out << "#include<string>" << endl;
    out << "#include<unordered_set>" << endl;
    out << "#include<unordered_map>" << endl;
    out << "#include<climits>" << endl;
    out << "using namespace std;" << endl;

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



int rulesScanner(istream& in, ofstream& out) {
    string cur = "";
    int curMachine = 0;

    stack<NFA*> nfaStack;

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

        if (regexNoAction && !regex.empty()) {
            out << "// " << regex << endl;
            out << "int action" << curMachine << "() {return -1;}" << endl;
        }

        NFA* nfa = regex_parse(regex, curMachine);

        for (int state = 0; state < nfa->stateNum; state++) {
            if (nfa->acceptStates[state]) {
                nfa->actionNum[state] = curMachine;
            }
        }

        nfaStack.push(nfa);
        curMachine++;
    }

    NFA* nfa = regex_parse(".", curMachine);

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

    delete nfaStack.top();

    out << "struct yy_DFAState {\n"
        "\tint actionNum;\n"
        "\tint matchStartActionNum;\n"
        "\tint matchEndActionNum;\n"
        "\tint matchStartAndEndActionNum;\n\n"
        "\tbool acceptState;\n\n"
        "\tint curCharIndex;\n\n"
        "\tunordered_set<int> backTo;\n\n"
        "\tunordered_map<char, int> transitions;\n"
        "};\n\n";

    out << *dfa << endl << endl;

    out << "#define ACTION_NUM " << curMachine << endl << endl;

    out << "int (*actionArr[ACTION_NUM])() = { ";

    for (int i = 0; i < curMachine; i++) {
        out << "&action" << i << ", ";
    }

    out << "};" << endl << endl;

    out << "int yy_i;\n"
        "int yy_match;\n"

        "pair<int, int> _yySimulate(string input, int startIndex = 0) {\n"
        "    yy_i = startIndex;\n"

        "    for (int start = startIndex; start < input.length();) {\n"
        "        int stateNum = 0;\n"
        "        int actionToRun = INT_MAX;\n"

        "        for (yy_i = start; yy_i < input.length(); yy_i++) {\n"
        "            char ch = input[yy_i];\n"

        "            if (yy_stateArr[stateNum].transitions.contains(ch)) {\n"
        "                stateNum = yy_stateArr[stateNum].transitions[ch];\n"
        "                yy_stateArr[stateNum].curCharIndex = yy_i;\n"
        "            }\n"
        "            else {\n"
        "                break;\n"
        "            }\n"

        "            if (yy_stateArr[stateNum].acceptState) {\n"
        "                yy_match = yy_i;\n"

        "                if (!yy_stateArr[stateNum].backTo.empty()) {\n"
        "                    int backToIndex = INT_MIN;\n"

        "                    for (auto backTo : yy_stateArr[stateNum].backTo) {\n"
        "                        backToIndex = max(backToIndex, yy_stateArr[backTo].curCharIndex);\n"
        "                    }\n"

        "                    yy_match = backToIndex;\n"
        "                }\n"

        "                if (yy_stateArr[stateNum].matchStartActionNum != INT_MAX && start == 0) {\n"
        "                    actionToRun = min(actionToRun, yy_stateArr[stateNum].matchStartActionNum);\n"
        "                }\n"

        "                if (yy_stateArr[stateNum].matchEndActionNum != INT_MAX && yy_i == input.length() -1) {\n"
        "                    actionToRun = min(actionToRun, yy_stateArr[stateNum].matchEndActionNum);\n"
        "                }\n"

        "                if (yy_stateArr[stateNum].matchStartAndEndActionNum != INT_MAX && start == 0 && yy_i == input.length() - 1) {\n"
        "                    actionToRun = min(actionToRun, yy_stateArr[stateNum].matchStartAndEndActionNum);\n"
        "                }\n"

        "                actionToRun = min(actionToRun, yy_stateArr[stateNum].actionNum);\n"
        "            }\n"
        "        }\n"

        "        if (actionToRun != INT_MAX) {\n"
        "            int res = actionArr[actionToRun]();\n"

        "            if (res > 0) {\n"
        "                return {res, yy_i};\n"
        "            }\n"

        "            start = yy_match + 1;\n"
        "        }\n"
        "        else {\n"
        "            return {0, 0};\n"
        "        }\n"
        "    }\n"

        "    return {0, 0};\n"
        "}\n"

        "int yywrap();\n"

        "int yylex() {\n"
        "    string input;\n"
        "    int start = 0;\n"

        "    while (getline(cin, input)) {\n"
        "        auto res = _yySimulate(input, start);\n"

        "        if (res.first < 0) {\n"
        "            return -1;\n"
        "        }\n"
        "    }\n"

        "    return yywrap();\n"
        "}\n";

    delete dfa;
    return 0;
}

int subroutinesScanner(istream& in, ofstream& out) {
    string cur;
    while (getline(in, cur)) {
        out << cur << endl;
    }

    return 0;
}