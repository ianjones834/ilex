//
// Created by ian on 1/12/25.
//

#include "DFA.h"

#include <map>
#include <set>
#include <stack>

int STATE_NUM = 0;

bool dfa_serialize(DFA* dfa, ostream& out) {
    return false;
}

// DFA to NFA Convert

DFA::DFA(NFA* nfa) {
    map<set<NFAState*>, DFAState*> stateMap;
    unordered_set<char>* alphabet = getAlphabet(nfa);

    stack<set<NFAState*>> nextState;

    set<NFAState*> start = epsilon_closure(nfa->start);
    nextState.push(start);
    this->start = new DFAState(start);
    stateMap[start] = this->start;
    this->states.insert(this->start);

    while (!nextState.empty()) {
        set<NFAState*> cur = nextState.top();
        nextState.pop();

        for (char ch : *alphabet) {
            set<NFAState*> next = epsilon_closure(move(cur, ch));

            if (!stateMap.contains(next)) {
                DFAState* newState = new DFAState(next);

                this->states.insert(newState);
                stateMap[next] = newState;
                nextState.push(next);
            }

            stateMap[cur]->transitions[ch] = stateMap[next];
        }
    }

    delete alphabet;
}

unordered_set<char>* getAlphabet(NFA* nfa) {
    unordered_set<char>* alphabet = new unordered_set<char>;

    for (NFAState* state : nfa->states) {
        for (const auto& pair : state->transitions) {
            if (pair.first != 0) {
                alphabet->insert(pair.first);
            }
        }
    }

    return alphabet;
}