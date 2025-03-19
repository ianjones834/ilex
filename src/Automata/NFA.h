//
// Created by ian on 1/12/25.
//

#ifndef NFA_H
#define NFA_H

#include <set>
#include <unordered_set>
#include <vector>
#include <array>
// #include "NFAState.h"
using namespace std;

struct NFA {
    // NFAState* start;
    // unordered_set<NFAState*> states;
    int stateNum;

    vector<bool> acceptStates;

    vector<int> actionNum;

    vector<bool> matchStart;
    vector<bool> matchEnd;
    vector<bool> matchStartAndEnd;
    vector<bool> notMatchStartAndNotMatchEnd;

    vector<int> curCharIndex;

    vector<unordered_set<int>> backTo;

    vector<array<unordered_set<int>, 128>> transitions;

    NFA(int);
};



NFA* nfa_new_single_char(char);

NFA* nfa_union(NFA*, NFA*, bool = false);

NFA* nfa_nUnion(vector<NFA*>, bool = false);

NFA* nfa_concat(NFA*, NFA*, bool = false);

NFA* nfa_optional(NFA*);

NFA* nfa_zero_or_more(const NFA*);

NFA* nfa_one_or_more(NFA*);

NFA* nfa_range(unordered_set<char>, unordered_set<pair<char, char>*>);

NFA* nfa_any();

NFA* nfa_notInRange(unordered_set<char>);

NFA* nfa_repeat(NFA*, int, int);

set<int> epsilon_closure(NFA*, int);

set<int> epsilon_closure(NFA*, set<int>);

set<int> move(NFA*, set<int>, char);

#endif //NFA_H
