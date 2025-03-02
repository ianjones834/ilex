//
// Created by ian on 1/12/25.
//

#include "dfa.h"

#include <limits.h>

int STATE_NUM = 0;

ostream& operator<<(ostream& out, const unordered_set<int> & stateSet) {
    out << "{";

    for (int i = 0; i < STATE_NUM; i++) {
        out << i << ", ";
    }

    out << "}";

    return out;
};

ostream& operator<<(ostream& out, const unordered_map<int, unordered_map<char, int>> &transitions) {
    out << "{" << endl;

    for (int i = 0; i < transitions.size(); i++) {
        out << "\t\t{";
        
        for (auto charMap : transitions.at(i)) {
            out << "{" << (int) charMap.first << ", " << charMap.second << "}, ";
        }

        out << "}," << endl;
    }

    out << "\t}";

    return out;
}

ostream& operator<<(ostream& out, const unordered_map<int, unordered_set<int>>& stateToRuleMap) {
    out << "{ ";

    for (int i = 0; i < STATE_NUM; i++) {
        if (stateToRuleMap.contains(i)) {
            int min = INT_MAX;

            for (int j : stateToRuleMap.at(i)) {
                min = j < min ? j : min;
            }

            out << "&action" << min << ", ";
        }
        else {
            out << "nullptr, ";
        }
    }

    out << "}";

    return out;
}

bool dfa_serialize(DFA* dfa, ostream& out) {
    try {
        STATE_NUM = dfa->states.size();
        out << "\tconst int START = " << dfa->start << ";" << endl;
        out << "\tconst int STATE_NUM = " << dfa->states.size() << ";" << endl;
        out << "\tconst std::unordered_map<char,int> transitions[STATE_NUM] = " << dfa->transitions << ";" << endl;

        out << "\tconst bool acceptedStates[STATE_NUM] = {";

        for (int i = 0; i < STATE_NUM; i++) {
            out << dfa->acceptedStates.contains(i) << ",";
        }

        out << "};" << endl;

        out << "\tconst bool matchStart[STATE_NUM] = {";

        for (int i = 0; i < STATE_NUM; i++) {
            out << dfa->matchStart.contains(i) << ",";
        }

        out << "};" << endl;

        out << "\tconst bool matchEnd[STATE_NUM] = {";

        for (int i = 0; i < STATE_NUM; i++) {
            out << dfa->matchEnd.contains(i) << ",";
        }

        out << "};" << endl;

        out << "\tint(*stateToActionMap[STATE_NUM])() = " << dfa->stateToRuleMap << ";" << endl;
        return true;
    }
    catch (exception ex) {
        return false;
    }
}