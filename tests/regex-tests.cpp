//
// Created by ian on 1/14/25.
//

#include "tests.h"
#include "../src/regex.h"
#include "utils.h"

#define REGEX_TEST_PASS(x,y) \
{string testName = "Regex ("; testName += (x); testName += ","; testName += (y); testName += ")"; \
TEST(testName, []() -> bool { \
    NFA* nfa = regex_parse((x)); \
    bool res = simulate_nfa(nfa, (y)); \
    if (!res) cout << *nfa; \
    return res; \
    })}

#define REGEX_TEST_FAIL(x,y) \
{string testName = "Regex ("; testName += (x); testName += ", "; testName += (y); testName += ")"; \
    TEST(testName, []() -> bool { \
    NFA* nfa = regex_parse((x)); \
    bool res = simulate_nfa(nfa, (y)); \
    if (res) cout << *nfa; \
    return !res; \
    })}

int main() {
    INIT_TESTING()

    REGEX_TEST_PASS("a", "a")
    REGEX_TEST_PASS("b", "b")
    REGEX_TEST_FAIL("a", "b")
    REGEX_TEST_PASS("a|b", "a")
    REGEX_TEST_PASS("a|b", "b")
    REGEX_TEST_PASS("a|a", "a")
    REGEX_TEST_PASS("aa", "aa")
    REGEX_TEST_FAIL("aa", "a")
    REGEX_TEST_FAIL("aa", "b")
    REGEX_TEST_PASS("aaa", "aaa")
    REGEX_TEST_FAIL("aaa", "aa")
    REGEX_TEST_FAIL("aaa", "a")
    REGEX_TEST_PASS("a*", "")
    REGEX_TEST_PASS("a*", "a")
    REGEX_TEST_PASS("a*", "aa")
    REGEX_TEST_FAIL("a*", "ab")
    REGEX_TEST_PASS("a+", "a")
    REGEX_TEST_FAIL("a+", "")
    REGEX_TEST_PASS("a+", "aaa")
    REGEX_TEST_PASS("(a|b)*", "a")
    REGEX_TEST_PASS("(a|b)*", "b")
    REGEX_TEST_PASS("(a|b)*", "ab")
    REGEX_TEST_PASS("(a|b)*", "abab")
    REGEX_TEST_PASS("(a|b)*", "abba")
    REGEX_TEST_PASS("(a|b)+", "abba")
    REGEX_TEST_FAIL("(a|b)+c", "abba")
    REGEX_TEST_PASS("(a|b)+c", "abbac")
    REGEX_TEST_FAIL("(a|b)+c", "abbacc")
    REGEX_TEST_PASS("(a|b)+ca*", "abbacaaaa")
    REGEX_TEST_PASS("(a|b)+c?", "abba")
    REGEX_TEST_PASS("((a|b)+)?c?", "")
    REGEX_TEST_PASS("((a|b)+)?c?", "c")
    REGEX_TEST_PASS("((a|b)+)?c?", "a")
    REGEX_TEST_FAIL("((ac|bd)+)?c?", "a")
    REGEX_TEST_PASS("((ac|bd)+)?c?", "")
    REGEX_TEST_PASS("((ac|bd)+)?c*?", "c")
    REGEX_TEST_PASS("((ac|bd)+)?c*?", "ccccc")
    REGEX_TEST_PASS("((ac|bd)+)?c?*", "ccccc")
    REGEX_TEST_PASS("((ac|bd)+)?c*?", "")
    REGEX_TEST_PASS("((ac|bd)+)?c*?", "accc")
    REGEX_TEST_PASS("[ab]", "a")
    REGEX_TEST_PASS("[ab]", "b")
    REGEX_TEST_PASS("[a-c]*", "abc")
    REGEX_TEST_PASS("[a-c]+", "abc")
    REGEX_TEST_PASS("[ a-zA-Z0-9]*", "Hello there my name is ianjones834")

    START_TESTS()
}
