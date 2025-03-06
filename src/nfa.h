//
// Created by ian on 1/12/25.
//

#ifndef NFA_H
#define NFA_H

#include "state.h"
using namespace std;

struct NFA {
    State* start;
    set<State*> states;
};

NFA* nfa_copy(NFA*);

NFA* nfa_new_single_char(char);

NFA* nfa_union(NFA*, NFA*);

NFA* nfa_concat(NFA*, NFA*);

NFA* nfa_optional(NFA*);

NFA* nfa_zero_or_more(const NFA*);

NFA* nfa_one_or_more(NFA*);

NFA* nfa_range(set<char>, set<pair<char, char>>);

NFA* nfa_any();

NFA* nfa_notInRange(set<char>);

NFA* nfa_repeat(NFA*, int, int);

set<State*> epsilon_closure(State*);

set<State*> epsilon_closure(set<State*>);

set<State*> move(set<State*>, char);

#endif //NFA_H
