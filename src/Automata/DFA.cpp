//
// Created by ian on 1/12/25.
//

#include "DFA.h"

#include <climits>
#include <map>
#include <set>
#include <stack>
#include <vector>

ostream& operator<<(ostream& os, const vector<bool>& booleanVector) {
    if (booleanVector.empty()) {
        return os;
    }

    string res = booleanVector[0] ? "true" : "false" ;

    for (bool boolean : booleanVector) {
        res += ", " + string(boolean ? "true" : "false");
    }

    os << res;

    return os;
}

ostream& operator<<(ostream& os, const vector<int>& intVector) {
    if (intVector.empty()) {
        return os;
    }

    string res = to_string(intVector[0]);

    for (int i : intVector) {
        res += ", " + to_string(i);
    }

    os << res;

    return os;
}

ostream& operator<<(ostream& os, const unordered_set<int>& intSet) {
    if (intSet.empty()) {
        return os;
    }

    string res = to_string(*intSet.begin());

    for (auto intPointer = ++intSet.begin(); intPointer != intSet.end(); ++intPointer) {
        res += ", " + to_string(*intPointer);
    }

    os << res;

    return os;
}

ostream& operator<<(ostream& os, const array<int, 128> intArray) {
    if (intArray.empty()) {
        return os;
    }

    string res = to_string(intArray[0]);

    for (int i = 1; i < intArray.size(); i++) {
        res += ", " + to_string(i);
    }

    os << res;
    return os;
}

ostream& operator<<(ostream& os, const vector<array<int, 128>> arrayVector) {
    if (arrayVector.empty()) {
        return os;
    }

    for (auto arrayElement : arrayVector) {
        os << "\t{ " << arrayElement << " }," << endl;
    }

    return os;
}

ostream& operator<<(ostream& os, const vector<unordered_set<int>> setVector) {
    if (setVector.empty()) {
        return os;
    }

    for (auto set : setVector) {
        os << "\t{ " << set << " }," << endl;
    }

    return os;
}



ostream& operator<<(ostream& os, const DFA& dfa) {
    os << "#define STATE_NUM " << dfa.stateNum << endl << endl;
    os << "bool acceptStates[STATE_NUM] = { " << dfa.acceptStates << " };" << endl;

    os << "int actionNum[STATE_NUM] = { " << dfa.actionNum << " };" << endl;
    os << "int matchStartActionNum[STATE_NUM] = { " << dfa.matchStartActionNum << " };" << endl;
    os << "int matchEndActionNum[STATE_NUM] = { " << dfa.matchEndActionNum << " };" << endl;
    os << "int matchStartAndEndActionNum[STATE_NUM] = { " << dfa.matchStartAndEndActionNum << " };" << endl << endl;

    os << "int curCharIndex[STATE_NUM] = { " << dfa.curCharIndex << " };" << endl << endl;

    os << "int* backTo[STATE_NUM] = {\n" << dfa.backTo << "\n};" << endl << endl;
    os << "int transitions[STATE_NUM][128] = {\n" << dfa.transitions << "\n};" << endl << endl;
}

// DFA to NFA Convert

DFA::DFA(NFA* nfa) {
    stateNum = 0;

    map<set<int>, int> stateMap;
    stack<set<int>> nextStateSet;

    set<int> start = epsilon_closure(nfa, 0);
    nextStateSet.push(start);

    stateMap[start] = stateNum++;

    acceptStates.push_back(false);

    actionNum.push_back(INT_MAX);
    matchStartActionNum.push_back(INT_MAX);
    matchEndActionNum.push_back(INT_MAX);
    matchStartAndEndActionNum.push_back(INT_MAX);

    curCharIndex.push_back(-1);

    backTo.push_back({});
    transitions.push_back({});
    transitions[stateNum - 1].fill(-1);

    for (int state : start) {
        if (nfa->acceptStates[state]) {
            acceptStates[0] = true;

            matchStartActionNum[0] = min(matchStartActionNum[0], nfa->matchStart[state] ? nfa->actionNum[state] : INT_MAX);
            matchEndActionNum[0] = min(matchEndActionNum[0], nfa->matchEnd[state] ? nfa->actionNum[state] : INT_MAX);
            matchStartAndEndActionNum[0] = min(matchStartAndEndActionNum[0], nfa->matchStartAndEnd[state] ? nfa->actionNum[state] : INT_MAX);

            actionNum[0] = min(actionNum[0], nfa->notMatchStartAndNotMatchEnd[state] ? nfa->actionNum[state] : INT_MAX);
        }
    }

    for (int state : start) {
        for (int back : nfa->backTo[state]) {
            backTo[0].insert(stateMap[epsilon_closure(nfa, back)]);
        }
    }

    while (!nextStateSet.empty()) {
        set<int> cur = nextStateSet.top();
        nextStateSet.pop();

        for (int ch = 1; ch < 128; ch++) {
            set<int> next = epsilon_closure(nfa, (move(nfa, cur, ch)));

            if (!stateMap.contains(next)) {
                stateMap[next] = stateNum++;
                nextStateSet.push(next);

                acceptStates.push_back(false);

                actionNum.push_back(INT_MAX);
                matchStartActionNum.push_back(INT_MAX);
                matchEndActionNum.push_back(INT_MAX);
                matchStartAndEndActionNum.push_back(INT_MIN);

                curCharIndex.push_back(-1);

                backTo.push_back({});
                transitions.push_back({});
                transitions[stateNum - 1].fill(-1);
            }

            int curStateNum = stateMap[next];

            for (int state : next) {
                if (nfa->acceptStates[state]) {
                    acceptStates[curStateNum] = true;

                    matchStartActionNum[curStateNum] = min(matchStartActionNum[curStateNum], nfa->matchStart[state] ? nfa->actionNum[state] : INT_MAX);
                    matchEndActionNum[curStateNum] = min(matchEndActionNum[curStateNum], nfa->matchEnd[state] ? nfa->actionNum[state] : INT_MAX);
                    matchStartAndEndActionNum[curStateNum] = min(matchStartAndEndActionNum[curStateNum], nfa->matchStartAndEnd[state] ? nfa->actionNum[state] : INT_MAX);

                    actionNum[curStateNum] = min(actionNum[curStateNum], nfa->notMatchStartAndNotMatchEnd[state] ? nfa->actionNum[state] : INT_MAX);
                }
            }

            for (int state : next) {
                for (int back : nfa->backTo[state]) {
                    backTo[stateMap[next]].insert(stateMap[epsilon_closure(nfa, back)]);
                }
            }

            transitions[stateMap[cur]][ch] = stateMap[next];
        }
    }
}