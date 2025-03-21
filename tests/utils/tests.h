//
// Created by ian on 1/12/25.
//

#ifndef TESTS_H
#define TESTS_H

#include<chrono>
using namespace std;
using namespace std::chrono;

struct Test {
    string name;
    bool(*action)();
};

#define INIT_TESTING() Test* test; vector<Test*> tests; auto start = high_resolution_clock::now();

#define TEST(x,y) \
    test = new Test; \
    test->name = (x); \
    test->action = (y); \
    tests.push_back(test);

#define START_TESTS() \
    int testsPassed = 0; \
    int testsFailed = 0; \
    for (Test* test : tests) { \
        auto curStart = high_resolution_clock::now(); \
        bool pass = test->action(); \
        auto curStop = high_resolution_clock::now(); \
        auto duration = duration_cast<milliseconds>(curStop - curStart); \
        cout << "TEST " << testsPassed + testsFailed + 1 << ": "; \
        if (pass) { \
            cout << test->name << " Passed "; \
            testsPassed++; \
        } \
        else { \
            cout << test->name << " Failed "; \
            testsFailed++; \
        } \
        cout << "(" << duration << ")" << endl; \
        delete test; \
    } \
    cout << endl << "Tests Passed: " << testsPassed << " Tests Failed: " << testsFailed << endl; \
    auto stop = high_resolution_clock::now(); \
    auto duration = duration_cast<milliseconds>(stop - start); \
    cout << endl << duration << endl; \
    return 0;

#endif //TESTS_H
