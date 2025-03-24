//
// Created by ian on 1/12/25.
//

#ifndef DFA_H
#define DFA_H

#include <unordered_set>
#include <fstream>
#include <unordered_map>

#include "NFA.h"
using namespace std;

struct DFA {
    // DFAState* start;
    // unordered_set<DFAState*> states;

    unordered_map<string, int> nameToState;

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
    DFA(int);
};

ostream& operator<<(ostream&, const DFA&);
DFA* dfa_nUnion(vector<DFA*>, unordered_map<int, string>);

#endif //DFA_H