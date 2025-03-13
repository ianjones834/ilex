//
// Created by ian on 1/12/25.
//

#ifndef NFA_H
#define NFA_H

#include <set>
#include <unordered_set>
#include "NFAState.h"
using namespace std;

struct NFA {
    NFAState* start;
    unordered_set<NFAState*> states;
};

NFA* nfa_copy(NFA*);

NFA* nfa_new_single_char(char);

NFA* nfa_union(NFA*, NFA*);

NFA* nfa_concat(NFA*, NFA*);

NFA* nfa_optional(NFA*);

NFA* nfa_zero_or_more(const NFA*);

NFA* nfa_one_or_more(NFA*);

NFA* nfa_range(unordered_set<char>, unordered_set<pair<char, char>*>);

NFA* nfa_any();

NFA* nfa_notInRange(unordered_set<char>);

NFA* nfa_repeat(NFA*, int, int);

set<NFAState*> epsilon_closure(NFAState*);

set<NFAState*> epsilon_closure(set<NFAState*>);

set<NFAState*> move(set<NFAState*>, char);

#endif //NFA_H
