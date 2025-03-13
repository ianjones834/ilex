//
// Created by ian on 3/12/25.
//
#include "NFAState.h"

#include <climits>

NFAState::NFAState() {
    actionNum = INT_MAX;
    acceptState = false;
    matchEnd = false;
    matchStart = false;
    matchStartAndEnd = false;
    secondaryNfa = nullptr;
}
