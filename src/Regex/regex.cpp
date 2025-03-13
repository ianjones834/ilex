//
// Created by ian on 1/12/25.
//

#include <set>
#include <stack>

#include "regex.h"

NFA* regex_parse(string str, bool isPrimaryNfa) {
    stack<NFA*> nfaStack;
    stack<char> opStack;
    bool matchStart = false, matchEnd = false, matchStartAndEnd = false;

    if (str[0] == '^') {
        matchStart = true;
        str = str.substr(1, str.length() - 1);
    }

    if (str[str.length() - 1] == '$') {
        str = str.substr(0, str.length() - 1);

        if (matchStart) {
            matchStartAndEnd = true;
            matchStart = false;
        }
        else {
            matchEnd = true;
        }
    }

    bool newSubExpression = true;

    for (int i = 0; i < str.length(); i++) {
        switch (char ch = str[i]) {
            case '(': {
                opStack.push(ch);
                newSubExpression = true;
                break;
            }
            case ')': {
                char op = opStack.top();

                while (op != '(') {
                    switch (op) {
                        case '|' : {
                            NFA* rhs = nfaStack.top();
                            nfaStack.pop();
                            NFA* lhs = nfaStack.top();
                            nfaStack.pop();

                            nfaStack.push(nfa_union(lhs, rhs));
                            break;
                        }
                        default:;
                    }

                    opStack.pop();
                    op = opStack.top();
                }

                opStack.pop();
                break;
            }
            case '|': {
                opStack.push(ch);
                newSubExpression = true;
                break;
            }
            case '*': {
                NFA* newNfa = nfa_zero_or_more(nfaStack.top());
                nfaStack.pop();
                nfaStack.push(newNfa);
                break;
            }
            case '+': {
                NFA* newNfa = nfa_one_or_more(nfaStack.top());
                nfaStack.pop();
                nfaStack.push(newNfa);
                break;
            }
            case '?': {
                NFA* newNfa = nfa_optional(nfaStack.top());
                nfaStack.pop();
                nfaStack.push(newNfa);
                break;
            }
            case '[': {
                if (str[i + 1] == '^') {
                    int j = i + 2;

                    unordered_set<char> charsToAvoid;

                    while (j < str.length() && str[j] != ']') {
                        if (j + 1 < str.length() && str[j + 1] == '-') {
                            if (j + 2 < str.length() && str[j] < str[j + 2]) {
                                for (char tmpCh = str[j]; tmpCh < str[j + 2]; tmpCh++) {
                                    charsToAvoid.insert(tmpCh);
                                }

                                j += 2;
                            }
                        }
                        else {
                            charsToAvoid.insert(str[j]);
                        }

                        j++;
                    }

                    NFA* newNfa = nfa_notInRange(charsToAvoid);
                    nfaStack.push(newNfa);
                    i = j;
                }
                else {
                    int j = i + 1;
                    unordered_set<char> charSet;
                    unordered_set<pair<char, char>*> rangeSet;

                    while (j < str.length() && str[j] != ']') {
                        if (j + 1 < str.length() && str[j + 1] == '-' && j + 2 < str.length() && str[j] < str[j + 2]) {
                            rangeSet.insert(new pair<char, char>{str[j], str[j + 2]});
                            j += 2;
                        }
                        else {
                            charSet.insert(str[j]);
                        }

                        j++;
                    }

                    if (!nfaStack.empty()) {
                        NFA* newNfa = nfa_concat(nfaStack.top(), nfa_range(charSet, rangeSet));
                        nfaStack.pop();
                        nfaStack.push(newNfa);
                    }
                    else {
                        nfaStack.push(nfa_range(charSet, rangeSet));
                    }

                    i = j;
                }

                break;
            }
            case '.': {
                NFA* newNfa = nfa_any();
                nfaStack.push(newNfa);
                break;
            }
            case '{': {
                int j = i + 1;

                string firstNum = "";

                while (j < str.length() && str[j] != ',') {
                    firstNum += str[j++];
                }

                j++;

                string secondNum = "";

                while (j < str.length() && str[j] != '}') {
                    secondNum += str[j++];
                }

                NFA* newNfa = nfa_repeat(nfaStack.top(), stoi(firstNum), stoi(secondNum));
                nfaStack.pop();
                nfaStack.push(newNfa);
                i = j + 1;
                break;
            }
            case '\\': {
                ch = str[++i];
            }
            default: {
                NFA* newNfa = nfa_new_single_char(ch);

                if (i + 1 < str.length()) {
                    switch (str[i + 1]) {
                        case '+': {
                            newNfa = nfa_one_or_more(newNfa);
                            i++;
                            break;
                        }
                        case '*': {
                            newNfa = nfa_zero_or_more(newNfa);
                            i++;
                            break;
                        }
                        case '?': {
                            newNfa = nfa_optional(newNfa);
                            i++;
                            break;
                        }
                        default:;
                    }
                }

                if (!newSubExpression) {
                    newNfa = nfa_concat(nfaStack.top(), newNfa);
                    nfaStack.pop();
                }
                else {
                    newSubExpression = false;
                }

                nfaStack.push(newNfa);
            }
        }
    }

    while (opStack.size() > 0) {
        switch (opStack.top()) {
            case '|': {
                NFA* rhs = nfaStack.top();
                nfaStack.pop();
                NFA *lhs = nfaStack.top();
                nfaStack.pop();
                nfaStack.push(nfa_union(lhs, rhs));
            }
        }

        opStack.pop();
    }

    while (nfaStack.size() > 1) {
        NFA* rhs = nfaStack.top();
        nfaStack.pop();
        NFA *lhs = nfaStack.top();
        nfaStack.pop();

        nfaStack.push(nfa_concat(lhs, rhs));
    }

    for (NFAState* state : nfaStack.top()->states) {
        if (state->acceptState) {
            state->matchStart = matchStart;
            state->matchEnd = matchEnd;
            state->matchStartAndEnd = matchStartAndEnd;
        }
    }

    return nfaStack.top();
}