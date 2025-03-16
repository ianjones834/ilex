//
// Created by ian on 1/12/25.
//

#include "DFA.h"

#include <map>
#include <set>
#include <stack>
#include <vector>

ostream& operator<<(ostream& os, const DFAState& dfaState) {
    /*int actionNum;
    int matchStartActionNum;
    int matchEndActionNum;
    int matchStartAndEndActionNum;

    bool acceptState;

    int curCharIndex;
    unordered_set<int> backTo;

    unordered_map<char, int> transitions;*/

    os << "\t\t .actionNum = " << dfaState.actionNum << "," << endl;
    os << "\t\t .matchStartActionNum = " << dfaState.matchStartActionNum << "," << endl;
    os << "\t\t .matchEndActionNum = " << dfaState.matchEndActionNum << "," << endl;
    os << "\t\t .matchStartAndEndActionNum = " << dfaState.matchStartAndEndActionNum << "," << endl;
    os << "\t\t .acceptState = " << (dfaState.acceptState ? "true" : "false") << "," << endl;
    os << "\t\t .curCharIndex = " << -1 << "," << endl;
    os << "\t\t .backTo = { ";

    for (auto state : dfaState.backTo) {
        os << state->stateNum << ", ";
    }

    os << "}," << endl;

    os << "\t\t .transitions = { ";
    for (auto pair : dfaState.transitions) {
        os << "{ " << (int) pair.first << ", " << pair.second->stateNum << " },";
    }

    os << "}" << endl;

    return os;
}

ostream& operator<<(ostream& os, const DFA& dfa) {
    vector<DFAState*> states;
    stack<DFAState*> stateStack;
    int stateNum = 0;
    stateStack.push(dfa.start);
    states.push_back(dfa.start);
    dfa.start->stateNum = stateNum++;

    while (!stateStack.empty()) {
        DFAState* curState = stateStack.top();
        stateStack.pop();

        for (auto pair : curState->transitions) {
            if (pair.second->stateNum == -1) {
                pair.second->stateNum = stateNum++;
                stateStack.push(pair.second);
                states.push_back(pair.second);
            }
        }
    }

    os << "#define STATE_NUM " << states.size() << endl << endl;

    os << "yy_DFAState yy_stateArr[STATE_NUM] = {\n";

    for (auto state : states) {
        os << "\t{\n" << *state << "\t},\n";
    }

    os << "};\n" << endl;

    return os;
}

// DFA to NFA Convert

DFA::DFA(NFA* nfa) {
    map<set<NFAState*>, DFAState*> stateMap;
    unordered_set<char> alphabet = getAlphabet(nfa);

    stack<set<NFAState*>> nextState;

    set<NFAState*> start = epsilon_closure(nfa->start);
    nextState.push(start);
    this->start = new DFAState(start);
    stateMap[start] = this->start;
    this->states.insert(this->start);
    while (!nextState.empty()) {
        set<NFAState*> cur = nextState.top();
        nextState.pop();

        for (char ch : alphabet) {
            set<NFAState*> next = epsilon_closure(move(cur, ch));

            if (!stateMap.contains(next)) {
                DFAState* newState = new DFAState(next);

                this->states.insert(newState);
                stateMap[next] = newState;
                nextState.push(next);


            }

            for (auto state : next) {
                if (!state->backTo.empty()) {
                    for (auto back : state->backTo) {
                        stateMap[next]->backTo.insert(stateMap[epsilon_closure(back)]);
                    }
                }
            }

            stateMap[cur]->transitions[ch] = stateMap[next];
        }
    }
}

unordered_set<char> getAlphabet(NFA* nfa) {
    unordered_set<char> alphabet;

    for (NFAState* state : nfa->states) {
        for (const auto& pair : state->transitions) {
            if (pair.first != 0) {
                alphabet.insert(pair.first);
            }
        }
    }

    return alphabet;
}