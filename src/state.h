//
// Created by ian on 1/15/25.
//

#ifndef STATE_H
#define STATE_H
#include <set>
#include <unordered_map>
using namespace std;

struct State {
    int nfaNum;
    unordered_map<char, set<State*>> transitions;
    bool acceptingState;
};

#endif //STATE_H
