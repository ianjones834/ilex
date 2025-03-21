//
// Created by ian on 1/12/25.
//

#include "NFA.h"

#include <climits>
#include <queue>
#include <stack>
#include <unordered_set>

NFA::NFA(const int num) {
    stateNum = num;

    acceptStates = vector(num, false);

    actionNum = vector(num, INT_MAX);

    matchStart = vector(num, false);
    matchEnd = vector(num, false);
    matchStartAndEnd = vector(num, false);
    notMatchStartAndNotMatchEnd = vector(num, false);

    curCharIndex = vector(num, -1);

    backTo = vector<unordered_set<int>>(num);

    transitions = vector<array<unordered_set<int>, 128>>(num);
}

NFA* nfa_new_single_char(const char ch) {
    auto nfa = new NFA(2);

    nfa->transitions[0][ch].insert(1);
    nfa->acceptStates[1] = true;

    return nfa;
}

NFA* nfa_union(NFA* nfa1, NFA* nfa2, bool forRules) {
    NFA* nfa = new NFA(nfa1->stateNum + nfa2->stateNum + 1);

    nfa->transitions[0][0].insert({1, nfa1->stateNum + 1});

    for (int i = 0; i < nfa1->stateNum; i++) {
        if (nfa1->acceptStates[i]) {
            nfa->acceptStates[i + 1] = true;
        }

        for (int j = 0; j < 128; j++) {
            for (int k : nfa1->transitions[i][j]) {
                nfa->transitions[i + 1][j].insert(k + 1);
            }
        }

        if (forRules) {
            nfa->actionNum[i + 1] = nfa1->actionNum[i];

            nfa->matchStart[i + 1] = nfa1->matchStart[i];
            nfa->matchEnd[i + 1] = nfa1->matchEnd[i];
            nfa->matchStartAndEnd[i + 1] = nfa1->matchStartAndEnd[i];
            nfa->notMatchStartAndNotMatchEnd[i + 1] = nfa1->notMatchStartAndNotMatchEnd[i];

            for (int k : nfa1->backTo[i]) {
                nfa->backTo[i + 1].insert(k + 1);
            }
        }
    }

    for (int i = 0; i < nfa2->stateNum; i++) {
        if (nfa2->acceptStates[i]) {
            nfa->acceptStates[i + nfa1->stateNum + 1] = true;
        }

        for (int j = 0; j < 128; j++) {
            for (int k : nfa2->transitions[i][j]) {
                nfa->transitions[i + nfa1->stateNum + 1][j].insert(k + nfa1->stateNum + 1);
            }
        }

        if (forRules) {
            nfa->actionNum[i + nfa1->stateNum +1] = nfa2->actionNum[i];

            nfa->matchStart[i + nfa1->stateNum + 1] = nfa2->matchStart[i];
            nfa->matchEnd[i + nfa1->stateNum + 1] = nfa2->matchEnd[i];
            nfa->matchStartAndEnd[i + nfa1->stateNum + 1] = nfa2->matchStartAndEnd[i];
            nfa->notMatchStartAndNotMatchEnd[i + nfa1->stateNum + 1] = nfa2->notMatchStartAndNotMatchEnd[i];

            for (int k : nfa2->backTo[i]) {
                nfa->backTo[i + nfa1->stateNum + 1].insert(k + nfa1->stateNum + 1);
            }
        }
    }

    delete nfa1;
    delete nfa2;

    return nfa;
}

NFA* nfa_nUnion(vector<NFA*> nfaArr, bool forRules) {
    int size = 1;

    for (NFA* nfa : nfaArr) {
        size += nfa->stateNum;
    }

    NFA* res = new NFA(size);

    int curIndex = 1;

    for (NFA* nfa : nfaArr) {
        res->transitions[0][0].insert(curIndex);

        for (int i = 0; i < nfa->stateNum; i++) {
            if (nfa->acceptStates[i]) {
                res->acceptStates[i + curIndex] = true;
            }

            for (int j = 0; j < 128; j++) {
                for (int k : nfa->transitions[i][j]) {
                    res->transitions[i + curIndex][j].insert(k + curIndex);
                }
            }

            if (forRules) {
                res->actionNum[i + curIndex] = nfa->actionNum[i];

                res->matchStart[i + curIndex] = nfa->matchStart[i];
                res->matchEnd[i + curIndex] = nfa->matchEnd[i];
                res->matchStartAndEnd[i + curIndex] = nfa->matchStartAndEnd[i];
                res->notMatchStartAndNotMatchEnd[i + curIndex] = nfa->notMatchStartAndNotMatchEnd[i];

                for (int k : nfa->backTo[i]) {
                    res->backTo[i + curIndex].insert(k + curIndex);
                }
            }
        }

        curIndex += nfa->stateNum;
        delete nfa;
    }

    return res;
}

