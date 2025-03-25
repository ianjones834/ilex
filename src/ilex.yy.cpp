#include<iostream>
#include<string>
#include<climits>
using namespace std;
#define YY_BUF_SIZE 1024
FILE* yyin = stdin;
FILE* yyout = stdout;
#define output(c) fputs (c, yyout)
int yy_dfaNum = 0;
#define BEGIN yy_dfaNum = 
int yy_match;
void yyless(int i) {
	yy_match -= i;
}

void yymore(int i) {
	yy_match += i;
}

int yyleng = 0;
string yytext = "";
#define ECHO output(yytext.c_str())
char yybuf[YY_BUF_SIZE];
#define input() fgets(yybuf, YY_BUF_SIZE, yyin)
    unsigned charCount = 0, wordCount = 0, lineCount = 0;
int action0() {
wordCount++; charCount += yyleng;
return -1;

}
int action1() {
 charCount++; lineCount++;
return -1;

}
int action2() {
charCount++;
return -1;

}
int action3() {return -1;}
#define INITIAL 0
#define STATE_NUM 6

bool acceptStates[STATE_NUM] = { false, true, true, true, false, true };
int actionNum[STATE_NUM] = { 2147483647, 0, 2, 1, 2147483647, 0 };
int matchStartActionNum[STATE_NUM] = { 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647 };
int matchEndActionNum[STATE_NUM] = { 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647 };
int matchStartAndEndActionNum[STATE_NUM] = { 2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647 };

int curCharIndex[STATE_NUM] = { 0, 0, 0, 0, 0, 0 };

bool hasBackTo[STATE_NUM] = {false, false, false, false, false, false };

bool backTo[STATE_NUM][STATE_NUM] = {
{},
{},
{},
{},
{},
{},

};

int transitions[STATE_NUM][128] = {
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 0, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 },
	{ 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 0, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 },

};


#define ACTION_NUM 4

int (*actionArr[ACTION_NUM])() = { &action0, &action1, &action2, &action3, };

int yy_i;
pair<int, int> yySimulate(string input) {
    int start = yy_match;
    for (int start = yy_match; start < input.size();) {
        int stateNum = yy_dfaNum;
        int actionToRun = INT_MAX;
        yy_i = start;        for (yy_i = start; yy_i < input.size(); yy_i++) {
            char ch = input[yy_i];
            if (transitions[stateNum][ch] != -1) {
                stateNum = transitions[stateNum][ch];
                curCharIndex[stateNum] = yy_i;
            }
            else {
                break;
            }
            if (acceptStates[stateNum]) {
                yy_match = yy_i;
                yyleng = yy_i - start + 1;
                 yytext = input.substr(start, yyleng);
                if (hasBackTo[stateNum]) {
                    int backToIndex = INT_MIN;
                    for (int j = 0; j < STATE_NUM; j++) {
                        if (backTo[j]) {
                            backToIndex = max(backToIndex, curCharIndex[j]);
                        }
                    }
                    yyleng = backToIndex - start + 1;
                    yytext = input.substr(start, yyleng);
                    yy_match = backToIndex;
                }
                if (matchStartActionNum[stateNum] != INT_MAX && start == 0) {
                    actionToRun = min(actionToRun, matchStartActionNum[stateNum]);
                }
                if (matchEndActionNum[stateNum] != INT_MAX && yy_i == input.length() -1) {
                    actionToRun = min(actionToRun, matchEndActionNum[stateNum]);
                }
                if (matchStartAndEndActionNum[stateNum] != INT_MAX && start == 0 && yy_i == input.length() - 1) {
                    actionToRun = min(actionToRun, matchStartAndEndActionNum[stateNum]);
                }
                actionToRun = min(actionToRun, actionNum[stateNum]);
            }
        }
        if (actionToRun != INT_MAX) {
            int res = actionArr[actionToRun]();
            if (res > 0) {
                return {res, yy_i};
            }
            start = yy_match + 1;
        }
        else {
            return {0, 0};
        }
    }
    return {0, 0};
}
int yywrap();
int yylex() {
    while (input() != nullptr) {
    yy_match = 0;
        fill(curCharIndex, curCharIndex + STATE_NUM, -1);
        auto res = yySimulate(yybuf);
        if (res.first < 0) {
            return -1;
        }
    }
    return yywrap();
}

int yywrap() {
    return 0;
}

int main() {
    yylex();
    printf("%d %d %d\n", lineCount, wordCount ,charCount);
}
