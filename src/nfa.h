//
// Created by ian on 1/12/25.
//

#ifndef NFA_H
#define NFA_H

#include <unordered_set>

#include "state.h"
using namespace std;

struct NFA {
    State* start;
    unordered_set<State*> states;
};

NFA* nfa_copy(NFA*);

NFA* nfa_new_single_char(char);

NFA* nfa_union(NFA*, NFA*);

NFA* nfa_concat(NFA*, NFA*);

NFA* nfa_optional(NFA*);

NFA* nfa_zero_or_more(const NFA*, bool oneOrMore = false);

NFA* nfa_one_or_more(NFA*);

NFA* nfa_range(char, char);

unordered_set<State*> epsilon_closure(NFA*, State*);

unordered_set<State*> epsilon_closure(NFA*, unordered_set<State*>);

unordered_set<State*> move(NFA*, unordered_set<State*>, char);

#endif //NFA_H
