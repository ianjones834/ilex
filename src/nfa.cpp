//
// Created by ian on 1/12/25.
//

#include "nfa.h"
#include <algorithm>
#include <iterator>

NFA::~NFA() {
    for (const int i : states) {
        delete[] transitionTable[i];
    }

    delete[] transitionTable;
}


NFA* nfa_new_single_char(const char ch) {
    NFA* nfa = new NFA;

    nfa->states = set<int>{0,1};
    nfa->alphabet = set<int>{ch};
    nfa->startState = 0;
    nfa->transitionTable = new set<int>*[2];

    for (int i = 0; i < 2; i++) {
        nfa->transitionTable[i] = new set<int>[128];
    }

    nfa->transitionTable[0][ch] = set<int>{1};
    nfa->acceptedStates = set<int>{1};

    return nfa;
}

NFA* nfa_union(NFA* nfa1, NFA* nfa2) {
    NFA* nfa = new NFA;

    nfa->startState = 0;
    nfa->states = set<int>{0};

    for (const char ch : nfa1->alphabet) {
        nfa->alphabet.insert(ch);
    }

    for (const char ch : nfa2->alphabet) {
        nfa->alphabet.insert(ch);
    }

    const int transitionTableStateCount = 1 + nfa1->states.size() + nfa2->states.size();

    nfa->transitionTable = new set<int>*[transitionTableStateCount];

    for (int i = 0; i < transitionTableStateCount; i++) {
        nfa->transitionTable[i] = new set<int>[128];
    }

    const int n = nfa1->states.size();

    nfa->transitionTable[0][0] = set<int>{nfa1->startState + 1, nfa2->startState + 1 + n};

    for (const int i : nfa1->states) {
        nfa->states.insert(i + 1);

        nfa->transitionTable[i + 1][0] = nfa1->transitionTable[i][0];

        for (const char j : nfa1->alphabet) {
            for (const int k : nfa1->transitionTable[i][j]) {
                nfa1->transitionTable[i][j].erase(k);
                nfa1->transitionTable[i][j].insert(k + 1);
            }

            nfa->transitionTable[i + 1][j] = nfa1->transitionTable[i][j];
        }
    }

    for (const int i : nfa2->states) {
        nfa->states.insert(i + 1 + nfa1->states.size());

        nfa->transitionTable[i + 1 + nfa1->states.size()][0] = nfa2->transitionTable[i][0];

        for (const char j : nfa2->alphabet) {
            for (const int k : nfa2->transitionTable[i][j]) {
                nfa2->transitionTable[i][j].erase(k);
                nfa2->transitionTable[i][j].insert(k + 1 + nfa1->states.size());
            }

            nfa->transitionTable[i + 1 + nfa1->states.size()][j] = nfa2->transitionTable[i][j];
        }
    }

    for (const int i : nfa1->acceptedStates) {
        nfa->acceptedStates.insert(i + 1);
    }

    for (const int i : nfa2->acceptedStates) {
        nfa->acceptedStates.insert(i + 1 + nfa1->states.size());
    }

    return nfa;
}
