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
        bool res = simulate_nfa(nfa, string("a"));

        for (State* state : nfa->states) {
            delete state;
        }

        delete nfa;

        return res;
    })

    TEST("NFA SINGLE CHAR (a, 'aa')", []() -> bool {
        NFA* nfa = nfa_new_single_char('a');
        bool res = simulate_nfa(nfa, string("aa"));

        for (State* state : nfa->states) {
            delete state;
        }

        delete nfa;

        return !res;
    })

    TEST("NFA SINGLE CHAR (a, ' ')", []() -> bool {
        NFA* nfa = nfa_new_single_char('a');
        bool res = simulate_nfa(nfa, " ");

        for (State* state : nfa->states) {
            delete state;
        }

        delete nfa;

        return !res;
    })

    TEST("NFA Union (a|b, 'a')", []() -> bool {
        NFA* nfa = nfa_union(nfa_new_single_char('a'), nfa_new_single_char('b'));

        bool res = simulate_nfa(nfa, "a");

        for (State* state : nfa->states) {
            delete state;
        }

        delete nfa;

        return res;
    })

    TEST("NFA Union (a|b, 'b')", []() -> bool {
        NFA* nfa = nfa_union(nfa_new_single_char('a'), nfa_new_single_char('b'));

        bool res = simulate_nfa(nfa, "b");

        for (State* state : nfa->states) {
            delete state;
        }

        delete nfa;

        return res;
    })

    TEST("NFA Union fail (a|b, 'ab')", []() -> bool {
        NFA* nfa = nfa_union(nfa_new_single_char('a'), nfa_new_single_char('b'));

        bool res = simulate_nfa(nfa, "ab");

        for (State* state : nfa->states) {
            delete state;
        }

        delete nfa;

        return !res;
    })

    TEST("NFA Concat ('aa', 'aa')", []() -> bool {
        NFA* nfa = nfa_concat(nfa_new_single_char('a'), nfa_new_single_char('a'));

        bool res = simulate_nfa(nfa, "aa");

        for (State* state : nfa->states)
            delete state;

        delete nfa;

        return res;
    })

    TEST("NFA Concat ('aa', 'ab')", []() -> bool {
        NFA* nfa = nfa_concat(nfa_new_single_char('a'), nfa_new_single_char('a'));

        bool res = simulate_nfa(nfa, "ab");

        for (State* state : nfa->states)
            delete state;

        delete nfa;

        return !res;
    })

    TEST("NFA Concat ('aa', 'a')", []() -> bool {
       NFA* nfa = nfa_concat(nfa_new_single_char('a'), nfa_new_single_char('a'));

       bool res = simulate_nfa(nfa, "a");

       for (State* state : nfa->states)
           delete state;

       delete nfa;

       return !res;
   })

    TEST("NFA Zero or More (a*, 'aaa')", []() -> bool {
        NFA* nfa = nfa_zero_or_more(nfa_new_single_char('a'));

        bool res = simulate_nfa(nfa, "aaa");

        for (State* state : nfa->states)
            delete state;

        delete nfa;

        return res;
    })

    TEST("NFA Zero or More (a*, '')", []() -> bool {
        NFA* nfa = nfa_zero_or_more(nfa_new_single_char('a'));

        bool res = simulate_nfa(nfa, "");

        for (State* state : nfa->states)
            delete state;

        delete nfa;

        return res;
    })

    TEST("NFA Zero or More (a*, 'b')", []() -> bool {
        NFA* nfa = nfa_zero_or_more(nfa_new_single_char('a'));

        bool res = simulate_nfa(nfa, "b");

        for (State* state : nfa->states)
            delete state;

        delete nfa;

        return !res;
    })

    TEST("NFA One or More (a+, 'a')", []() -> bool {
        NFA* nfa = nfa_one_or_more(nfa_new_single_char('a'));

        bool res = simulate_nfa(nfa, "a");

        for (State* state : nfa->states)
            delete state;

        delete nfa;

        return res;
    })

    TEST("NFA Range ([a-z]*, 'abcdefghijklmnopqrstuvwxyz')", []() -> bool {
        NFA* nfa = nfa_zero_or_more(nfa_range({}, {{'a', 'z'}}));

        bool res = simulate_nfa(nfa, "abcdefghijklmnopqrstuvwxyz");

        for (State* state : nfa->states) {
            delete state;
        }

        delete nfa;

        return res;
    })

    TEST("NFA any character ( ., ' ')", []() -> bool {
        NFA* nfa = nfa_any();

        bool res = simulate_nfa(nfa, " ");

        for (State* state : nfa->states) {
            delete state;
        }

        delete nfa;

        return res;
    })

    START_TESTS()
  }
