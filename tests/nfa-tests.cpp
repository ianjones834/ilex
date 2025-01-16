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

        return res;
    })

    TEST("NFA Concat (aa)", []()-> bool {
        NFA* nfa1 = nfa_new_single_char('a');
        NFA* nfa2 = nfa_new_single_char('a');

        NFA* nfaConcatRes = nfa_concat(nfa1, nfa2);

        NFA* nfa = new NFA;

        nfa->startState = 0;

        nfa->alphabet = set<int>{'a'};
        nfa->alphabet.insert('a');

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
        nfa->transitionTable[2]['a'] = set<int>{3};

        bool res = ASSERT_EQUALS(*nfaConcatRes, *nfa);

        if (!res) cout << *nfaConcatRes << *nfa;

        delete nfaConcatRes;
        delete nfa;

        return res;
    })

    TEST("NFA Concat (ab)", []()-> bool {
    NFA* nfa1 = nfa_new_single_char('a');
    NFA* nfa2 = nfa_new_single_char('b');

    NFA* nfaConcatRes = nfa_concat(nfa1, nfa2);

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

    bool res = ASSERT_EQUALS(*nfaConcatRes, *nfa);

    if (!res) cout << *nfaConcatRes << *nfa;

    delete nfaConcatRes;
    delete nfa;

    return res;
})

    TEST("NFA Zero Or More (a*)", []() -> bool {
        NFA* nfa1 = nfa_new_single_char('a');
        NFA* nfaZeroOrMore = nfa_zero_or_more(nfa1);

        NFA* nfa = new NFA;

        nfa->startState = 0;

        nfa->acceptedStates =  set<int>{0};
        nfa->acceptedStates.insert(2);

        nfa->alphabet = set<int>{'a'};

        nfa->states = set<int>{0};
        nfa->states.insert(1);
        nfa->states.insert(2);

        nfa->transitionTable = new set<int>*[3];

        nfa->transitionTable[0] = new set<int>[128];
        nfa->transitionTable[1] = new set<int>[128];
        nfa->transitionTable[2] = new set<int>[128];

        nfa->transitionTable[0][0].insert(1);
        nfa->transitionTable[1]['a'].insert(2);
        nfa->transitionTable[2][0].insert(0);

        bool res = ASSERT_EQUALS(*nfaZeroOrMore, *nfa);
        if (!res) cout << *nfaZeroOrMore << *nfa;

        delete nfaZeroOrMore;
        delete nfa;

        return res;
    })

    TEST("NFA Zero Or More (b*)", []() -> bool {
    NFA* nfa1 = nfa_new_single_char('b');
    NFA* nfaZeroOrMore = nfa_zero_or_more(nfa1);

    NFA* nfa = new NFA;

    nfa->startState = 0;

    nfa->acceptedStates =  set<int>{0};
    nfa->acceptedStates.insert(2);

    nfa->alphabet = set<int>{'b'};

    nfa->states = set<int>{0};
    nfa->states.insert(1);
    nfa->states.insert(2);

    nfa->transitionTable = new set<int>*[3];

    nfa->transitionTable[0] = new set<int>[128];
    nfa->transitionTable[1] = new set<int>[128];
    nfa->transitionTable[2] = new set<int>[128];

    nfa->transitionTable[0][0].insert(1);
    nfa->transitionTable[1]['b'].insert(2);
    nfa->transitionTable[2][0].insert(0);

    bool res = ASSERT_EQUALS(*nfaZeroOrMore, *nfa);
    if (!res) cout << *nfaZeroOrMore << *nfa;

    delete nfaZeroOrMore;
    delete nfa;

    return res;
})

    TEST("NFA One or More (a+)", []() -> bool {
        NFA* nfaOneOrMore = nfa_one_or_more(nfa_new_single_char('a'));
        NFA* nfa = new NFA;

        nfa->startState = 0;

        nfa->alphabet = set<int>{'a'};

        nfa->states.insert(0);
        nfa->states.insert(1);
        nfa->states.insert(2);
        nfa->states.insert(3);
        nfa->states.insert(4);

        nfa->acceptedStates.insert(2);
        nfa->acceptedStates.insert(4);

        nfa->transitionTable = new set<int>*[5];

        for (int i = 0; i < 5; i++) {
            nfa->transitionTable[i] = new set<int>[128];
        }

        nfa->transitionTable[0]['a'] = set<int>{1};
        nfa->transitionTable[1][0] = set<int>{2};
        nfa->transitionTable[2][0] = set<int>{3};
        nfa->transitionTable[3]['a'] = set<int>{4};
        nfa->transitionTable[4][0] = set<int>{2};

        bool res = ASSERT_EQUALS(*nfaOneOrMore, *nfa);

        if (!res) cout << *nfaOneOrMore << *nfa;

        delete nfaOneOrMore;
        delete nfa;

        return res;
    })

    TEST("NFA Optional (a?)", []() -> bool {
        NFA *nfaOptional = nfa_optional(nfa_new_single_char('a'));

        NFA* nfa = new NFA;

        nfa->startState = 0;

        nfa->states.insert(0);
        nfa->states.insert(1);

        nfa->alphabet.insert('a');

        nfa->transitionTable = new set<int>*[2];
        nfa->transitionTable[0] = new set<int>[128];
        nfa->transitionTable[0]['a'].insert(1);
        nfa->transitionTable[1] = new set<int>[128];

        nfa->acceptedStates.insert(0);
        nfa->acceptedStates.insert(1);

        bool res = ASSERT_EQUALS(*nfaOptional, *nfa);

        if (!res) cout << *nfaOptional << *nfa;

        delete nfaOptional;
        delete nfa;

        return res;
    })

    TEST("Epsilon Closure of a state (a*, 0)", []() -> bool {
        NFA* nfa = nfa_zero_or_more(nfa_new_single_char('a'));

        set<int> epsilonClosureSet = epsilon_closure(nfa, 0);
        set<int> expected;
        expected.insert(0);
        expected.insert(1);

        bool res = ASSERT_EQUALS(epsilonClosureSet, expected);

        if (!res) cout << epsilonClosureSet << expected;

        delete nfa;

        return res;
    })

    TEST("Epsilon Closure of a state (a*, 2)", []() -> bool {
        NFA* nfa = nfa_zero_or_more(nfa_new_single_char('a'));

        set<int> epsilonClosureSet = epsilon_closure(nfa, 2);
        set<int> expected;
        expected.insert(0);
        expected.insert(1);
        expected.insert(2);

        bool res = ASSERT_EQUALS(epsilonClosureSet, expected);

        if (!res) cout << epsilonClosureSet << expected;

        delete nfa;

        return res;
    })

    TEST("Epsilon Closure of a state ((a|b)*, 0)", []() -> bool {
        NFA* nfa = nfa_zero_or_more(nfa_union(nfa_new_single_char('a'), nfa_new_single_char('b')));

        set<int> epsilonClosureSet = epsilon_closure(nfa, 0);
        set<int> expected;
        expected.insert(0);
        expected.insert(1);
        expected.insert(2);
        expected.insert(4);

        bool res = ASSERT_EQUALS(epsilonClosureSet, expected);

        if (!res) cout << epsilonClosureSet << expected;

        delete nfa;

        return res;
    })

    TEST("Epsilon Closure of a state (a(a|b)*, 1)", []() -> bool {
        NFA* nfa = nfa_concat(nfa_new_single_char('a'), nfa_zero_or_more(nfa_union(nfa_new_single_char('a'), nfa_new_single_char('b'))));

        set<int> epsilonClosureSet = epsilon_closure(nfa, 1);
        set<int> expected;
        expected.insert(1);
        expected.insert(2);
        expected.insert(3);
        expected.insert(4);
        expected.insert(6);

        bool res = ASSERT_EQUALS(epsilonClosureSet, expected);

        if (!res) cout << epsilonClosureSet << expected;

        delete nfa;

        return res;
    })

    TEST("Epsilon Closure of a set (a*, {0,2})", []() -> bool {
        NFA* nfa = nfa_zero_or_more(nfa_new_single_char('a'));

        set<int> T;
        T.insert(0);
        T.insert(2);

        set<int> epsilonClosure = epsilon_closure(nfa, T);
        set<int> expected;
        expected.insert(0);
        expected.insert(2);
        expected.insert(1);

        bool res = ASSERT_EQUALS(epsilonClosure, expected);

        if (!res) cout << epsilonClosure << endl << expected << endl << endl;

        return res;
    })

    TEST("Epsilon Closure of a set (a(a|b)*, {5,7})", []() -> bool {
        NFA* nfa = nfa_concat(nfa_new_single_char('a'), nfa_zero_or_more(nfa_union(nfa_new_single_char('a'), nfa_new_single_char('b'))));

        set<int> t;
        t.insert(5);
        t.insert(7);

        set<int> epsilonClosureSet = epsilon_closure(nfa, t);
        set<int> expected;
        expected.insert(2);
        expected.insert(3);
        expected.insert(4);
        expected.insert(6);
        expected.insert(5);
        expected.insert(7);

        bool res = ASSERT_EQUALS(epsilonClosureSet, expected);

        if (!res) cout << epsilonClosureSet << expected;

        delete nfa;

        return res;
    })

    TEST("Move ((a|a), {1,3}, a", []() -> bool {
        NFA* nfa = nfa_union(nfa_new_single_char('a'), nfa_new_single_char('a'));

        set<int> t;
        t.insert(1);
        t.insert(3);

        set<int> moveSet = move(nfa, t, 'a');
        set<int> expected;
        expected.insert(2);
        expected.insert(4);

        bool res = ASSERT_EQUALS(moveSet, expected);

        if (!res) cout << moveSet << expected;

        delete nfa;

        return res;

    })

    START_TESTS()
  }
