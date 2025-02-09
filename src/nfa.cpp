//
// Created by ian on 1/12/25.
//

#include "nfa.h"
#include <queue>
#include <stack>
#include <stdexcept>
#include <unordered_set>

NFA *nfa_copy(NFA *nfa1) {
    NFA* nfa = new NFA;

    unordered_map<State*, State*> oldToNewStates;
    unordered_set<State*> seen;
    queue<State*> stateQueue;

    nfa->start = new State;
    nfa->start->acceptingState = nfa1->start->acceptingState;
    nfa->states.insert(nfa->start);

    stateQueue.push(nfa1->start);
    seen.insert(nfa1->start);

    oldToNewStates.emplace(nfa1->start, nfa->start);

    while (!stateQueue.empty()) {
        int curLevelSize = stateQueue.size();

        while (curLevelSize--) {
            State* cur = stateQueue.front();
            stateQueue.pop();

            for (auto pair : cur->transitions) {
                for (State* state : pair.second) {
                    if (!seen.contains(state)) {
                        State* newState = new State;
                        newState->acceptingState = state->acceptingState;

                        oldToNewStates[state] = newState;
                        oldToNewStates[cur]->transitions[pair.first].insert(newState);

                        nfa->states.insert(newState);

                        stateQueue.push(state);
                        seen.insert(state);
                    }
                }
            }
        }
    }

    return nfa;
}


NFA* nfa_new_single_char(const char ch) {
    NFA* nfa = new NFA;

    auto* start = new State;
    auto* end = new State;

    end->acceptingState = true;
    start->transitions.emplace(ch, set<State*>{end});

    nfa->start = start;
    nfa->start->acceptingState = false;

    nfa->states.insert(start);
    nfa->states.insert(end);

    return nfa;
}

NFA* nfa_union(NFA* nfa1, NFA* nfa2) {
    NFA* nfa = new NFA;

    nfa->start = new State;
    nfa->start->acceptingState = false;

    nfa->states.insert(nfa->start);

    nfa->start->transitions.emplace(0, set<State*>{nfa1->start, nfa2->start});

    for (State* state : nfa1->states) {
        nfa->states.insert(state);
    }

    for (State* state : nfa2->states) {
        nfa->states.insert(state);
    }

    return nfa;
}

NFA *nfa_concat(NFA *nfa1, NFA *nfa2) {
    NFA* nfa = new NFA;

    nfa->start = nfa1->start;

    for (State* state : nfa1->states) {
        State* newState = state;
        if (newState->acceptingState == true) {
            newState->acceptingState = false;
            newState->transitions[0].insert(nfa2->start);
        }

        nfa->states.insert(newState);
    }

    for (State* state : nfa2->states) {
        nfa->states.insert(state);
    }


    return nfa;
}

NFA *nfa_zero_or_more(const NFA *nfa1, const bool oneOrMore) {
    NFA *nfa = new NFA;

    nfa->start = new State;
    nfa->start->acceptingState = true;
    nfa->start->transitions.emplace(0, set<State*>{nfa1->start});

    nfa->states.insert(nfa->start);

    for (State* state : nfa1->states) {
        State* newState = state;
        nfa->states.insert(newState);

        if (newState->acceptingState == true) {
                newState->transitions[0].insert(nfa->start);
        }
    }

    if (!oneOrMore) delete nfa1;

    return nfa;
}

NFA *nfa_one_or_more(NFA *nfa1) {
    NFA* nfaCopy = nfa_copy(nfa1);

    return nfa_concat(nfaCopy, nfa_zero_or_more(nfa1, true));
}

NFA *nfa_optional(NFA *nfa1) {
    for (State* state : nfa1->states) {
        if (state->acceptingState) {
            nfa1->start->transitions[0].insert(state);
        }
    }

    return nfa1;
}

NFA *nfa_range(char start, char end) {
    if (start > end) {
        throw std::invalid_argument("[end must be less than start]");
    }

    NFA* nfa = nfa_new_single_char(start);

    for (char ch = start + 1; ch <= end; ch += 1) {
        nfa = nfa_union(nfa, nfa_new_single_char(ch));
    }

    return nfa;
}



set<State*> epsilon_closure(NFA* nfa, State* s) {
    set<State*> res;
    stack<State*> toSearch;

    res.insert(s);
    toSearch.push(s);

    while (!toSearch.empty()) {
        State* cur = toSearch.top();
        toSearch.pop();

        for (State* state : cur->transitions[0]) {
            if (!res.contains(state)) {
                res.insert(state);
                toSearch.push(state);
            }
        }
    }

    return res;
}

set<State*> epsilon_closure(NFA *nfa, set<State*> stateSet) {
    set<State*> res;
    stack<State*> toSearch;

    for (State* i : stateSet) {
        toSearch.push(i);
        res.insert(i);
    }

    while (!toSearch.empty()) {
        State* cur = toSearch.top();
        toSearch.pop();

        for (State* state : cur->transitions[0]) {
            if (!res.contains(state)) {
                res.insert(state);
                toSearch.push(state);
            }
        }
    }

    return res;
}

set<State*> move(NFA *nfa, set<State*> T, char ch) {
    set<State*> res;

    for (State* cur : T) {
        for (State* state : cur->transitions[ch]) {
            res.insert(state);
        }
    }

    return res;
}


