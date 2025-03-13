//
// Created by ian on 1/12/25.
//

#include "NFA.h"
#include <queue>
#include <stack>
#include <stdexcept>
#include <unordered_set>

NFA *nfa_copy(NFA *nfa1) {
    NFA* nfa = new NFA;

    unordered_map<NFAState*, NFAState*> oldToNewStates;
    queue<NFAState*> stateQueue;

    nfa->start = new NFAState;
    nfa->start->acceptState = nfa1->start->acceptState;
    nfa->states.insert(nfa->start);

    stateQueue.push(nfa1->start);

    oldToNewStates.emplace(nfa1->start, nfa->start);

    while (!stateQueue.empty()) {
        int curLevelSize = stateQueue.size();

        while (curLevelSize--) {
            NFAState* cur = stateQueue.front();
            stateQueue.pop();

            for (auto pair : cur->transitions) {
                for (NFAState* state : pair.second) {
                    if (!oldToNewStates.contains(state)) {
                        NFAState* newState = new NFAState;
                        newState->acceptState = state->acceptState;

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

    NFAState* start = new NFAState;
    NFAState* end = new NFAState;

    end->acceptState = true;
    start->transitions[ch] = unordered_set<NFAState*>{end};

    nfa->start = start;
    nfa->start->acceptState = false;

    nfa->states.insert(start);
    nfa->states.insert(end);

    return nfa;
}

NFA* nfa_union(NFA* nfa1, NFA* nfa2) {
    NFA* nfa = new NFA;

    nfa->start = new NFAState;
    nfa->start->acceptState = false;

    nfa->states.insert(nfa->start);

    nfa->start->transitions[0] = unordered_set<NFAState*>{nfa1->start, nfa2->start};

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

    for (NFAState* state : nfa1->states) {
        if (state->acceptState) {
            state->acceptState = false;
            state->transitions[0].insert(nfa2->start);
        }
    }

    delete nfa1;
    delete nfa2;

    return nfa;
}

NFA *nfa_zero_or_more(const NFA *nfa1) {
    NFA *nfa = new NFA;

    nfa->start = new NFAState;

    nfa->start->acceptState = true;
    nfa->start->transitions[0] = unordered_set<NFAState*>{nfa1->start};

    nfa->states.insert(nfa->start);
    nfa->states.insert(nfa1->states.begin(), nfa1->states.end());

    for (NFAState* state : nfa1->states) {
        if (state->acceptState) {
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

    for (NFAState* state : nfaRes->states) {
        if (state->acceptState) {
            nfaRes->start->transitions[0].insert(state);
        }
    }

    for (auto state : nfa->states) delete state;

    delete nfa;
    return nfaRes;
}

NFA *nfa_range(unordered_set<char> charSet, unordered_set<pair<char, char>*> charRangeSet) {
    NFA* nfa = new NFA;
    nfa->start = new NFAState;
    nfa->states.insert(nfa->start);

    for (char ch : charSet) {
        NFAState* newState = new NFAState;
        NFAState* acceptingState = new NFAState;

        newState->transitions[ch].insert(acceptingState);
        acceptingState->acceptState = true;

        nfa->start->transitions[0].insert(newState);

        nfa->states.insert(newState);
        nfa->states.insert(acceptingState);
    }

    for (auto pair : charRangeSet) {
        char start = pair->first, end = pair->second;

        if (start > end) {
            throw std::invalid_argument("[end must be less than start]");
        }

        for (char ch = start; start <= ch && ch <= end; ch++) {
            NFAState* newState = new NFAState;
            NFAState* acceptingState = new NFAState;

            acceptingState->acceptState = true;
            newState->transitions[ch] = unordered_set<NFAState*>{acceptingState};

            nfa->start->transitions[0].insert(newState);
            nfa->states.insert(newState);
            nfa->states.insert(acceptingState);
        }

        delete pair;
    }

    return nfa;
}

NFA *nfa_any() {
    return nfa_range({}, {new pair<char, char>{1, 127}});
}

NFA *nfa_notInRange(unordered_set<char> notAccepted) {
    unordered_set<char> charSet;

    for (int i = 1; i < 128; i++) {
        if (!notAccepted.contains(i)) {
            charSet.insert(i);
        }
    }

    return nfa_range(charSet, unordered_set<pair<char, char>*>{});
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

    for (auto state : nfa->states) {
        delete state;
    }

    for (auto state : optionalCopy->states) {
        delete state;
    }

    for (auto state : nfaCopy->states) {
        delete state;
    }

    delete nfa;
    delete optionalCopy;
    delete nfaCopy;

    return res;
}


set<NFAState*> epsilon_closure(NFAState* s) {
    set<NFAState*> res;
    stack<NFAState*> toSearch;

    res.insert(s);
    toSearch.push(s);

    while (!toSearch.empty()) {
        NFAState* cur = toSearch.top();
        toSearch.pop();

        for (NFAState* state : cur->transitions[0]) {
            if (!res.contains(state)) {
                res.insert(state);
                toSearch.push(state);
            }
        }
    }

    return res;
}

set<NFAState*> epsilon_closure(set<NFAState*> stateSet) {
    set<NFAState*> res;
    stack<NFAState*> toSearch;

    for (NFAState* i : stateSet) {
        toSearch.push(i);
        res.insert(i);
    }

    while (!toSearch.empty()) {
        NFAState* cur = toSearch.top();
        toSearch.pop();

        for (NFAState* state : cur->transitions[0]) {
            if (!res.contains(state)) {
                res.insert(state);
                toSearch.push(state);
            }
        }
    }

    return res;
}

set<NFAState*> move(set<NFAState*> T, char ch) {
    set<NFAState*> res;

    for (NFAState* cur : T) {
        for (NFAState* state : cur->transitions[ch]) {
            res.insert(state);
        }
    }

    return res;
}


