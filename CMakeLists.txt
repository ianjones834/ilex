cmake_minimum_required(VERSION 3.30)
project(ilex)

set(CMAKE_CXX_STANDARD 20)

add_executable(ilex main.cpp
        src/convert.cpp
        src/convert.h
        src/dfa.cpp
        src/dfa.h
        src/nfa.cpp
        src/nfa.h
        src/regex.cpp
        src/regex.h)

add_executable(nfa-tests tests/nfa-tests.cpp
        tests/tests.h
)
add_executable(test-tests tests/test-tests.cpp
        tests/tests.h)
