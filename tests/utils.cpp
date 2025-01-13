//
// Created by ian on 1/12/25.
//

#include "utils.h"

ostream& operator<<(ostream& os, set<int>& set) {
    os << "{";

    for (int i : set) {
        os << " " << i;
    }

    os << " }";

    return os;
}

ostream& operator<<(ostream& os, NFA& nfa) {
    os << "NFA: " << endl;
    os << "\tStart state: " << nfa.startState << endl;
    os << "\tAlphabet: " << nfa.alphabet << endl;
    os << "\tStates: " << nfa.states << endl;

    os << "\t Transition Table: " << endl;

    for (int i : nfa.states) {
        os << "\t\t(" << i << ", " << "epsilon): " << nfa.transitionTable[i][0] << endl;

        for (char j : nfa.alphabet) {
            os << "\t\t(" << i << ", " << j << "): " << nfa.transitionTable[i][j] << endl;
        }
    }

    os << "\tAccepted States: " << nfa.acceptedStates << endl << endl;

    return os;
}

ostream& operator<<(ostream& os, DFA& dfa) {
    os << "DFA: " << endl;
    os << "\tStart state: " << dfa.startState << endl;
    os << "\tAlphabet: " << dfa.alphabet << endl;
    os << "\tStates: " << dfa.states << endl;

    os << "\t Transition Table: " << endl;

    for (int i : dfa.states) {
        os << "\t\t(" << i << ", " << "epsilon): " << dfa.transitionTable[i][0] << endl;

        for (char j : dfa.alphabet) {
            os << "\t\t(" << i << ", " << j << "): " << dfa.transitionTable[i][j] << endl;
        }
    }

    os << "\tAccepted States: " << dfa.acceptedStates << endl << endl;

    return os;
}

bool operator==(NFA& nfa1, NFA& nfa2) {
    bool states = nfa1.states == nfa2.states;
    bool acceptedStates = nfa1.acceptedStates == nfa2.acceptedStates;
    bool alphabet = nfa1.alphabet == nfa2.alphabet;
    bool startState = nfa1.startState == nfa2.startState;
    bool transitionTable = true;
    bool actions = true;

    for (int i : nfa1.states) {
        for (int j = 0; j < 128; j++) {
            if (nfa1.transitionTable[i][j] != nfa2.transitionTable[i][j]) {
                transitionTable = false;
                break;
            }
        }

        if (nfa1.actions.contains(i) != nfa2.actions.contains(i) && nfa1.actions.at(i) != nfa2.actions.at(i)) {
            actions = false;
        }

        if (!transitionTable && !actions) {
            break;
        }
    }

    if (!states) {
        cout << "STATES: FALSE" << endl;
    }

    if (!acceptedStates) {
        cout << "ACCEPTED STATES: FALSE" << endl;
    }

    if (!alphabet) {
        cout << "ALPHABET: FALSE" << endl;
    }

    if (!startState) {
        cout << "START STATE: FALSE" << endl;
    }

    if (!transitionTable) {
        cout << "TRANSITION TABLE: FALSE" << endl;
    }

    if (!actions) {
        cout << "ACTIONS: FALSE" << endl;
    }

    return states && acceptedStates && alphabet && startState && transitionTable && actions;
}

bool operator==(DFA& dfa1, DFA& dfa2) {
    bool states = dfa1.states == dfa2.states;
    bool acceptedStates = dfa1.acceptedStates == dfa2.acceptedStates;
    bool alphabet = dfa1.alphabet == dfa2.alphabet;
    bool startState = dfa1.startState == dfa2.startState;
    bool transitionTable = true;

    for (int i : dfa1.states) {
        for (int j = 0; j < 128; j++) {
            if (dfa1.transitionTable[i][j] != dfa2.transitionTable[i][j]) {
                transitionTable = false;
                break;
            }
        }

        if (!transitionTable) {
            break;
        }
    }

    return states && acceptedStates && alphabet && startState && transitionTable;
}
