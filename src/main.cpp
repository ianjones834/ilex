#include "ilex.h"
#include<iostream>
#include<fstream>
#include <stack>
#include<regex>

#include "regex.h"

unordered_map<string, string> namedPatterns;

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

    out << "\t#include<unordered_map>" << endl;
    out << "\t#include<unordered_set>" << endl;
    out << "\t#include<string>" << endl;
    out << "\t#include<iostream>" << endl;

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
    string cur;
    int curMachine = 0;
    stack<NFA*> nfaStack;

    while (getline(in, cur)) {
        if (cur.empty()) {
            continue;
        }

        if (cur == "%%") {
            break;;
        }

        string regex;

        int i = 0;

        while (i < cur.length()) {
            char ch = cur[i];
            bool inQuotes = false;

            switch (ch) {
                case ' ': {
                    if (inQuotes) {
                        regex += ' ';
                    }
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
                    inQuotes = !inQuotes;
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
                    }
                    else {
                        while (i < cur.length() && cur[i] != '}') {
                            if (cur[i] == ' ') {
                                return 2;
                            }

                            regex += cur[i++];
                        }
                    }
                    break;
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
                    }
                    else {
                        regex += ch;
                        i++;
                    }
                }
            }
        }

        NFA* nfa = regex_parse(regex);

        for (State* state : nfa->states) {
            state->nfaNum = curMachine;
        }

        nfaStack.push(nfa);
        curMachine++;
    }

    while (nfaStack.size() > 1) {
        NFA* nfa1 = nfaStack.top();
        nfaStack.pop();
        NFA* nfa2 = nfaStack.top();
        nfaStack.pop();

        nfaStack.push(nfa_union(nfa1, nfa2));
    }

    DFA* dfa = convert(nfaStack.top());

    out << "int yylex() {" << endl;



    if (!dfa_serialize(dfa, out)) {
        return 2;
    }
    out << string("    std::string input;\n") +
            "    while (getline(std::cin, input)) {\n" +
            "        int i = 0;\n" +
            "        while (i < input.length()) {\n" +
            "            int cur = START;\n" +
            "            int(*mostRecentAction)() = nullptr;\n" +
            "            int mostRecentActionLength = -1;\n" +
            "            while (i < input.length()) {\n" +
            "                char ch = input[i];\n" +
            "                if (acceptedStates[cur]) {\n" +
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
            "            if (acceptedStates[cur]) {\n" +
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
            "   return 1;\n" +
            "}\n";

    return 0;
}

int subroutinesScanner(istream& in, ostream& out) {
    string cur;
    while (getline(in, cur)) {
        out << cur << endl;
    }

    return 0;
}