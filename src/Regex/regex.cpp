//
// Created by ian on 1/12/25.
//

#include <set>
#include <stack>

#include "regex.h"

#include <climits>

NFA* regex_parse(string str, bool isPrimaryNfa) {
    stack<NFA*> nfaStack;
    stack<char> opStack;
    bool matchStart = false, matchEnd = false, matchStartAndEnd = false;
    bool nfaBackTo = false;
    bool concat = false;
    NFA* secondaryNfa;

    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '\\' && i + 1 < str.length()) {
            char newChar = 0;

            switch (str[i + 1]) {
                case 'n' : {
                    newChar = '\n';
                    break;
                }
                case 't' : {
                    newChar = '\t';
                    break;
                }
            }

            if (newChar != 0) {
                str = str.substr(0, i) + newChar + str.substr(i + 2, str.length() - i - 2);
            }
        }
    }

    for (int i = 0; i < str.length(); i++) {
        switch (char ch = str[i]) {
            case '"': {
                i++;

                while (i < str.length() && str[i] != '"') {
                    if (concat) {
                        opStack.push('*');
                    }
                    else {
                        concat = true;
                    }

                    nfaStack.push(nfa_new_single_char(str[i]));

                    i++;
                }

                break;
            }
            case '^': {
                if (isPrimaryNfa && i == 0) {
                    matchStart = true;
                }

                break;
            }
            case '$': {
                if (i == str.length() - 1) {
                    if (matchStart) {
                        matchStart = false;
                        matchStartAndEnd = true;
                    }
                    else {
                        matchEnd = true;
                    }

                    NFA* nfa = nfa_optional(nfa_new_single_char('\n'));

                    NFA* top = nfaStack.top();
                    nfaStack.pop();

                    nfaStack.push(nfa_concat(top, nfa));

                    break;
                }
            }
            case '(': {
                if (concat) {
                    opStack.push('*');
                    concat = false;
                }

                opStack.push(ch);


                break;
            }
            case ')': {
                char op = opStack.top();

                while (op != '(') {
                    switch (op) {
                        case '*': {
                            opStack.pop();
                            NFA* rhs = nfaStack.top();
                            nfaStack.pop();
                            NFA* lhs = nfaStack.top();
                            nfaStack.pop();
                            nfaStack.push(nfa_concat(lhs, rhs));
                            break;
                        }
                        case '|' : {
                            opStack.pop();
                            NFA* rhs = nfaStack.top();
                            nfaStack.pop();

                            while (!opStack.empty() && opStack.top() == '*') {
                                NFA* rhs2 = nfaStack.top();
                                nfaStack.pop();
                                NFA* lhs = nfaStack.top();
                                nfaStack.pop();
                                nfaStack.push(nfa_concat(lhs, rhs2));
                                opStack.pop();
                            }

                            NFA* lhs = nfaStack.top();
                            nfaStack.pop();

                            nfaStack.push(nfa_union(lhs, rhs));
                            break;
                        }
                        default:;
                    }

                    op = opStack.top();
                }

                opStack.pop();
                break;
            }
            case '|': {
                opStack.push(ch);
                concat = false;
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
                                for (char tmpCh = str[j]; tmpCh <= str[j + 2]; tmpCh++) {
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

                    if (concat) {
                        opStack.push('*');
                    }
                    else {
                        concat = true;
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

                    if (concat) {
                        opStack.push('*');
                    }
                    else {
                        concat = true;
                    }

                    nfaStack.push(nfa_range(charSet, rangeSet));

                    i = j;
                }

                break;
            }
            case '.': {
                if (concat) {
                    opStack.push('*');
                }
                else {
                    concat = true;
                }


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
            case '/': {
                if (isPrimaryNfa && !nfaBackTo) {
                    secondaryNfa = regex_parse(str.substr(i + 1, str.length() - i - 1), false);

                    nfaBackTo = true;

                    i = str.length();
                }
                else {
                    NFA* nfa = nfaStack.top();
                    nfaStack.pop();
                    nfaStack.push(nfa_concat(nfa, nfa_new_single_char('/')));
                }
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

                if (concat) {
                    opStack.push('*');
                }
                else {
                    concat = true;
                }

                nfaStack.push(newNfa);
            }
        }
    }

    while (opStack.size() > 0) {
        switch (opStack.top()) {
            case '*': {
                NFA* rhs = nfaStack.top();
                nfaStack.pop();
                NFA* lhs = nfaStack.top();
                nfaStack.pop();
                nfaStack.push(nfa_concat(lhs, rhs));
                opStack.pop();
                break;
            }
            case '|' : {
                opStack.pop();
                NFA* rhs = nfaStack.top();
                nfaStack.pop();

                while (!opStack.empty() && opStack.top() == '*') {
                    opStack.pop();
                    NFA* rhs2 = nfaStack.top();
                    nfaStack.pop();
                    NFA* lhs = nfaStack.top();
                    nfaStack.pop();
                    nfaStack.push(nfa_concat(lhs, rhs2));
                }

                NFA* lhs = nfaStack.top();
                nfaStack.pop();

                nfaStack.push(nfa_union(lhs, rhs));
                break;
            }
            default:;
        }
    }

    NFA* nfa = nfaStack.top();
    nfaStack.pop();

    if (nfaBackTo) {
        nfa = nfa_concat(nfa, secondaryNfa, true);
    }

    for (int i = 0; i < nfa->stateNum; i++) {
        if (nfa->acceptStates[i]) {
            nfa->matchStart[i] = matchStart;
            nfa->matchEnd[i] = matchEnd;
            nfa->matchStartAndEnd[i] = matchStartAndEnd;
            nfa->notMatchStartAndNotMatchEnd[i] = !matchStart && !matchEnd && !matchStartAndEnd;
        }
    }

    return nfa;
}

