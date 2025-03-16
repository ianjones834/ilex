cmake_minimum_required(VERSION 3.30)
project(ilex)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")

add_executable(ilex src/main.cpp
        src/Automata/DFA.cpp
        src/Automata/DFA.h
        src/Automata/NFA.cpp
        src/Automata/NFA.h
        src/Regex/regex.cpp
        src/Regex/regex.h
        src/Automata/NFAState.h
        src/ilex.h
        tests/utils/utils.cpp
        tests/utils/utils.h
        src/Automata/DFAState.h
        src/Automata/DFAState.cpp
        src/Automata/NFAState.cpp
)

add_executable(nfa-tests tests/nfa-tests.cpp
        tests/utils/tests.h
        tests/utils/utils.cpp
        tests/utils/utils.h
        src/Automata/NFA.h
        src/Automata/NFA.cpp
        src/Automata/NFAState.h
        src/Automata/NFAState.cpp
)

add_executable(regex-tests tests/regex-tests.cpp
        tests/utils/tests.h
        tests/utils/utils.cpp
        tests/utils/utils.h
        src/Automata/NFA.h
        src/Automata/NFA.cpp
        src/Regex/regex.h
        src/Regex/regex.cpp
        src/Automata/NFAState.h
        src/Automata/DFAState.h
        src/Automata/DFAState.cpp
        src/Automata/NFAState.cpp
)
add_executable(convert-tests tests/convert-tests.cpp
        tests/utils/utils.h
        tests/utils/utils.cpp
        tests/utils/tests.h
        src/Automata/DFAState.h
        src/Automata/DFAState.cpp
        src/Automata/NFAState.h
        src/Automata/NFAState.cpp
        src/Automata/NFA.h
        src/Automata/NFA.cpp
        src/Automata/DFA.cpp
        src/Automata/DFA.h
        src/Regex/regex.cpp
        src/Regex/regex.h
)

add_executable(ilex-tests tests/ilex-tests.cpp
        src/Automata/DFAState.h
        src/Automata/DFAState.cpp
        src/Automata/NFAState.cpp
)
add_executable(ilex.yy.text src/ilex.yy.test.cpp)
