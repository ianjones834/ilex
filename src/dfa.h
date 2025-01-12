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
    set<int>** transitionTable;
    set<int> acceptedStates;
    unordered_map<int, void*(*)()> actions;
};

bool dfa_serialize(DFA*);

#endif //DFA_H