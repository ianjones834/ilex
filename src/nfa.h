//
// Created by ian on 1/12/25.
//

#ifndef NFA_H
#define NFA_H

#include<set>
#include "state.h"
using namespace std;

struct NFA {
    State* start;
    set<State*> states;

    ~NFA();
};

NFA* nfa_new_single_char(char);

NFA* nfa_union(NFA*, NFA*);

NFA* nfa_concat(NFA*, NFA*);

NFA* nfa_optional(NFA*);

NFA* nfa_zero_or_more(const NFA*, bool oneOrMore = false);

NFA* nfa_one_or_more(NFA*);

set<State*> epsilon_closure(NFA*, State*);

set<State*> epsilon_closure(NFA*, set<State*>&);

set<State*> move(NFA*, set<State*>&, char);

#endif //NFA_H
