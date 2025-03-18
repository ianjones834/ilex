//
// Created by ian on 1/14/25.
//

#include "utils/tests.h"
#include "../src/Regex/regex.h"
#include "utils/utils.h"

#define COMMA ,
#define REGEX_TEST_PASS(x,y,z) \
{string testName = "Regex PASS ("; testName += (x); testName += ","; testName += (y); testName += ", {" + aggregate((z)) + "})"; \
TEST(testName, []() -> bool { \
    NFA* nfa = regex_parse((x)); \
    unordered_set<string> matches = simulate_nfa(nfa, (y)); \
    if (matches != (z)) cout << matches << endl; \
    delete nfa; \
    return matches == (z); \
    })}

int main() {
    INIT_TESTING()

    REGEX_TEST_PASS("a", "a", unordered_set<string>{"a"})
    REGEX_TEST_PASS("b", "b", unordered_set<string>{"b"})
    REGEX_TEST_PASS("a", "b", unordered_set<string>{})
    REGEX_TEST_PASS("a|b", "a", unordered_set<string>{"a"})
    REGEX_TEST_PASS("a|b", "b", unordered_set<string>{"b"})
    REGEX_TEST_PASS("a|a", "a", unordered_set<string>{"a"})
    REGEX_TEST_PASS("aa", "aa", unordered_set<string>{"aa"})
    REGEX_TEST_PASS("aa", "a", unordered_set<string>{})
    REGEX_TEST_PASS("aa", "b", unordered_set<string>{})
    REGEX_TEST_PASS("aaa", "aaa", unordered_set<string>{"aaa"})
    REGEX_TEST_PASS("aaa", "aa", unordered_set<string>{})
    REGEX_TEST_PASS("aaa", "a", unordered_set<string>{})
    REGEX_TEST_PASS("a*", "a", unordered_set<string>{"a"})
    REGEX_TEST_PASS("a*", "aa", unordered_set<string>{"a" COMMA "aa"})
    REGEX_TEST_PASS("a*", "ab", unordered_set<string>{"a"})
    REGEX_TEST_PASS("a+", "a", unordered_set<string>{"a"})
    REGEX_TEST_PASS("a+", "aaa", unordered_set<string>{"a" COMMA "aa" COMMA "aaa"})
    REGEX_TEST_PASS("(a|b)*", "a", unordered_set<string>{"a"})
    REGEX_TEST_PASS("(a|b)*", "b", unordered_set<string>{"b"})
    REGEX_TEST_PASS("(a|b)*", "ab", unordered_set<string>{"a" COMMA "ab" COMMA "b"})
    REGEX_TEST_PASS("(a|b)*", "abab", unordered_set<string>{"a" COMMA "ab" COMMA "aba" COMMA "abab" COMMA "b" COMMA "ba" COMMA "bab"})
    REGEX_TEST_PASS("(a|b)*", "abba", unordered_set<string>{"a" COMMA "ab" COMMA "abb" COMMA "abba" COMMA "b" COMMA "bb" COMMA "bba" COMMA "ba"})
    REGEX_TEST_PASS("(a|b)+", "abba", unordered_set<string>{"a" COMMA "ab" COMMA "abb" COMMA "abba" COMMA "b" COMMA "bb" COMMA "bba" COMMA "ba"})
    REGEX_TEST_PASS("(a|b)+c", "abba", unordered_set<string>{})
    REGEX_TEST_PASS("(a|b)+c", "abbac", unordered_set<string>{"abbac" COMMA "bbac" COMMA "bac" COMMA "ac"})
    REGEX_TEST_PASS("(a|b)+c", "abbacc", unordered_set<string>{"abbac" COMMA "bbac" COMMA "bac" COMMA "ac"})
    REGEX_TEST_PASS("(a|b)+c?", "abba", unordered_set<string>{"a" COMMA "ab" COMMA "abb" COMMA "abba" COMMA "b" COMMA "bb" COMMA "bba" COMMA "ba"})
    REGEX_TEST_PASS("((a|b)+)?c?", "c", unordered_set<string>{"c"})
    REGEX_TEST_PASS("((a|b)+)?c?", "a", unordered_set<string>{"a"})
    REGEX_TEST_PASS("((ac|bd)+)?c?", "a", unordered_set<string>{})
    REGEX_TEST_PASS("((ac|bd)+)?c*?", "c", unordered_set<string>{"c"})
    REGEX_TEST_PASS("((ac|bd)+)?c*?", "ccccc", unordered_set<string>{"c" COMMA "cc" COMMA "ccc" COMMA "cccc" COMMA "ccccc"})
    REGEX_TEST_PASS("((ac|bd)+)?c?*", "ccccc", unordered_set<string>{"c" COMMA "cc" COMMA "ccc" COMMA "cccc" COMMA "ccccc" COMMA "ccccc"})
    REGEX_TEST_PASS("((ac|bd)+)?c*?", "accc", unordered_set<string>{"c" COMMA "cc" COMMA "ccc" COMMA "ac" COMMA "acc" COMMA "accc"})
    REGEX_TEST_PASS("[ab]", "a", unordered_set<string>{"a"})
    REGEX_TEST_PASS("[ab]", "b", unordered_set<string>{"b"})
    REGEX_TEST_PASS("[a-c]*", "abc", unordered_set<string>{"a" COMMA "ab" COMMA "abc" COMMA "b" COMMA "bc" COMMA "c"})
    REGEX_TEST_PASS("[a-c]+", "abc", unordered_set<string>{"a" COMMA "ab" COMMA "abc" COMMA "b" COMMA "bc" COMMA "c"})
    REGEX_TEST_PASS("[ a-zA-Z0-9]*", "a 1", unordered_set<string>{ "a" COMMA "a " COMMA "a 1" COMMA " " COMMA " 1" COMMA "1"})
    REGEX_TEST_PASS("[^a-z]*", "H", unordered_set<string>{"H"})
    REGEX_TEST_PASS("[^a-z]", "a", unordered_set<string>{})
    REGEX_TEST_PASS("^ian$", "ian", unordered_set<string>{"ian"})
    REGEX_TEST_PASS("^ian$", "abian", unordered_set<string>{})
    REGEX_TEST_PASS("^ian$", "ianj", unordered_set<string>{})
    REGEX_TEST_PASS("a/b", "ab", unordered_set<string>{"a"})
    REGEX_TEST_PASS("ian/jones", "ianjones", unordered_set<string>{"ian"})

    START_TESTS()
}
