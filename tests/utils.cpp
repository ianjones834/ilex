//
// Created by ian on 1/12/25.
//

#include "utils.h"

#include <queue>
#include <unordered_map>

#include "../src/nfa.h"

ostream& operator<<(ostream& os, const NFA& nfa) {
    unordered_map<State*, int> stateNumMap;

    stateNumMap.clear();
    queue<State*> stateQueue;
    int stateNum = 0;

    stateQueue.push(nfa.start);
    stateNumMap[nfa.start] = stateNum++;

    while (!stateQueue.empty()) {
        int curLevel = stateQueue.size();

        while (curLevel--) {
            State* curState = stateQueue.front();
            stateQueue.pop();

            for (const auto& pair : curState->transitions) {
                for (State* next : pair.second) {
                    if (!stateNumMap.contains(next)) {
                        stateNumMap[next] = stateNum++;
                        stateQueue.push(next);
                    }
                }
            }
        }
    }

    for (auto pair : stateNumMap) {
        os << "State: " << pair.second << (pair.first->acceptingState ? " (Accepting) " : "") << " Transitions: " << endl;

        for (const auto& transitionPair : pair.first->transitions) {
            for (State* cur : transitionPair.second) {
                os << "\t" << transitionPair.first << " -> " << stateNumMap[cur] << endl;
            }
        }
    }

    os << endl;

    return os;
}

ostream& operator<<(ostream& os, const DFA& dfa) {
    for (const auto& pair : dfa.transitions) {
        os << "State: " << pair.first << ((dfa.acceptedStates.contains(pair.first)) ? " (Accepting) " : " ") << "Transitions: " << endl;

        for (const auto& transitionPair : pair.second) {
            os << "\t" << transitionPair.first << " -> " << transitionPair.second << endl;
        }
    }

    return os;
}

bool simulate_nfa(NFA *nfa, string input) {
    set<State*> stateSet = epsilon_closure(nfa->start);

    for (char ch : input) {
        stateSet = epsilon_closure(move(stateSet, ch));
        if (stateSet.empty()) return false;
    }

    for (State* state : stateSet) {
        if (state->acceptingState == true) {
            return true;
        }
    }

    return false;
}

bool simulate_dfa(DFA *dfa, string input) {
    int cur = 0;

    for (char ch : input) {
        if (dfa->transitions[cur].contains(ch)) {
            cur = dfa->transitions[cur][ch];
        }
        else {
            return false;
        }
    }

    return dfa->acceptedStates.contains(cur);
}

