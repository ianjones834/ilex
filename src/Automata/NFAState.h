//
// Created by ian on 1/15/25.
//

#ifndef STATE_H
#define STATE_H

#include <unordered_set>
#include <unordered_map>

#include "NFA.h"
using namespace std;

struct NFAState {
    int actionNum;
    bool acceptState;
    bool matchStart;
    bool matchEnd;
    bool matchStartAndEnd;

    int curCharIndex;
    unordered_set<NFAState*> backTo;

    unordered_map<char, unordered_set<NFAState*>> transitions;

    NFAState();
};

#endif //STATE_H
