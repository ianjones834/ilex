//
// Created by ian on 1/12/25.
//

#ifndef DFA_H
#define DFA_H

#include <ostream>
#include <unordered_map>
#include <set>
#include <unordered_set>

#include "state.h"
using namespace std;

struct DFA {
    int start;
    unordered_set<int> states;
    unordered_map<int, unordered_map<char, int>> transitions;
    unordered_set<int> acceptedStates;
    unordered_map<int, set<int>> stateToRuleMap;
};

bool dfa_serialize(DFA*, ostream&);

#endif //DFA_H