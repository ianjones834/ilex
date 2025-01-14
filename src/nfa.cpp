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

    delete nfa1;
    delete nfa2;

    return nfa;
}

NFA *nfa_concat(NFA *nfa1, NFA *nfa2) {
    NFA* nfa = new NFA;

    nfa->startState = nfa1->startState;

    for (int i : nfa1->alphabet) {
        nfa->alphabet.insert(i);
    }

    for (int i : nfa2->alphabet) {
        nfa->alphabet.insert(i);
    }



    const int nfa1StateSize = nfa1->states.size();
    const int nfa2StateSize = nfa2->states.size();

    for (int i : nfa2->acceptedStates) {
        nfa->acceptedStates.insert(i + nfa1StateSize);
    }

    nfa->transitionTable = new set<int>*[nfa1StateSize + nfa2StateSize];

    for (int i = 0; i < nfa1StateSize + nfa2StateSize; i++) {
        nfa->transitionTable[i] = new set<int>[128];
    }

    for (int i : nfa1->states) {
        nfa->states.insert(i);

        for (int j : nfa1->alphabet) {
            nfa->transitionTable[i][j] = nfa1->transitionTable[i][j];
        }
    }

    for (int i : nfa1->acceptedStates) {
        nfa->transitionTable[i][0].insert(nfa2->startState + nfa1StateSize);
    }

    for (int i : nfa2->states) {
        nfa->states.insert(i + nfa1StateSize);

        for (int j  : nfa2->alphabet) {
            for (int k : nfa2->transitionTable[i][j]) {
                nfa2->transitionTable[i][j].erase(k);
                nfa2->transitionTable[i][j].insert(k + nfa1StateSize);
            }

            nfa->transitionTable[i + nfa1StateSize][j] = nfa2->transitionTable[i][j];
        }
    }

    delete nfa1;
    delete nfa2;

    return nfa;
}

NFA *nfa_zero_or_more(const NFA *nfa1, const bool oneOrMore) {
    NFA *nfa = new NFA;

    nfa->startState = 0;
    nfa->alphabet = nfa1->alphabet;

    nfa->states.insert(0);

    nfa->acceptedStates.insert(0);

    for (int i : nfa1->acceptedStates) {
        nfa->acceptedStates.insert(i + 1);
    }

    int nfa1StateSize = nfa1->states.size();

    nfa->transitionTable = new set<int>*[1 + nfa1StateSize];

    for (int i = 0; i < 1 + nfa1StateSize; i++) {
        nfa->transitionTable[i] = new set<int>[128];
    }

    for (int i : nfa1->states) {
        nfa->states.insert(i + 1);

        for (int j : nfa1->alphabet) {
            for (int k : nfa1->transitionTable[i][j]) {
                nfa1->transitionTable[i][j].erase(k);
                nfa1->transitionTable[i][j].insert(k + 1);
            }

            nfa->transitionTable[i + 1][j] = nfa1->transitionTable[i][j];
        }

        if (nfa1->acceptedStates.contains(i)) {
            nfa->transitionTable[i + 1][0].insert(0);
        }
    }

    nfa->transitionTable[0][0].insert(nfa1->startState + 1);

    if (!oneOrMore) delete nfa1;

    return nfa;
}

NFA *nfa_one_or_more(NFA *nfa1) {
    return nfa_concat(nfa1, nfa_zero_or_more(nfa1, true));
}

NFA *nfa_optional(NFA *nfa1) {
    nfa1->acceptedStates.insert(nfa1->startState);
    return nfa1;
}


