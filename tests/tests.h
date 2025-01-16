//
// Created by ian on 1/12/25.
//

#ifndef TESTS_H
#define TESTS_H

#include<string>
#include<iostream>
#include<vector>
using namespace std;

struct Test {
    string name;
    bool(*action)();
};

#define INIT_TESTING() Test* test; vector<Test*> tests;

#define TEST(x,y) \
    test = new Test; \
    test->name = (x); \
    test->action = (y); \
    tests.push_back(test);

#define START_TESTS() \
    int testsPassed = 0; \
    int testsFailed = 0; \
    for (int i = 0; i < tests.size(); i++) { \
        bool pass = tests[i]->action(); \
        cout << "TEST " << i + 1 << ": "; \
        if (pass) { \
            cout << tests[i]->name << " Passed" << endl; \
            testsPassed++; \
        } \
        else { \
            cout << tests[i]->name << " Failed" << endl; \
            testsFailed++; \
        } \
        delete tests[i]; \
    } \
    cout << endl << "Tests Passed: " << testsPassed << " Tests Failed: " << testsFailed << endl;

#endif //TESTS_H
