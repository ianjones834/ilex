//
// Created by ian on 1/12/25.
//

#include "../src/nfa.h"
#include "utils.h"
#include "tests.h"


int main() {
    INIT_TESTING()

    TEST("NFA SINGLE CHAR (a)", []() -> bool {
        NFA* nfa = nfa_new_single_char('a');
        NFA* nfaVerify = new NFA;

        nfaVerify->startState = 0;
        nfaVerify->alphabet = set<int>{'a'};
        nfaVerify->acceptedStates = set<int>{1};
        nfaVerify->transitionTable = new set<int>*[2];
        nfaVerify->transitionTable[0] = new set<int>[128];
        nfaVerify->transitionTable[1] = new set<int>[128];
        nfaVerify->states = set<int>{0};
        nfaVerify->states.insert(1);

        nfaVerify->transitionTable[0]['a'] = set<int>{1};


        bool res = ASSERT_EQUALS(*nfa, *nfaVerify);

        if (!res) cout << *nfa << *nfaVerify;

        delete nfa;
        delete nfaVerify;

        return res;
    })

    TEST("NFA SINGLE CHAR (b)", []() -> bool {
        NFA* nfa = nfa_new_single_char('b');
        NFA* nfaVerify = new NFA;

        nfaVerify->startState = 0;
        nfaVerify->alphabet = set<int>{'b'};
        nfaVerify->acceptedStates = set<int>{1};
        nfaVerify->transitionTable = new set<int>*[2];
        nfaVerify->transitionTable[0] = new set<int>[128];
        nfaVerify->transitionTable[1] = new set<int>[128];
        nfaVerify->states = set<int>{0};
        nfaVerify->states.insert(1);

        nfaVerify->transitionTable[0]['b'] = set<int>{1};


        bool res = ASSERT_EQUALS(*nfa, *nfaVerify);
        if (!res) cout << *nfa << *nfaVerify;

        delete nfa;
        delete nfaVerify;

        return res;
    })

    TEST("NFA SINGLE CHAR ( )", []() -> bool {
        NFA* nfa = nfa_new_single_char(' ');
        NFA* nfaVerify = new NFA;

        nfaVerify->startState = 0;
        nfaVerify->alphabet = set<int>{' '};
        nfaVerify->acceptedStates = set<int>{1};
        nfaVerify->transitionTable = new set<int>*[2];
        nfaVerify->transitionTable[0] = new set<int>[128];
        nfaVerify->transitionTable[1] = new set<int>[128];
        nfaVerify->states = set<int>{0};
        nfaVerify->states.insert(1);

        nfaVerify->transitionTable[0][' '] = set<int>{1};


        bool res = ASSERT_EQUALS(*nfa, *nfaVerify);
        if (!res) cout << *nfa << *nfaVerify;

        delete nfa;
        delete nfaVerify;

        return res;
    })

    TEST("NFA SINGLE CHAR (/)", []() -> bool {
        NFA* nfa = nfa_new_single_char('/');
        NFA* nfaVerify = new NFA;

        nfaVerify->startState = 0;
        nfaVerify->alphabet = set<int>{'/'};
        nfaVerify->acceptedStates = set<int>{1};
        nfaVerify->transitionTable = new set<int>*[2];
        nfaVerify->transitionTable[0] = new set<int>[128];
        nfaVerify->transitionTable[1] = new set<int>[128];
        nfaVerify->states = set<int>{0};
        nfaVerify->states.insert(1);

        nfaVerify->transitionTable[0]['/'] = set<int>{1};


        bool res = ASSERT_EQUALS(*nfa, *nfaVerify);
        if (!res) cout << *nfa << *nfaVerify;

        delete nfa;
        delete nfaVerify;

        return res;
    })


    TEST("NFA Union (a|b)", []() -> bool {
        NFA* nfa1 = nfa_new_single_char('a');
        NFA* nfa2 = nfa_new_single_char('b');

        NFA* nfa = new NFA;

        for (int i = 0; i < 5; i++) {
            nfa->states.insert(i);
        }

        nfa->acceptedStates.insert(2);
        nfa->acceptedStates.insert(4);

        nfa->alphabet.insert('a');
        nfa->alphabet.insert('b');

        nfa->startState = 0;

        nfa->transitionTable = new set<int>*[5];

        for (int i = 0; i < 5; i++) {
            nfa->transitionTable[i] = new set<int>[128];
        }

        nfa->transitionTable[0][0].insert(1);
        nfa->transitionTable[0][0].insert(3);

        nfa->transitionTable[1]['a'].insert(2);
        nfa->transitionTable[3]['b'].insert(4);

        NFA* nfaUnion = nfa_union(nfa1, nfa2);

        const bool res = ASSERT_EQUALS(*nfaUnion, *nfa);
        if (!res) cout << *nfaUnion << *nfa;

        delete nfaUnion;
        delete nfa;
        delete nfa1;
        delete nfa2;

        return res;
    })

    TEST("NFA Union (a|a)", []() -> bool {
        NFA* nfa1 = nfa_new_single_char('a');
        NFA* nfa2 = nfa_new_single_char('a');

        NFA* nfa = new NFA;

        for (int i = 0; i < 5; i++) {
            nfa->states.insert(i);
        }

        nfa->acceptedStates.insert(2);
        nfa->acceptedStates.insert(4);

        nfa->alphabet.insert('a');
        nfa->alphabet.insert('a');

        nfa->startState = 0;

        nfa->transitionTable = new set<int>*[5];

        for (int i = 0; i < 5; i++) {
            nfa->transitionTable[i] = new set<int>[128];
        }

        nfa->transitionTable[0][0].insert(1);
        nfa->transitionTable[0][0].insert(3);

        nfa->transitionTable[1]['a'].insert(2);
        nfa->transitionTable[3]['a'].insert(4);

        NFA* nfaUnion = nfa_union(nfa1, nfa2);

        const bool res = ASSERT_EQUALS(*nfaUnion, *nfa);
        if (!res) cout << *nfaUnion << *nfa;

        delete nfaUnion;
        delete nfa;
        delete nfa1;
        delete nfa2;

        return res;
    })

    START_TESTS()
  }
