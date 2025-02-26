//
// Created by ian on 1/12/25.
//

#ifndef DFA_H
#define DFA_H

#include <iostream>
#include <unordered_map>
#include <set>
#include <unordered_set>
using namespace std;

struct DFA {
    int start;
    unordered_set<int> states;
    unordered_map<int, unordered_map<char, int>> transitions;
    unordered_set<int> acceptedStates;
    unordered_map<int, unordered_set<int>> stateToRuleMap;
    unordered_map<int, bool> matchStart;
    unordered_map<int, bool> matchEnd;
};

bool dfa_serialize(DFA*, ostream&);

ostream& operator<<(ostream&, const unordered_set<int>&);
ostream& operator<<(ostream&, const unordered_map<int, unordered_map<char, int>>&);
ostream& operator<<(ostream&, const unordered_set<int>&);

#endif //DFA_H