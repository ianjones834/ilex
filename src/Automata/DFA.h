//
// Created by ian on 1/12/25.
//

#ifndef DFA_H
#define DFA_H

#include <unordered_set>
#include <fstream>

#include "NFA.h"
using namespace std;

struct DFA {
    // DFAState* start;
    // unordered_set<DFAState*> states;

    int stateNum;

    vector<bool> acceptStates;

    vector<int> actionNum;
    vector<int> matchStartActionNum;
    vector<int> matchEndActionNum;
    vector<int> matchStartAndEndActionNum;

    vector<int> curCharIndex;

    vector<unordered_set<int>> backTo;
    vector<array<int, 128>> transitions;

    DFA(NFA*);
};

ostream& operator<<(ostream&, const DFA&);

#endif //DFA_H