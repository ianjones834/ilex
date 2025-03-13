#include<unordered_map>
#include<unordered_set>
using namespace std;

struct DFAState;

struct DFA {
    DFAState* start;
    unordered_set<DFAState*> states;
};

struct DFAState {
    int actionNum;
    int matchStartActionNum;
    int matchEndActionNum;
    int matchStartAndEndActionNum;
    bool acceptState;

    int curCharIndex;
    unordered_set<DFAState*> backTo;

    unordered_map<char, DFAState*> transitions;
};

const DFA dfa = DFA{
  .states = unordered_set<DFAState*>{
      new DFAState{.secondaryDFA = {nullptr, nullptr}},
      new DFAState{.actionNum = 0}
    }
};