NFA *nfa_concat(NFA *nfa1, NFA *nfa2, bool backTo) {
    NFA* nfa = new NFA(nfa1->stateNum + nfa2->stateNum);

    for (int i = 0; i < nfa1->stateNum; i++) {
        for (int j = 0; j < 128; j++) {
                nfa->transitions[i][j] = nfa1->transitions[i][j];
        }

        if (nfa1->acceptStates[i]) {
            nfa->transitions[i][0].insert(nfa1->stateNum);
        }
    }

    for (int i = 0; i < nfa2->stateNum; i++) {
        for (int j = 0; j < 128; j++) {
            for (int k : nfa2->transitions[i][j]) {
                nfa->transitions[i + nfa1->stateNum][j].insert(k + nfa1->stateNum);
            }
        }

        if (backTo) {
            if (nfa2->acceptStates[i]) {
                for (int k = 0; k < nfa1->stateNum; k++) {
                    if (nfa1->acceptStates[k]) {
                        nfa->backTo[i + nfa1->stateNum].insert(k);
                    }
                }
            }
        }

        if (nfa2->acceptStates[i]) {
            nfa->acceptStates[i + nfa1->stateNum] = true;
        }
    }

    delete nfa1;
    delete nfa2;

    return nfa;
}

NFA *nfa_zero_or_more(const NFA *nfa1) {
    NFA *nfa = new NFA(nfa1->stateNum + 1);

    nfa->transitions[0][0].insert(1);
    nfa->acceptStates[0] = true;

    for (int i = 0; i < nfa1->stateNum; i++) {
        for (int j = 0; j < 128; j++) {
            for (int k : nfa1->transitions[i][j]) {
                nfa->transitions[i + 1][j].insert(k + 1);
            }
        }

        if (nfa1->acceptStates[i]) {
            nfa->acceptStates[i + 1] = true;
            nfa->transitions[i + 1][0].insert(0);
        }
    }

    delete nfa1;

    return nfa;
}

NFA *nfa_one_or_more(NFA *nfa1) {
    NFA* nfa = new NFA(*nfa1);

    return nfa_concat(nfa, nfa_zero_or_more(nfa1));
}

NFA *nfa_optional(NFA *nfa) {
    NFA* nfaRes = new NFA(*nfa);

    for (int i = 0; i < nfaRes->stateNum; i++) {
        if (nfaRes->acceptStates[i]) {
            nfaRes->transitions[0][0].insert(i);
        }
    }

    delete nfa;
    return nfaRes;
}

NFA *nfa_range(unordered_set<char> charSet, unordered_set<pair<char, char>*> charRangeSet) {
    NFA* nfa = new NFA(2);

    nfa->acceptStates[1] = true;

    if (!charSet.empty()) {
        for (auto chPointer = charSet.begin(); chPointer != charSet.end(); chPointer++) {
            nfa->transitions[0][*chPointer].insert(1);
        }

        for (auto pair : charRangeSet) {
            char start = pair->first, end = pair->second;

            for (char ch = start; start <= ch && ch <= end; ch++) {
                nfa->transitions[0][ch].insert(1);
            }

            delete pair;
        }
    }
    else if (!charRangeSet.empty()) {
        for (auto pair : charRangeSet) {
            char start = pair->first, end = pair->second;

            for (char ch = start; start <= ch && ch <= end; ch++) {
                nfa->transitions[0][ch].insert(1);
            }

            delete pair;
        }
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

    return nfa_range(charSet, {});
}

NFA *nfa_repeat(NFA* nfa, int left, int right) {
    NFA* nfaCopy = new NFA(*nfa);
    NFA* res;

    if (left > 0) {
        res = new NFA(*nfa);
    }
    else {
        res = nfa_optional(new NFA(*nfa));
    }

    for (int i = 1; i < left; i++) {
        NFA* tmp = new NFA(*nfaCopy);
        res = nfa_concat(res, tmp);
    }

    NFA* optionalCopy = nfa_optional(new NFA(*nfaCopy));

    for (int i = left; i < right; i++) {
        NFA* tmp = new NFA(*optionalCopy);
        res = nfa_concat(res, tmp);
    }

    delete nfa;
    delete optionalCopy;
    delete nfaCopy;

    return res;
}


set<int> epsilon_closure(NFA* nfa, int s) {
    set<int> res = {s};
    stack<int> toSearch;

    toSearch.push(s);

    while (!toSearch.empty()) {
        int cur = toSearch.top();
        toSearch.pop();

        for (int state : nfa->transitions[cur][0]) {
            if (!res.contains(state)) {
                res.insert(state);
                toSearch.push(state);
            }
        }
    }

    return res;
}

set<int> epsilon_closure(NFA* nfa, set<int> stateSet) {
    set<int> res = stateSet;
    stack<int> toSearch;

    for (int i : stateSet) {
        toSearch.push(i);
    }

    while (!toSearch.empty()) {
        int cur = toSearch.top();
        toSearch.pop();

        for (int state : nfa->transitions[cur][0]) {
            if (!res.contains(state)) {
                res.insert(state);
                toSearch.push(state);
            }
        }
    }

    return res;
}

set<int> move(NFA* nfa, set<int> stateSet, char ch) {
    set<int> res;

    for (int cur : stateSet) {
        for (int state : nfa->transitions[cur][ch]) {
            res.insert(state);
        }
    }

    return res;
}


