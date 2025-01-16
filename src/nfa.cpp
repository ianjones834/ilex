//
// Created by ian on 1/12/25.
//

#include "nfa.h"
#include "../tests/utils.h"
#include <stack>


NFA* nfa_new_single_char(const char ch) {
    NFA* nfa = new NFA;

    auto* start = new State;
    auto* end = new State;

    end->acceptingState = true;
    start->transitions.emplace(ch, set<State*>{end});

    nfa->states.insert(start);
    nfa->states.insert(end);

    return nfa;
}

NFA* nfa_union(NFA* nfa1, NFA* nfa2) {
    NFA* nfa = new NFA;

    nfa->start = new State;
    nfa->states.insert(nfa->start);

    nfa->start->transitions.emplace(0, set<State*>{nfa1->start, nfa2->start});

    nfa->states.insert(nfa1->states.begin(), nfa1->states.end());
    nfa->states.insert(nfa2->states.begin(), nfa2->states.end());

    delete nfa1;
    delete nfa2;

    return nfa;
}

NFA *nfa_concat(NFA *nfa1, NFA *nfa2) {
    NFA* nfa = new NFA;

    nfa->start = nfa1->start;

    for (State* state : nfa1->states) {
        if (state->acceptingState == true) {
            state->acceptingState = false;

            if (state->transitions.contains(0)) {
                state->transitions.emplace(0, set<State*>{nfa2->start});
            }
            else {
                state->transitions[0].insert(nfa2->start);
            }
        }
    }

    nfa->states.insert(nfa1->states.begin(), nfa1->states.end());
    nfa->states.insert(nfa2->states.begin(), nfa2->states.end());

    delete nfa1;
    delete nfa2;

    return nfa;
}

NFA *nfa_zero_or_more(const NFA *nfa1, const bool oneOrMore) {
    NFA *nfa = new NFA;

    nfa->start = new State;
    nfa->start->acceptingState = true;
    nfa->start->transitions.emplace(0, set<State*>{nfa1->start});

    for (State* state : nfa1->states) {
        if (state->acceptingState == true) {
            if (state->transitions.contains(0)) {
                state->transitions[0].insert(nfa->start);
            }
            else {
                state->transitions.emplace(0, set<State*>{nfa->start});
            }
        }
    }

    if (!oneOrMore) delete nfa1;

    return nfa;
}

NFA *nfa_one_or_more(NFA *nfa1) {
    return nfa_concat(nfa1, nfa_zero_or_more(nfa1, true));
}

NFA *nfa_optional(NFA *nfa1) {
    nfa1->start->acceptingState = true;
    return nfa1;
}


set<State*> epsilon_closure(NFA* nfa, State* s) {
    set<State*> res;
    stack<State*> toSearch;

    res.insert(s);
    toSearch.push(s);

    while (!toSearch.empty()) {
        State* cur = toSearch.top();
        toSearch.pop();

        if (cur->transitions.contains(0)) {
            for (State* state : cur->transitions[0]) {
                if (!res.contains(state)) {
                    res.insert(state);
                    toSearch.push(state);
                }
            }
        }
    }

    return res;
}

set<State*> epsilon_closure(NFA *nfa, const set<State*>& stateSet) {
    set<State*> res;
    stack<State*> toSearch;

    for (State* i : stateSet) {
        toSearch.push(i);
        res.insert(i);
    }

    while (!toSearch.empty()) {
        State* cur = toSearch.top();
        toSearch.pop();

        if (cur->transitions.contains(0)) {
            for (State* state : cur->transitions[0]) {
                if (!res.contains(state)) {
                    res.insert(state);
                    toSearch.push(state);
                }
            }
        }
    }

    return res;
}

set<State*> move(NFA *nfa, const set<State*>& T, char ch) {
    set<State*> res;

    for (State* cur : T) {
        if (cur->transitions.contains(ch)) {
            for (State* state : cur->transitions[ch]) {
                res.insert(state);
            }
        }
    }

    return res;
}


