//
// Created by ian on 1/12/25.
//

#include "utils.h"

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

ostream& operator<<(ostream& os, const DFA& dfa) {
    return os;
}

bool simulate_nfa(NFA *nfa, string input) {
    set<NFAState*> stateSet = epsilon_closure(nfa->start);

    for (char ch : input) {
        stateSet = epsilon_closure(move(stateSet, ch));
        if (stateSet.empty()) return false;
    }

    for (NFAState* state : stateSet) {
        if (state->acceptState == true) {
            return true;
        }
    }

    return false;
}

bool simulate_dfa(DFA *dfa, string input) {
    DFAState* cur = dfa->start;

    for (char ch : input) {
        if (cur->transitions.contains(ch)) {
            cur = cur->transitions[ch];
        }
        else {
            return false;
        }
    }

    return cur->acceptState;;
}

