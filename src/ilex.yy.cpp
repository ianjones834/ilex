	#include<unordered_map>
#include<unordered_set>
#include<string>
#include<iostream>
int action0() {
    printf("a\n");
    return -1;

}
int action1() {
    printf("b\n");
    return -1;

}
int yylex() {
    const int START = 0;
    const int STATE_NUM = 4;
    const std::unordered_map<char,int> transitions[STATE_NUM] = {
        {{'a', 2}, {'b', 1}, },
        {{'a', 3}, {'b', 3}, },
        {{'a', 3}, {'b', 3}, },
        {{'a', 3}, {'b', 3}, },
    };
    const bool acceptedStates[STATE_NUM] = {0,1,1,0,};
    int(*stateToActionMap[STATE_NUM])() = { nullptr, &action1, &action0, nullptr, };
    std::string input;
    while (getline(std::cin, input)) {
        int i = 0;
        while (i < input.length()) {
            int cur = START;
            int(*mostRecentAction)() = nullptr;
            int mostRecentActionLength = -1;
            while (i < input.length()) {
                char ch = input[i];
                if (acceptedStates[cur]) {
                    mostRecentAction = stateToActionMap[cur];
                    mostRecentActionLength = i;
                }
                if (transitions[cur].contains(ch)) {
                    cur = transitions[cur].at(ch);
                }
                else {
                    break;
                }
                i++;
            }
            if (acceptedStates[cur]) {
                mostRecentAction = stateToActionMap[cur];
                mostRecentActionLength = i;
            }
            int res = -2;
            if (mostRecentAction != nullptr) {
                res = mostRecentAction();
            }
            if (res != -2) {
                i = mostRecentActionLength;
                continue;
            }
            else {
                break;
            }
            return -1;
        }
    }
    return 1;
}
int main() {
    yylex();
}
