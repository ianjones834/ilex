#include <climits>
#include <string>
#include<iostream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
using namespace std;

#define STATE_NUM 10
#define ACTION_NUM 5

int(*actionArr[STATE_NUM])() = {
    &action1, &action2, &action3, &action4, &action5, &action6, &action7,
};

struct yy_DFAState {
    int actionNum;
    int matchStartActionNum;
    int matchEndActionNum;
    int matchStartAndEndActionNum;

    bool acceptState;

    int curCharIndex;

    unordered_set<int> backTo;

    unordered_map<char, int> transitions;
};

yy_DFAState yy_stateArr[STATE_NUM];

"int yy_i;"

"pair<int, int> _yySimulate(string input, int startIndex = 0) {"
"    yy_i = startIndex;"

"    for (int start = startIndex; start < input.length();) {"
"        int stateNum = 0;"
"        int actionToRun = INT_MAX;"

"        for (; yy_i < input.length(); yy_i++) {"
"            char ch = input[yy_i];"

"            if (yy_stateArr[stateNum].transitions.contains(ch)) {"
"                stateNum = yy_stateArr[stateNum].transitions[ch];"
"                yy_stateArr[stateNum].curCharIndex = yy_i;"
"            }"
"            else {"
"                break;"
"            }"

"            if (yy_stateArr[stateNum].acceptState) {"
"                int matchEnd = yy_i;"

"                if (!yy_stateArr[stateNum].backTo.empty()) {"
"                    int backToIndex = INT_MIN;"

"                    for (auto backTo : yy_stateArr[stateNum].backTo) {"
"                        backToIndex = max(backToIndex, yy_stateArr[backTo].curCharIndex);"
"                    }"

"                    matchEnd = backToIndex;"
"                }"

"                if (yy_stateArr[stateNum].matchStartActionNum != INT_MAX && start == 0) {"
"                    actionToRun = min(actionToRun, yy_stateArr[stateNum].matchStartActionNum);"
"                }"

"                if (yy_stateArr[stateNum].matchEndActionNum != INT_MAX && yy_i == input.length() -1) {"
"                    actionToRun = min(actionToRun, yy_stateArr[stateNum].matchEndActionNum);"
"                }"

"                if (yy_stateArr[stateNum].matchStartAndEndActionNum != INT_MAX && start == 0 && yy_i == input.length() - 1) {"
"                    actionToRun = min(actionToRun, yy_stateArr[stateNum].matchStartAndEndActionNum);"
"                }"

"                actionToRun = min(actionToRun, yy_stateArr[stateNum].actionNum);"
"            }"
"        }"

"        if (actionToRun != INT_MAX) {"
"            int res = actionArr[actionToRun]();"

"            if (res > 0) {"
"                return {res, yy_i};"
"            }"

"            start = yy_i + 1;"
"        }"
"        else {"
"            return {-2, 0};"
"        }"
"    }"

"    return {-2, 0};"
"}"

"int yywrap();"

"int yylex() {"
"    string input;"
"    int start = 0;"

"    while (getline(cin, input)) {"
"        auto res = _yySimulate(input, start);"

"        if (res.first < 0) {"
"            return -1;"
"        }"
"    }"

"    return yywrap();"
"}"

