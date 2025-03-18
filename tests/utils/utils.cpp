//
// Created by ian on 1/12/25.
//

#include "utils.h"

#include <charconv>
#include <limits.h>
#include <queue>
#include <unordered_map>

#include "../../src/Automata/NFA.h"

ostream& operator<<(ostream& os, const NFA& nfa) {
    // for (int i = 0; i < nfa.stateNum; i++) {
    //     os << "State";
    //
    //     if (nfa.acceptStates[i]) {
    //         os << " (Accepting State)";
    //     }
    //
    //     os << ":" << i;
    //
    //     for (int j = 0; j < 128; j++) {
    //         os << "\t" << to_string(j) << " -> ";
    //
    //         for (int k : nfa.transitions[i][j]) {
    //             os << to_string(k) + " ";
    //         }
    //
    //         os << endl;
    //     }
    // }

    return os;
}

ostream& operator<<(ostream& os, const unordered_set<string>& set) {
    if (set.empty()) {
        return os;
    }

    os << aggregate(set);
    return os;
}

string aggregate(unordered_set<string> set) {
    if (set.empty()) return "";

    string cur = *set.begin();

    for (auto i = ++set.begin(); i != set.end(); i++) {
        cur += ", " + *i;
    }

    return cur;
}

bool contains(vector<string> arr, string str) {
    for (string cur : arr) {
        if (cur == str) {
            return true;
        }
    }

    return false;
}

unordered_set<string> simulate_nfa(NFA *nfa, string input) {
    unordered_set<string> matches;

    for (int start = 0; start < input.length(); start++) {
        set<int> stateSet = epsilon_closure(nfa, 0);

        for (int cur = start; cur < input.length(); cur++) {
            char ch = input[cur];
            stateSet = epsilon_closure(nfa, move(nfa, stateSet, ch));

            for (int state : stateSet) {
                nfa->curCharIndex[state] = cur;

                if (nfa->acceptStates[state]) {
                    int matchEnd = cur;

                    if (!nfa->backTo[state].empty()) {
                        int backToIndex = INT_MIN;

                        for (int backTo : nfa->backTo[state]) {
                            backToIndex = max(backToIndex, nfa->curCharIndex[backTo]);
                        }

                        matchEnd = backToIndex;
                    }

                    if (nfa->matchStart[state] && start == 0) {
                        matches.insert(input.substr(start, matchEnd - start + 1));
                    }
                    else if (nfa->matchEnd[state] && cur == input.length() - 1) {
                        matches.insert(input.substr(start, matchEnd - start + 1));
                    }
                    else if (nfa->matchStartAndEnd[state] && start == 0 && cur == input.length() - 1) {
                        matches.insert(input.substr(start, matchEnd - start + 1));
                    }
                    else if (nfa->notMatchStartAndNotMatchEnd[state]) {
                        matches.insert(input.substr(start, matchEnd - start + 1));
                    }

                    break;
                }
            }
        }
    }

    return matches;

}

unordered_set<string> simulate_dfa(DFA *dfa, string input) {
    unordered_set<string> matches;

    for (int start = 0; start < input.length(); start++) {
        int state = 0;

        for (int cur = start; cur < input.length(); cur++) {
            char ch = input[cur];

            if (dfa->transitions[state][ch] >= 0) {
                state = dfa->transitions[state][ch];
                dfa->curCharIndex[state] = cur;
            }
            else {
                break;
            }

            if (dfa->acceptStates[state]) {
                int matchEnd = cur;

                if (!dfa->backTo[state].empty()) {
                    int backToIndex = INT_MIN;

                    for (auto backTo : dfa->backTo[state]) {
                        backToIndex = max(backToIndex, dfa->curCharIndex[backTo]);
                    }

                    matchEnd = backToIndex;
                }

                if (dfa->matchStartActionNum[state] != INT_MAX && start == 0) {
                    matches.insert(input.substr(start, matchEnd - start + 1));
                }
                else if (dfa->matchEndActionNum[state] != INT_MAX && cur == input.length() - 1) {
                    matches.insert(input.substr(start, matchEnd - start + 1));
                }
                else if (dfa->matchStartAndEndActionNum[state] != INT_MAX && start == 0 && cur == input.length() - 1) {
                    matches.insert(input.substr(start, matchEnd - start + 1));
                }
                else if (dfa->actionNum[state] != INT_MAX) {
                    matches.insert(input.substr(start, matchEnd - start + 1));
                }
            }
        }
    }

    return matches;
}

