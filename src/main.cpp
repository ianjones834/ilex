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
unordered_map<string, int> dfaNameToNum = {{"INITIAL", 0}};
unordered_map<int, string> dfaNumToName = {{0, "INITIAL"}};

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
    out << "#include<climits>" << endl;
    out << "#include<cstring>" << endl;
    out << "using namespace std;" << endl;

    out << "#define yywrap() 1\n";
    out << "#define YY_BUF_SIZE 1024\n";
    out << "FILE* yyin = stdin;" << endl;
    out << "FILE* yyout = stdout;" << endl;
    out << "#define output(c) fputs (c, yyout)\n";
    out << "int yy_dfaNum = 0;\n";
    out << "#define BEGIN yy_dfaNum = " << endl;
    out << "int yy_match;\n";
    out << "void yyless(int i) {\n\tyy_match -= i;\n}\n\n";
    out << "void yymore(int i) {\n\tyy_match += i;\n}\n\n";
    out << "int yyleng = 0;\n";
    out << "string yytext = \"\";\n";
    out << "#define ECHO output(yytext.c_str())" << endl;
    out << "char yybuf[YY_BUF_SIZE];\n";
    out << "#define input() fgetc(yyin)" << endl;
    out << "#define unput(c) ungetc((c), yyin)" << endl;

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

            if (i + 1 < cur.length() && cur[i] == '%' && cur[i + 1] == 's') {
                i += 2;

                while (i < cur.length() && cur[i] == ' ') {
                    i++;
                }

                int j = cur.length() - 1;

                while (j >= i && cur[j] == ' ') {
                    j--;
                }

                string name = cur.substr(i, j - i + 1);

                dfaNameToNum[name] = dfaNameToNum.size();
                dfaNumToName[dfaNumToName.size()] = name;

                continue;
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
    vector<string> actions;

    int dfaNum = 0;

    stack<NFA*> nfaStack;

    while (getline(in, cur)) {
        if (cur.empty()) {
            continue;
        }

        if (cur == "%%") {
            break;
        }

        string regex = "";
        dfaNum = 0;

        int i = 0;
        bool regexNoAction = true;

        while (i < cur.length()) {
            switch (char ch = cur[i]) {
                case ' ': {
                    i++;
                    break;
                }
                case '\t': {
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

                    regex += cur[i++];
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

                        actions.push_back("int action" + to_string(curMachine) + "() {\n" + action + "\n}\n");

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
                case '<': {
                    i++;
                    string dfaName;

                    while (cur[i] != '>') {
                        dfaName += cur[i++];
                    }

                    dfaNum = dfaNameToNum[dfaName];
                    i++;
                    break;
                }
                default: {
                    if (i > 0 && cur[i - 1] == ' ' && !regex.empty()) {
                        string action;

                        while (i < cur.length()) {
                            action += cur[i++];
                        }

                        if (!regex_search(action, std::regex(";?[\\s]*return.*;"))) {
                            action += "\nreturn -1;\n";
                        }

                        actions.push_back("int action" + to_string(curMachine) + "() {\n" + action + "\n}\n");

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
        nfa->dfaNum = dfaNum;

        for (int state = 0; state < nfa->stateNum; state++) {
            if (nfa->acceptStates[state]) {
                nfa->actionNum[state] = curMachine;
            }
        }

        nfaStack.push(nfa);
        curMachine++;
    }

    NFA* nfaAnyCharacter = regex_parse(".", curMachine);
    nfaAnyCharacter->dfaNum = 0;

    actions.push_back("int action" + to_string(curMachine++) + "() {return -1;}\n");

    nfaStack.push(nfaAnyCharacter);

    vector<vector<NFA*>> dfaNfaMatrix = vector<vector<NFA*>>(dfaNameToNum.size());

    if (nfaStack.empty()) {
        return 2;
    }

    while (!nfaStack.empty()) {
        dfaNfaMatrix[nfaStack.top()->dfaNum].push_back(nfaStack.top());
        nfaStack.pop();
    }

    vector<DFA*> dfaArr;

    for (int i = 0; i < dfaNfaMatrix.size(); i++) {
        NFA* nfa = nfa_nUnion(dfaNfaMatrix[i], true);
        dfaArr.push_back(new DFA(nfa));
        delete nfa;
    }

    DFA* dfa = dfa_nUnion(dfaArr, dfaNumToName);

    out << *dfa << endl;

    for (string action : actions) {
        out << action << endl;
    }

    out << "#define ACTION_NUM " << curMachine << endl << endl;

    out << "int (*actionArr[ACTION_NUM])() = { ";

    for (int i = 0; i < curMachine; i++) {
        out << "&action" << i << ", ";
    }

    out << "};" << endl << endl;

    out << "int yy_i;" << endl;

        out << "int yylex() {\n"
"    yylex_start:\n"
"    if (feof(yyin)) return 1;\n"
"    int start = yy_match;\n"
"    int stateNum = yy_dfaNum;\n"
"    int actionToRun = INT_MAX;\n"
"    yy_i = start;\n"
"    yytext = \"\";\n"
"    char ch;"
"    while ((ch = input()) != EOF) {\n"
"        if (ch == 0) break;\n"
"        yytext += ch;\n"
"        if (transitions[stateNum][ch] != -1) {\n"
"            stateNum = transitions[stateNum][ch];\n"
"            curCharIndex[stateNum] = yy_i;\n"
"        }\n"
"        else {\n"
"            break;\n"
"        }\n"
"        if (acceptStates[stateNum]) {\n"
"            yy_match = yy_i;\n"
"            yyleng = yy_i - start + 1;\n"

"            if (hasBackTo[stateNum]) {\n"
"                int backToIndex = INT_MIN;\n"
"                for (int j = 0; j < STATE_NUM; j++) {\n"
"                    if (backTo[j]) {\n"
"                        backToIndex = max(backToIndex, curCharIndex[j]);\n"
"                    }\n"
"                }\n"
"                yyleng = backToIndex - start + 1;\n"
"                yy_match = backToIndex;\n"
"            }\n"
"            if (matchStartActionNum[stateNum] != INT_MAX && start == 0) {\n"
"                actionToRun = min(actionToRun, matchStartActionNum[stateNum]);\n"
"            }\n"
"            if (matchEndActionNum[stateNum] != INT_MAX) {\n"
"                actionToRun = min(actionToRun, matchEndActionNum[stateNum]);\n"
"            }\n"
"            if (matchStartAndEndActionNum[stateNum] != INT_MAX && start == 0) {\n"
"                actionToRun = min(actionToRun, matchStartAndEndActionNum[stateNum]);\n"
"            }\n"
"            actionToRun = min(actionToRun, actionNum[stateNum]);\n"
"        }\n"

"        if (endState[stateNum] && actionToRun != INT_MAX) {\n"
"            while (yytext.length() > yyleng) {\n"
"                ungetc(yytext[yytext.length() - 1], yyin);\n"
"                yytext.erase(yytext.length() - 1, 1);\n"
"            }\n"

"            int res = actionArr[actionToRun]();\n"
"            yy_match++;\n"
"            goto yylex_start;\n"
"        }\n"
"        yy_i++;\n"
"    }\n"

"    if (actionToRun != INT_MAX) {\n"
"        while (yytext.length() > yyleng) {\n"
"            ungetc(yytext[yytext.length() - 1], yyin);\n"
"            yytext.erase(yytext.length() - 1, 1);\n"
"        }\n"
"        yy_match++;\n"

"        int res = actionArr[actionToRun]();\n"
"        char nextChar = input();\n"
"        if (nextChar == 0 || nextChar == EOF) return yywrap();\n"
"        ungetc(nextChar, yyin);\n"
"        goto yylex_start;\n"
"    }\n"
"    if (yywrap() == 0) { goto yylex_start;}\n"
"    return 0;\n"
"}\n" << endl;

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