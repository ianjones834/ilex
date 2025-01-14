cmake_minimum_required(VERSION 3.30)
project(ilex)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")

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
        tests/utils.cpp
        tests/utils.h
        src/nfa.h
        src/nfa.cpp
)

add_executable(regex-tests tests/regex-tests.cpp
        tests/tests.h
        tests/utils.cpp
        tests/utils.h
        src/nfa.h
        src/nfa.cpp
        src/regex.h
        src/regex.cpp
)
