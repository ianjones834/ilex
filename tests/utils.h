//
// Created by ian on 1/12/25.
//

#ifndef UTILS_H
#define UTILS_H

#include "../src/nfa.h"
#include "../src/dfa.h"
#include<set>
#include <iostream>
using namespace std;

ostream& operator<<(ostream&, set<int>&);

ostream& operator<<(ostream&, NFA&);

ostream& operator<<(ostream&, DFA&);

bool operator==(NFA&, NFA&);

bool operator==(DFA&, DFA&);

#endif //UTILS_H
