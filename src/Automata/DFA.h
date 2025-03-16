//
// Created by ian on 1/12/25.
//

#ifndef DFA_H
#define DFA_H

#include <unordered_set>
#include <fstream>

#include "NFA.h"
#include "DFAState.h"
using namespace std;

struct DFA {
    DFAState* start;
    unordered_set<DFAState*> states;

    DFA(NFA*);
};

ostream& operator<<(ostream&, const DFA&);
ostream& operator<<(ostream&, const DFAState&);

unordered_set<char> getAlphabet(NFA*);

#endif //DFA_H