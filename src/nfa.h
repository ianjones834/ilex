//
// Created by ian on 1/12/25.
//

#ifndef NFA_H
#define NFA_H

#include<unordered_map>
#include<set>
using namespace std;

struct NFA {
    int startState;
    set<int> alphabet;
    set<int> states;
    set<int>** transitionTable;
    set<int> acceptedStates;
    unordered_map<int, void*(*)()> actions;
};

NFA* nfa_new_single_char(char);

NFA* nfa_union(NFA*, NFA*);

NFA* nfa_concat(NFA*, NFA*);

NFA* nfa_optional(NFA*);

NFA* nfa_zero_or_more(NFA*);

NFA* nfa_one_or_more(NFA*);

set<int> epsilon_closure(int);

set<int> epsilon_closure(set<int>);

set<int> move(set<int>, char);

#endif //NFA_H
