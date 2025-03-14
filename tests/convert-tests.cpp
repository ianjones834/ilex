//
// Created by ian on 1/20/25.
//

#include "tests.h"
#include "../src/Regex/regex.h"
#include "utils.h"

#define COMMA ,
#define CONVERT_TEST_PASS(x,y,z) \
{string testName = "CONVERT PASS ("; testName += (x); testName += ","; testName += (y); testName += ", {" + aggregate((z)) + "})"; \
TEST(testName, []() -> bool { \
    NFA* nfa = regex_parse((x)); \
    DFA* dfa = new DFA(nfa); \
    for (NFAState* s : nfa->states) delete s; \
    delete nfa; \
    unordered_set<string> matches = simulate_dfa(dfa, (y)); \
    if (matches != (z)) cout << matches << endl << *dfa; \
    for (DFAState* s : dfa->states) delete s; \
    delete dfa; \
    return matches == (z); \
    })}

int main() {
    INIT_TESTING()

    CONVERT_TEST_PASS("a", "a", unordered_set<string>{"a"})
    CONVERT_TEST_PASS("b", "b", unordered_set<string>{"b"})
    CONVERT_TEST_PASS("a", "b", unordered_set<string>{})
    CONVERT_TEST_PASS("a|b", "a", unordered_set<string>{"a"})
    CONVERT_TEST_PASS("a|b", "b", unordered_set<string>{"b"})
    CONVERT_TEST_PASS("a|a", "a", unordered_set<string>{"a"})
    CONVERT_TEST_PASS("aa", "aa", unordered_set<string>{"aa"})
    CONVERT_TEST_PASS("aa", "a", unordered_set<string>{})
    CONVERT_TEST_PASS("aa", "b", unordered_set<string>{})
    CONVERT_TEST_PASS("aaa", "aaa", unordered_set<string>{"aaa"})
    CONVERT_TEST_PASS("aaa", "aa", unordered_set<string>{})
    CONVERT_TEST_PASS("aaa", "a", unordered_set<string>{})
    CONVERT_TEST_PASS("a*", "a", unordered_set<string>{"a"})
    CONVERT_TEST_PASS("a*", "aa", unordered_set<string>{"a" COMMA "aa"})
    CONVERT_TEST_PASS("a*", "ab", unordered_set<string>{"a"})
    CONVERT_TEST_PASS("a+", "a", unordered_set<string>{"a"})
    CONVERT_TEST_PASS("a+", "aaa", unordered_set<string>{"a" COMMA "aa" COMMA "aaa"})
    CONVERT_TEST_PASS("(a|b)*", "a", unordered_set<string>{"a"})
    CONVERT_TEST_PASS("(a|b)*", "b", unordered_set<string>{"b"})
    CONVERT_TEST_PASS("(a|b)*", "ab", unordered_set<string>{"a" COMMA "ab" COMMA "b"})
    CONVERT_TEST_PASS("(a|b)*", "abab", unordered_set<string>{"a" COMMA "ab" COMMA "aba" COMMA "abab" COMMA "b" COMMA "ba" COMMA "bab"})
    CONVERT_TEST_PASS("(a|b)*", "abba", unordered_set<string>{"a" COMMA "ab" COMMA "abb" COMMA "abba" COMMA "b" COMMA "bb" COMMA "bba" COMMA "ba"})
    CONVERT_TEST_PASS("(a|b)+", "abba", unordered_set<string>{"a" COMMA "ab" COMMA "abb" COMMA "abba" COMMA "b" COMMA "bb" COMMA "bba" COMMA "ba"})
    CONVERT_TEST_PASS("(a|b)+c", "abba", unordered_set<string>{})
    CONVERT_TEST_PASS("(a|b)+c", "abbac", unordered_set<string>{"abbac" COMMA "bbac" COMMA "bac" COMMA "ac"})
    CONVERT_TEST_PASS("(a|b)+c", "abbacc", unordered_set<string>{"abbac" COMMA "bbac" COMMA "bac" COMMA "ac"})
    CONVERT_TEST_PASS("(a|b)+c?", "abba", unordered_set<string>{"a" COMMA "ab" COMMA "abb" COMMA "abba" COMMA "b" COMMA "bb" COMMA "bba" COMMA "ba"})
    CONVERT_TEST_PASS("((a|b)+)?c?", "c", unordered_set<string>{"c"})
    CONVERT_TEST_PASS("((a|b)+)?c?", "a", unordered_set<string>{"a"})
    CONVERT_TEST_PASS("((ac|bd)+)?c?", "a", unordered_set<string>{})
    CONVERT_TEST_PASS("((ac|bd)+)?c*?", "c", unordered_set<string>{"c"})
    CONVERT_TEST_PASS("((ac|bd)+)?c*?", "ccccc", unordered_set<string>{"c" COMMA "cc" COMMA "ccc" COMMA "cccc" COMMA "ccccc"})
    CONVERT_TEST_PASS("((ac|bd)+)?c?*", "ccccc", unordered_set<string>{"c" COMMA "cc" COMMA "ccc" COMMA "cccc" COMMA "ccccc" COMMA "ccccc"})
    CONVERT_TEST_PASS("((ac|bd)+)?c*?", "accc", unordered_set<string>{"c" COMMA "cc" COMMA "ccc" COMMA "ac" COMMA "acc" COMMA "accc"})
    CONVERT_TEST_PASS("[ab]", "a", unordered_set<string>{"a"})
    CONVERT_TEST_PASS("[ab]", "b", unordered_set<string>{"b"})
    CONVERT_TEST_PASS("[a-c]*", "abc", unordered_set<string>{"a" COMMA "ab" COMMA "abc" COMMA "b" COMMA "bc" COMMA "c"})
    CONVERT_TEST_PASS("[a-c]+", "abc", unordered_set<string>{"a" COMMA "ab" COMMA "abc" COMMA "b" COMMA "bc" COMMA "c"})
    CONVERT_TEST_PASS("[ a-zA-Z0-9]*", "a 1", unordered_set<string>{ "a" COMMA "a " COMMA "a 1" COMMA " " COMMA " 1" COMMA "1"})
    CONVERT_TEST_PASS("[^a-z]*", "H", unordered_set<string>{"H"})
    CONVERT_TEST_PASS("[^a-z]", "a", unordered_set<string>{})
    CONVERT_TEST_PASS("^ian$", "ian", unordered_set<string>{"ian"})
    CONVERT_TEST_PASS("^ian$", "abian", unordered_set<string>{})
    CONVERT_TEST_PASS("^ian$", "ianj", unordered_set<string>{})
    CONVERT_TEST_PASS("a/b", "ab", unordered_set<string>{"a"})
    CONVERT_TEST_PASS("ian/jones", "ianjones", unordered_set<string>{"ian"})
    CONVERT_TEST_PASS(".", " ", unordered_set<string>{" "})
    CONVERT_TEST_PASS("[^a-z]", "A", unordered_set<string>{"A"})
    CONVERT_TEST_PASS("[^a-z]", "a", unordered_set<string>{})
    CONVERT_TEST_PASS("A{1,3}", "A", unordered_set<string>{"A"})
    CONVERT_TEST_PASS("A{1,3}", "AA", unordered_set<string>{"A" COMMA "AA"})
    CONVERT_TEST_PASS("A{1,3}", "AAA", unordered_set<string>{"A" COMMA "AA" COMMA "AAA"})
    CONVERT_TEST_PASS("A{1,3}", "AAAA", unordered_set<string>{"A" COMMA "AA" COMMA "AAA"})
    CONVERT_TEST_PASS("A{3,5}", "AAAA", unordered_set<string>{"AAA" COMMA "AAAA"})
    CONVERT_TEST_PASS("(jones|ian)(ian)?(ian)?", "jonesianian", unordered_set<string>{"jones" COMMA "jonesian" COMMA "jonesianian" COMMA "ian" COMMA "ianian"})
    CONVERT_TEST_PASS("(alice|bob){1,3}", "alice", unordered_set<string>{"alice"})
    CONVERT_TEST_PASS("(alice|bob){1,3}", "alicealice", unordered_set<string>{"alice" COMMA "alicealice"})
    CONVERT_TEST_PASS("(alice|bob){1,3}", "alicealicealicealice", unordered_set<string>{"alice" COMMA "alicealice" COMMA "alicealicealice"})
    CONVERT_TEST_PASS("(alice|bob){1,3}", "bobalicebob", unordered_set<string>{"bob" COMMA "bobalice" COMMA "bobalicebob" COMMA "alicebob" COMMA "alice"})

    START_TESTS()
}
