//
// Created by ian on 1/14/25.
//

#include "tests.h"
#include "../src/regex.h"
#include "utils.h"

int main() {
    INIT_TESTING()

    TEST("Regex (a)", []()->bool {
        NFA* regex = regex_parse("a");

        NFA* nfa = new NFA;

        nfa->states.insert(0);
        nfa->states.insert(1);

        nfa->acceptedStates.insert(1);

        nfa->alphabet.insert('a');

        nfa->transitionTable = new set<int>*[2];
        nfa->transitionTable[0] = new set<int>[128];
        nfa->transitionTable[1] = new set<int>[128];

        nfa->transitionTable[0]['a'].insert(1);

        nfa->startState = 0;

        bool res = ASSERT_EQUALS(*regex, *nfa);

        if (!res) cout << *regex << *nfa;

        delete regex;
        delete nfa;

        return res;
    })

    TEST("Regex (ab)", []()->bool {
        NFA* regex = regex_parse("ab");

        NFA* nfa = new NFA;

        nfa->startState = 0;

        nfa->alphabet = set<int>{'a'};
        nfa->alphabet.insert('b');

        nfa->states = set<int>{0};
        nfa->states.insert(1);
        nfa->states.insert(2);
        nfa->states.insert(3);

        nfa->acceptedStates.insert(3);

        nfa->transitionTable = new set<int>*[4];

        for (int i = 0; i < 4; i++) {
            nfa->transitionTable[i] = new set<int>[128];
        }

        nfa->transitionTable[0]['a'] = set<int>{1};
        nfa->transitionTable[1][0] = set<int>{2};
        nfa->transitionTable[2]['b'] = set<int>{3};

        bool res = ASSERT_EQUALS(*regex, *nfa);

        if (!res) cout << *regex << *nfa;

        delete regex;
        delete nfa;

        return res;
    })

    START_TESTS()
}
