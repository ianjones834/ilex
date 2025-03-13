//
// Created by ian on 1/20/25.
//

#include "tests.h"
#include "../src/Regex/regex.h"
#include "utils.h"

#define CONVERT_TEST_PASS(x,y) \
{string testName = "CONVERT PASS ("; testName += (x); testName += ","; testName += (y); testName += ")"; \
TEST(testName, []() -> bool { \
    NFA* nfa = regex_parse((x)); \
    DFA* dfa = new DFA(nfa); \
    for (NFAState* s : nfa->states) delete s; \
    delete nfa; \
    bool res = simulate_dfa(dfa, (y)); \
    if (!res) cout << *dfa; \
    for (DFAState* s : dfa->states) delete s; \
    delete dfa; \
    return res; \
    })}

#define CONVERT_TEST_FAIL(x,y) \
{string testName = "CONVERT FAIL ("; testName += (x); testName += ", "; testName += (y); testName += ")"; \
    TEST(testName, []() -> bool { \
    NFA * nfa = regex_parse((x)); \
    DFA* dfa = new DFA(nfa); \
    for (NFAState* s : nfa->states) delete s; \
    delete nfa; \
    bool res = simulate_dfa(dfa, (y)); \
    if (res) cout << *dfa; \
    for (DFAState* s : dfa->states) delete s; \
    delete dfa; \
    return !res; \
    })}

int main() {
    INIT_TESTING()

    CONVERT_TEST_PASS("a", "a")
    CONVERT_TEST_PASS("b", "b")
    CONVERT_TEST_FAIL("a", "b")
    CONVERT_TEST_PASS("a|b", "a")
    CONVERT_TEST_PASS("a|b", "b")
    CONVERT_TEST_PASS("a|a", "a")
    CONVERT_TEST_PASS("aa", "aa")
    CONVERT_TEST_FAIL("aa", "a")
    CONVERT_TEST_FAIL("aa", "b")
    CONVERT_TEST_PASS("aaa", "aaa")
    CONVERT_TEST_FAIL("aaa", "aa")
    CONVERT_TEST_FAIL("aaa", "a")
    CONVERT_TEST_PASS("a*", "")
    CONVERT_TEST_PASS("a*", "a")
    CONVERT_TEST_PASS("a*", "aa")
    CONVERT_TEST_FAIL("a*", "ab")
    CONVERT_TEST_PASS("a+", "a")
    CONVERT_TEST_FAIL("a+", "")
    CONVERT_TEST_PASS("a+", "aaa")
    CONVERT_TEST_PASS("(a|b)*", "a")
    CONVERT_TEST_PASS("(a|b)*", "b")
    CONVERT_TEST_PASS("(a|b)*", "ab")
    CONVERT_TEST_PASS("(a|b)*", "abab")
    CONVERT_TEST_PASS("(a|b)*", "abba")
    CONVERT_TEST_PASS("(a|b)+", "abba")
    CONVERT_TEST_FAIL("(a|b)+c", "abba")
    CONVERT_TEST_PASS("(a|b)+c", "abbac")
    CONVERT_TEST_FAIL("(a|b)+c", "abbacc")
    CONVERT_TEST_PASS("(a|b)+ca*", "abbacaaaa")
    CONVERT_TEST_PASS("(a|b)+c?", "abba")
    CONVERT_TEST_PASS("((a|b)+)?c?", "")
    CONVERT_TEST_PASS("((a|b)+)?c?", "c")
    CONVERT_TEST_PASS("((a|b)+)?c?", "a")
    CONVERT_TEST_FAIL("((ac|bd)+)?c?", "a")
    CONVERT_TEST_PASS("((ac|bd)+)?c?", "")
    CONVERT_TEST_PASS("((ac|bd)+)?c*?", "c")
    CONVERT_TEST_PASS("((ac|bd)+)?c*?", "ccccc")
    CONVERT_TEST_PASS("((ac|bd)+)?c?*", "ccccc")
    CONVERT_TEST_PASS("((ac|bd)+)?c*?", "")
    CONVERT_TEST_PASS("((ac|bd)+)?c*?", "accc")
    CONVERT_TEST_PASS("[ab]", "a")
    CONVERT_TEST_PASS("[ab]", "b")
    CONVERT_TEST_PASS("[a-c]*", "abc")
    CONVERT_TEST_PASS("[a-c]+", "abc")
    CONVERT_TEST_PASS("[ a-zA-Z0-9]", "A")
    CONVERT_TEST_PASS("[ a-zA-Z0-9]*", "Hello there my name is ianjones834")
    CONVERT_TEST_PASS(".", " ");
    CONVERT_TEST_PASS("[^a-z]", "A");
    CONVERT_TEST_FAIL("[^a-z]", "a");
    CONVERT_TEST_PASS("A{1,3}", "A");
    CONVERT_TEST_PASS("A{1,3}", "AA");
    CONVERT_TEST_PASS("A{1,3}", "AAA");
    CONVERT_TEST_FAIL("A{1,3}", "AAAA");
    CONVERT_TEST_PASS("A{1,5}", "AAAA");
    CONVERT_TEST_FAIL("A{3,5}", "AA");
    CONVERT_TEST_PASS("(jones|ian)(ian)?(ian)?", "jonesianian")
    CONVERT_TEST_PASS("(alice|bob){1,3}", "alice")
    CONVERT_TEST_PASS("(alice|bob){1,3}", "alicealice")
    CONVERT_TEST_PASS("(alice|bob){1,3}", "alicealice")
    CONVERT_TEST_PASS("(alice|bob){1,3}", "alicealicealice")
    CONVERT_TEST_FAIL("(alice|bob){1,3}", "alicealicealicealice")
    CONVERT_TEST_PASS("(alice|bob){1,3}", "bobalicebob")
    CONVERT_TEST_PASS("(alice|bob){0,3}", "alicebobbob")
    CONVERT_TEST_PASS("(alice|bob){0,3}", "")

    START_TESTS()
}
