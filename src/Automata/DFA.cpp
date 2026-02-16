//
// Created by ian on 1/12/25.
//

#include "DFA.h"

#include <climits>
#include <map>
#include <set>
#include <unordered_map>
#include <stack>
#include <vector>
#include <sstream>

bool isRejectState(int stateNum, array<int, 128> transitions) {
    for (int state : transitions) {
        if (state != stateNum) {
            return false;
        }
    }

    return true;
}


ostream& operator<<(ostream& os, const vector<bool>& booleanVector) {
    if (booleanVector.empty()) {
        return os;
    }

    string res = booleanVector[0] ? "true" : "false" ;

    for (int i = 1; i < booleanVector.size(); i++) {
        res += ", " + string(booleanVector[i] ? "true" : "false");
    }

    os << res;

    return os;
}

ostream& operator<<(ostream& os, const vector<int>& intVector) {
    if (intVector.empty()) {
        return os;
    }

    string res = to_string(intVector[0]);

    for (int i = 1; i < intVector.size(); i++) {
        res += ", " + to_string(intVector[i]);
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
        res += ", " + to_string(intArray[i]);
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
    for (auto pair : dfa.nameToState) {
        os << "#define " << pair.first << " " << pair.second << endl;
    }

    os << "#define STATE_NUM " << dfa.stateNum << endl << endl;
    os << "bool acceptStates[STATE_NUM] = { " << dfa.acceptStates << " };" << endl;

    os << "int actionNum[STATE_NUM] = { " << dfa.actionNum << " };" << endl;
    os << "int matchStartActionNum[STATE_NUM] = { " << dfa.matchStartActionNum << " };" << endl;
    os << "int matchEndActionNum[STATE_NUM] = { " << dfa.matchEndActionNum << " };" << endl;
    os << "int matchStartAndEndActionNum[STATE_NUM] = { " << dfa.matchStartAndEndActionNum << " };" << endl << endl;

    os << "int curCharIndex[STATE_NUM] = { " << dfa.curCharIndex << " };" << endl << endl;

    os << "bool hasBackTo[STATE_NUM] = {";

    os << (dfa.backTo[0].empty() ? "false" : "true");

    for (int i = 1; i < dfa.stateNum; i++) {
        os << ", " << (dfa.backTo[i].empty() ? "false" : "true");
    }

    os << " };\n" << endl;

    os << "bool backTo[STATE_NUM][STATE_NUM] = {\n";

    for (int i = 0; i < dfa.stateNum; i++) {
        if (dfa.backTo[i].empty()) {
            os << "{}," << endl;
            continue;
        }

        os << "{ " << (dfa.backTo[i].contains(0) ? "true" : "false");

        for (int j = 1; j < dfa.stateNum; j++) {
            os << ", " << (dfa.backTo[i].contains(j) ? "true" : "false");
        }

        os << " },\n";
    }
    os << "\n};" << endl << endl;
    os << "int transitions[STATE_NUM][128] = {\n" << dfa.transitions << "\n};" << endl << endl;

    os << "bool endState[STATE_NUM] = { ";

    os << (isRejectState(0, dfa.transitions[0]) ? "true" : "false");

    for (int i = 1; i < dfa.stateNum; i++) {
        os << ", " << (isRejectState(i, dfa.transitions[i]) ? "true" : "false");
    }

    os << " };" << endl;



    return os;
}


// DFA to NFA Convert

DFA::DFA(NFA* nfa) {
    stateNum = 0;

    unordered_map<int, set<int>> epsilonClosureState;

    map<set<int>, int> stateMap;
    stack<set<int>> nextStateSet;

    set<int> start = epsilon_closure(nfa, 0);
    epsilonClosureState[0] = start;
    nextStateSet.push(start);

    stateMap[start] = stateNum++;

    acceptStates.push_back(false);

    actionNum.push_back(INT_MAX);
    matchEndActionNum.push_back(INT_MAX);
    matchStartActionNum.push_back(INT_MAX);
    matchStartAndEndActionNum.push_back(INT_MAX);

    curCharIndex.push_back(INT_MAX);

    backTo.push_back({});
    transitions.push_back({});

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
            if (!epsilonClosureState.contains(back)) {
                epsilonClosureState[back] = epsilon_closure(nfa, back);
            }

            backTo[0].insert(stateMap[epsilonClosureState[back]]);
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
                matchEndActionNum.push_back(INT_MAX);
                matchStartActionNum.push_back(INT_MAX);
                matchStartAndEndActionNum.push_back(INT_MAX);

                curCharIndex.push_back(INT_MAX);

                backTo.push_back({});
                transitions.push_back({});

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
                    if (!epsilonClosureState.contains(back)) {
                        epsilonClosureState[back] = epsilon_closure(nfa, back);
                    }

                    backTo[stateMap[next]].insert(stateMap[epsilonClosureState[back]]);
                }
            }

            transitions[stateMap[cur]][ch] = stateMap[next];
        }
    }

    for (int i = 0; i < stateNum; i++) {
        transitions[i][0] = i;
    }
}

DFA::DFA(int num) {
    stateNum = num;

    acceptStates = vector<bool>(stateNum);

    actionNum = vector<int>(stateNum);
    matchStartActionNum = vector<int>(stateNum);
    matchEndActionNum = vector<int>(stateNum);
    matchStartAndEndActionNum = vector<int>(stateNum);

    curCharIndex = vector<int>(stateNum);

    backTo = vector<unordered_set<int>>(stateNum);
    transitions = vector<array<int, 128>>(stateNum);
}

DFA *dfa_nUnion(vector<DFA *> dfaArr, unordered_map<int, string> dfaIndexToName) {
    int size = 0;

    for (DFA* dfa : dfaArr) {
        size += dfa->stateNum;
    }

    DFA* res = new DFA(size);

    int curIndex = 0;

    for (int dfaPointer = 0; dfaPointer < dfaArr.size(); dfaPointer++) {
        DFA* dfa = dfaArr[dfaPointer];

        for (int i = 0; i < dfa->stateNum; i++) {
            if (dfa->acceptStates[i]) {
                res->acceptStates[i + curIndex] = true;
            }

            for (int j = 0; j < 128; j++) {
                res->transitions[i + curIndex][j] = dfa->transitions[i][j] + curIndex;
            }

            res->actionNum[i + curIndex] = dfa->actionNum[i];
            res->matchStartActionNum[i + curIndex] = dfa->matchStartActionNum[i];
            res->matchEndActionNum[i + curIndex] = dfa->matchEndActionNum[i];
            res->matchStartAndEndActionNum[i + curIndex] = dfa->matchStartAndEndActionNum[i];

            for (int k : dfa->backTo[i]) {
                res->backTo[i + curIndex].insert(k + curIndex);
            }
        }

        res->nameToState[dfaIndexToName[dfaPointer]] = curIndex;

        curIndex += dfa->stateNum;
        delete dfa;
    }

    return res;
}
