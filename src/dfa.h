//
// Created by ian on 1/12/25.
//

#ifndef DFA_H
#define DFA_H

#include <unordered_map>
#include <set>
using namespace std;

struct DFA {
    int startState;
    set<int> alphabet;
    set<int> states;
    int** transitionTable;
    set<int> acceptedStates;
};

bool dfa_serialize(DFA*);

#endif //DFA_H