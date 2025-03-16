//
// Created by ian on 1/12/25.
//

#include "utils.h"

#include <limits.h>
#include <queue>
#include <unordered_map>

#include "../src/Automata/NFA.h"

ostream& operator<<(ostream& os, const NFA& nfa) {
    unordered_map<NFAState*, int> stateNumMap;

    stateNumMap.clear();
    queue<NFAState*> stateQueue;
    int stateNum = 0;

    stateQueue.push(nfa.start);
    stateNumMap[nfa.start] = stateNum++;

    while (!stateQueue.empty()) {
        int curLevel = stateQueue.size();

        while (curLevel--) {
            NFAState* curState = stateQueue.front();
            stateQueue.pop();

            for (const auto& pair : curState->transitions) {
                for (NFAState* next : pair.second) {
                    if (!stateNumMap.contains(next)) {
                        stateNumMap[next] = stateNum++;
                        stateQueue.push(next);
                    }
                }
            }
        }
    }

    for (auto pair : stateNumMap) {
        os << "State: " << pair.second << (pair.first->acceptState ? " (Accepting) " : "") << " Transitions: " << endl;

        for (const auto& transitionPair : pair.first->transitions) {
            for (NFAState* cur : transitionPair.second) {
                os << "\t" << transitionPair.first << " -> " << stateNumMap[cur] << endl;
            }
        }
    }

    os << endl;

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
        set<NFAState*> stateSet = epsilon_closure(nfa->start);

        for (int cur = start; cur < input.length(); cur++) {
            char ch = input[cur];
            stateSet = epsilon_closure(move(stateSet, ch));

            for (NFAState* state : stateSet) {
                state->curCharIndex = cur;

                if (state->acceptState) {
                    int matchEnd = cur;

                    if (!state->backTo.empty()) {
                        int backToIndex = INT_MIN;

                        for (NFAState* backTo : state->backTo) {
                            backToIndex = max(backToIndex, backTo->curCharIndex);
                        }

                        matchEnd = backToIndex;
                    }

                    if (state->matchStart && start == 0) {
                        matches.insert(input.substr(start, matchEnd - start + 1));
                    }
                    else if (state->matchEnd && cur == input.length() - 1) {
                        matches.insert(input.substr(start, matchEnd - start + 1));
                    }
                    else if (state->matchStartAndEnd && start == 0 && cur == input.length() - 1) {
                        matches.insert(input.substr(start, matchEnd - start + 1));
                    }
                    else if (state->notMatchStartAndNotMatchEnd) {
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
        DFAState* state = dfa->start;

        for (int cur = start; cur < input.length(); cur++) {
            char ch = input[cur];

            if (state->transitions.contains(ch)) {
                state = state->transitions[ch];
                state->curCharIndex = cur;
            }
            else {
                break;
            }

            if (state->acceptState) {
                int matchEnd = cur;

                if (!state->backTo.empty()) {
                    int backToIndex = INT_MIN;

                    for (auto backTo : state->backTo) {
                        backToIndex = max(backToIndex, backTo->curCharIndex);
                    }

                    matchEnd = backToIndex;
                }

                if (state->matchStart && start == 0) {
                    matches.insert(input.substr(start, matchEnd - start + 1));
                }
                else if (state->matchEnd && cur == input.length() - 1) {
                    matches.insert(input.substr(start, matchEnd - start + 1));
                }
                else if (state->matchStartAndEnd && start == 0 && cur == input.length() - 1) {
                    matches.insert(input.substr(start, matchEnd - start + 1));
                }
                else if (state->notMatchStartAndNotMatchEnd) {
                    matches.insert(input.substr(start, matchEnd - start + 1));
                }
            }
        }
    }

    return matches;
}

