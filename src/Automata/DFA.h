//
// Created by ian on 1/12/25.
//

#ifndef DFA_H
#define DFA_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "NFA.h"
#include "DFAState.h"
using namespace std;

struct DFA {
    DFAState* start;
    unordered_set<DFAState*> states;

    DFA(NFA*);
};

ostream& operator<<(ostream& os, const DFA& dfa);

unordered_set<char>* getAlphabet(NFA*);

#endif //DFA_H