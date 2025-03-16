//
// Created by ian on 1/12/25.
//

#ifndef UTILS_H
#define UTILS_H

#include "../../src/Automata/NFA.h"
#include "../../src/Automata/DFA.h"
#include<string>
#include<iostream>
#include<unordered_set>
#include <vector>
using namespace std;

ostream& operator<<(ostream&, const NFA&);
ostream& operator<<(ostream&, const DFA&);

ostream& operator<<(ostream&, const unordered_set<string>&);
bool contains(vector<string>, string);
string aggregate(unordered_set<string>);

unordered_set<string> simulate_nfa(NFA*, string);
unordered_set<string> simulate_dfa(DFA*, string);
#endif //UTILS_H
