//
// Created by ian on 1/14/25.
//

#include "tests.h"
#include "../src/regex.h"
#include "utils.h"

int main() {
    INIT_TESTING()

    TEST("Regex 'a'", []() -> bool {
        NFA* nfa = regex_parse("a");
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

    TEST("Regex 'b'", []() -> bool {
        NFA* nfa = regex_parse("b");
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

    TEST("Regex ' '", []() -> bool {
        NFA* nfa = regex_parse(" ");
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

    TEST("Regex '/'", []() -> bool {
        NFA* nfa = regex_parse("/");
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


    TEST("Regex 'a|b'", []() -> bool {
        NFA* nfaUnion = regex_parse("a|b");

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

        const bool res = ASSERT_EQUALS(*nfaUnion, *nfa);
        if (!res) cout << *nfaUnion << *nfa;

        delete nfaUnion;
        delete nfa;

        return res;
    })


    TEST("Regex 'a|a'", []() -> bool {
         NFA* nfaUnion = regex_parse("a|a");

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

         const bool res = ASSERT_EQUALS(*nfaUnion, *nfa);
         if (!res) cout << *nfaUnion << *nfa;

         delete nfaUnion;
         delete nfa;

         return res;
         })
    TEST("Regex concat 'aa'", []()-> bool {
         NFA* nfaConcatRes = regex_parse("aa");

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

    TEST("Regex concat 'ab'", []()-> bool {
    NFA* nfaConcatRes = regex_parse("ab");

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

    TEST("Regex zero or more 'a*'", []() -> bool {
        NFA* nfaZeroOrMore = regex_parse("a*");

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

    TEST("Regex zero or more 'b*'", []() -> bool {
    NFA* nfaZeroOrMore = regex_parse("b*");

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

    TEST("Regex one or more 'a+'", []() -> bool {
        NFA* nfaOneOrMore = regex_parse("a+");
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

    TEST("Regex optional 'a?'", []() -> bool {
        NFA *nfaOptional = regex_parse("a?");

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

    START_TESTS()
}
