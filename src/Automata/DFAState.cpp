//
// Created by ian on 3/12/25.
//
#include "DFAState.h"

#include <climits>

DFAState::DFAState() {
    matchStartActionNum = INT_MAX;
    matchEndActionNum = INT_MAX;
    matchStartAndEndActionNum = INT_MAX;
    actionNum = INT_MAX;
    acceptState = false;
}

DFAState::DFAState(set<NFAState *> nfaStates) {
    acceptState = false;
    matchStartActionNum = INT_MAX;
    matchEndActionNum = INT_MAX;
    matchStartAndEndActionNum = INT_MAX;
    actionNum = INT_MAX;

    for (auto state : nfaStates) {
        if (state->acceptState) {
            acceptState = true;
            matchStartActionNum = min(matchStartActionNum, state->matchStart ? state->actionNum : INT_MAX);
            matchEndActionNum = min(matchEndActionNum, state->matchEnd ? state->actionNum : INT_MAX);
            matchStartAndEndActionNum = min(matchStartAndEndActionNum, state->matchStartAndEnd ? state->actionNum : INT_MAX);
            actionNum = min(actionNum, state->actionNum);
        }
    }
}
