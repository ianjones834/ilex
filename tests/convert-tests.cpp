//
// Created by ian on 1/20/25.
//

#include "tests.h"
#include "../src/regex.h"
#include "../src/convert.h"
#include "utils.h"

#define CONVERT_TEST_PASS(x,y) \
{string testName = "CONVERT ("; testName += (x); testName += ","; testName += (y); testName += ")"; \
TEST(testName, []() -> bool { \
    DFA* dfa = convert(regex_parse((x))); \
    bool res = simulate_dfa(dfa, (y)); \
    if (!res) cout << *dfa; \
    return res; \
    })}

#define CONVERT_TEST_FAIL(x,y) \
{string testName = "CONVERT ("; testName += (x); testName += ", "; testName += (y); testName += ")"; \
    TEST(testName, []() -> bool { \
    DFA* dfa = convert(regex_parse((x))); \
    bool res = simulate_dfa(dfa, (y)); \
    if (res) cout << *dfa; \
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
    CONVERT_TEST_PASS("[ a-zA-Z0-9]*", "Hello there my name is ianjones834")

    START_TESTS()
}
