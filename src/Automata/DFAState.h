//
// Created by ian on 3/12/25.
//

#ifndef DFASTATE_H
#define DFASTATE_H

#include <unordered_map>
#include <set>

#include "NFAState.h"
using namespace std;

struct DFAState {
    int actionNum;
    int matchStartActionNum;
    int matchEndActionNum;
    int matchStartAndEndActionNum;

    bool acceptState;
    bool matchStart;
    bool matchEnd;
    bool matchStartAndEnd;

    int curCharIndex;
    unordered_set<DFAState*> backTo;

    unordered_map<char, DFAState*> transitions;


    DFAState();
    DFAState(set<NFAState*>);
};

#endif //DFASTATE_H
