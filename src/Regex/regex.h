//
// Created by ian on 1/12/25.
//

#ifndef REGEX_H
#define REGEX_H

#include "../Automata/NFA.h"
#include<string>
using namespace std;

NFA* regex_parse(string, int, bool = true);

#endif //REGEX_H
