cmake_minimum_required(VERSION 3.30)
project(ilex)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")

add_executable(ilex src/main.cpp
        src/convert.cpp
        src/convert.h
        src/dfa.cpp
        src/dfa.h
        src/nfa.cpp
        src/nfa.h
        src/regex.cpp
        src/regex.h
        src/state.h
        src/ilex.h
        tests/utils.cpp
        tests/utils.h
)

add_executable(nfa-tests tests/nfa-tests.cpp
        tests/tests.h
        tests/utils.cpp
        tests/utils.h
        src/nfa.h
        src/nfa.cpp
        src/state.h
)

add_executable(regex-tests tests/regex-tests.cpp
        tests/tests.h
        tests/utils.cpp
        tests/utils.h
        src/nfa.h
        src/nfa.cpp
        src/regex.h
        src/regex.cpp
        src/state.h
)
add_executable(convert-tests tests/convert-tests.cpp
        tests/utils.h
        tests/utils.cpp
        tests/tests.h
        src/nfa.h
        src/nfa.cpp
        src/regex.cpp
        src/regex.h
        src/state.h
        src/dfa.cpp
        src/dfa.h
        src/convert.cpp
        src/convert.h
)
add_executable(ilex.yy src/ilex.yy.cpp)
add_executable(ilex-tests tests/ilex-tests.cpp
    ilex
)
