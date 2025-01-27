//
// Created by ian on 1/12/25.
//

#include "convert.h"

#include <map>
#include <stack>

DFA* convert(NFA* nfa) {
    DFA* dfa = new DFA;
    dfa->start = 0;

    map<set<State*>, int> stateMap;
    unordered_set<char>* alphabet = getAlphabet(nfa);

    stack<set<State*>> toProcess;

    int stateCount = 0;
    set<State*> start = epsilon_closure(nfa, nfa->start);
    toProcess.push(start);
    stateMap[start] = stateCount++;

    while (!toProcess.empty()) {
        set<State*> cur = toProcess.top();
        toProcess.pop();

        for (State* state : cur) {
            if (state->acceptingState) {
                dfa->acceptedStates.insert(stateMap[cur]);
                break;
            }
        }

        for (char ch : *alphabet) {
            set<State*> next = epsilon_closure(nfa, move(nfa, cur, ch));

            if (!stateMap.contains(next)) {
                stateMap[next] = stateCount++;
                toProcess.push(next);
            }

            dfa->transitions[stateMap[cur]][ch] = stateMap[next];
        }
    }

    delete alphabet;
    return dfa;
}

unordered_set<char>* getAlphabet(NFA* nfa) {
    unordered_set<char>* alphabet = new unordered_set<char>;

    for (State* state : nfa->states) {
        for (const auto& pair : state->transitions) {
            alphabet->insert(pair.first);
        }
    }

    return alphabet;
}