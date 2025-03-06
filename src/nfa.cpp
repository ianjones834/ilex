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
    queue<State*> stateQueue;

    nfa->start = new State;
    nfa->start->acceptingState = nfa1->start->acceptingState;
    nfa->states.insert(nfa->start);

    stateQueue.push(nfa1->start);

    oldToNewStates.emplace(nfa1->start, nfa->start);

    while (!stateQueue.empty()) {
        int curLevelSize = stateQueue.size();

        while (curLevelSize--) {
            State* cur = stateQueue.front();
            stateQueue.pop();

            for (auto pair : cur->transitions) {
                for (State* state : pair.second) {
                    if (!oldToNewStates.contains(state)) {
                        State* newState = new State;
                        newState->acceptingState = state->acceptingState;

                        oldToNewStates[state] = newState;
                        oldToNewStates[cur]->transitions[pair.first].insert(newState);

                        nfa->states.insert(newState);

                        stateQueue.push(state);
                    }
                    else {
                        oldToNewStates[cur]->transitions[pair.first].insert(oldToNewStates[state]);
                    }
                }
            }
        }
    }

    return nfa;
}


NFA* nfa_new_single_char(const char ch) {
    NFA* nfa = new NFA;

    State* start = new State;
    State* end = new State;

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

    nfa->states.insert(nfa1->states.begin(), nfa1->states.end());
    nfa->states.insert(nfa2->states.begin(), nfa2->states.end());

    delete nfa1;
    delete nfa2;

    return nfa;
}

NFA *nfa_concat(NFA *nfa1, NFA *nfa2) {
    NFA* nfa = new NFA;

    nfa->start = nfa1->start;

    nfa->states.insert(nfa1->states.begin(), nfa1->states.end());
    nfa->states.insert(nfa2->states.begin(), nfa2->states.end());

    for (State* state : nfa1->states) {
        if (state->acceptingState) {
            state->acceptingState = false;
            state->transitions[0].insert(nfa2->start);
        }
    }

    delete nfa1;
    delete nfa2;

    return nfa;
}

NFA *nfa_zero_or_more(const NFA *nfa1) {
    NFA *nfa = new NFA;

    nfa->start = new State;

    nfa->start->acceptingState = true;
    nfa->start->transitions.emplace(0, set<State*>{nfa1->start});

    nfa->states.insert(nfa->start);
    nfa->states.insert(nfa1->states.begin(), nfa1->states.end());

    for (State* state : nfa1->states) {
        if (state->acceptingState) {
            state->transitions[0].insert(nfa->start);
        }
    }

    delete nfa1;

    return nfa;
}

NFA *nfa_one_or_more(NFA *nfa1) {
    NFA* nfaCopy = nfa_copy(nfa1);

    return nfa_concat(nfaCopy, nfa_zero_or_more(nfa1));
}

NFA *nfa_optional(NFA *nfa) {
    NFA* nfaRes = nfa_copy(nfa);

    for (State* state : nfaRes->states) {
        if (state->acceptingState) {
            nfaRes->start->transitions[0].insert(state);
        }
    }

    delete nfa;
    return nfaRes;
}

NFA *nfa_range(set<char> charSet, set<pair<char, char>> charRangeSet) {
    NFA* nfa = new NFA;
    nfa->start = new State;
    nfa->states.insert(nfa->start);

    for (char ch : charSet) {
        State* newState = new State;
        State* acceptingState = new State;

        newState->transitions[ch].insert(acceptingState);
        acceptingState->acceptingState = true;

        nfa->start->transitions[0].insert(newState);

        nfa->states.insert(newState);
        nfa->states.insert(acceptingState);
    }

    for (auto [start, end] : charRangeSet) {
        if (start > end) {
            throw std::invalid_argument("[end must be less than start]");
        }

        for (char ch = start; start <= ch && ch <= end; ch++) {
            State* newState = new State;
            State* acceptingState = new State;

            acceptingState->acceptingState = true;
            newState->transitions.emplace(ch, set<State*>{acceptingState});

            nfa->start->transitions[0].insert(newState);
            nfa->states.insert(newState);
            nfa->states.insert(acceptingState);
        }
    }

    return nfa;
}

NFA *nfa_any() {
    return nfa_range({}, {{1, 127}});
}

NFA *nfa_notInRange(set<char> notAccepted) {
    set<char> charSet;

    for (int i = 1; i < 128; i++) {
        if (!notAccepted.contains(i)) {
            charSet.insert(i);
        }
    }

    return nfa_range(charSet, {});
}

NFA *nfa_repeat(NFA* nfa, int left, int right) {
    NFA* nfaCopy = nfa_copy(nfa);
    NFA* res;

    if (left > 0) {
        res = nfa_copy(nfa);
    }
    else {
        res = nfa_optional(nfa_copy(nfa));
    }

    for (int i = 1; i < left; i++) {
        NFA* tmp = nfa_copy(nfaCopy);
        res = nfa_concat(res, tmp);
    }

    NFA* optionalCopy = nfa_optional(nfa_copy(nfaCopy));

    for (int i = left; i < right; i++) {
        NFA* tmp = nfa_copy(optionalCopy);
        res = nfa_concat(res, tmp);
    }

    delete nfa;
    delete optionalCopy;

    return res;
}


set<State*> epsilon_closure(State* s) {
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

set<State*> epsilon_closure(set<State*> stateSet) {
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

set<State*> move(set<State*> T, char ch) {
    set<State*> res;

    for (State* cur : T) {
        for (State* state : cur->transitions[ch]) {
            res.insert(state);
        }
    }

    return res;
}


