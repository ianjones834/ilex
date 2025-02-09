//
// Created by ian on 1/12/25.
//

#include "regex.h"
#include <stack>



NFA* regex_parse(string str) {
    stack<NFA*> nfaStack;
    stack<char> opStack;

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
                int j = i + 1;
                stack<NFA*> rangeStack;

                while (str[j] != ']') {
                    if (j + 1 < str.length() && str[j + 1] == '-') {
                        if (j +2 < str.length() && str[j] < str[j + 2]) {
                            rangeStack.push(nfa_range(str[j], str[j + 2]));
                            j += 2;
                        }
                    }
                    else {
                        rangeStack.push(nfa_new_single_char(str[j]));
                    }

                    j++;
                }

                while (rangeStack.size() > 1) {
                    NFA* lhs = rangeStack.top();
                    rangeStack.pop();
                    NFA* rhs = rangeStack.top();
                    rangeStack.pop();

                    rangeStack.push(nfa_union(lhs, rhs));
                }

                if (!nfaStack.empty()) {
                    NFA* newNfa = nfa_concat(nfaStack.top(), rangeStack.top());
                    nfaStack.pop();
                    nfaStack.push(newNfa);
                }
                else {
                    nfaStack.push(rangeStack.top());
                }

                i = j;
                break;
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

    return nfaStack.top();
}