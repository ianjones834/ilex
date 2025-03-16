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
    matchStart = false;
    matchEnd = false;
    matchStartAndEnd = false;

    curCharIndex = -1;

    stateNum = -1;
}

DFAState::DFAState(set<NFAState *> nfaStates) {
    acceptState = false;
    matchStart = false;
    matchEnd = false;
    matchStartAndEnd = false;
    notMatchStartAndNotMatchEnd = false;

    matchStartActionNum = INT_MAX;
    matchEndActionNum = INT_MAX;
    matchStartAndEndActionNum = INT_MAX;
    actionNum = INT_MAX;

    curCharIndex = -1;

    stateNum = -1;

    for (auto state : nfaStates) {
        if (state->acceptState) {
            acceptState = true;
            matchStart = matchStart || state->matchStart;
            matchEnd = matchEnd || state->matchEnd;
            notMatchStartAndNotMatchEnd = notMatchStartAndNotMatchEnd || state->notMatchStartAndNotMatchEnd;

            matchStartAndEnd = matchStartAndEnd || state->matchStartAndEnd;
            matchStartActionNum = min(matchStartActionNum, state->matchStart ? state->actionNum : INT_MAX);
            matchEndActionNum = min(matchEndActionNum, state->matchEnd ? state->actionNum : INT_MAX);
            matchStartAndEndActionNum = min(matchStartAndEndActionNum, state->matchStartAndEnd ? state->actionNum : INT_MAX);

            actionNum = min(actionNum, notMatchStartAndNotMatchEnd ? state->actionNum : INT_MAX);
        }
    }
}


