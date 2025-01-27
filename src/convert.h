//
// Created by ian on 1/12/25.
//

#ifndef CONVERT_H
#define CONVERT_H

#include <string>

#include "dfa.h"
#include "nfa.h"

DFA* convert(NFA*);
unordered_set<char>* getAlphabet(NFA*);
#endif //CONVERT_H
