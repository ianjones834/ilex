//
// Created by ian on 1/12/25.
//

#ifndef UTILS_H
#define UTILS_H

#include "../src/nfa.h"
#include "../src/dfa.h"
#include<string>
#include<iostream>
#include<unordered_set>
using namespace std;

ostream& operator<<(ostream&, const NFA&);
ostream& operator<<(ostream&, const DFA&);

bool simulate_nfa(NFA*, string);
bool simulate_dfa(DFA*, string);

#endif //UTILS_H
