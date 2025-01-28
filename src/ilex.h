//
// Created by ian on 1/27/25.
//

#ifndef ILEX_H
#define ILEX_H

#include <iostream>
#include <fstream>
#include "convert.h"

int definitionsScanner(ifstream&, ofstream&);
int rulesScanner(ifstream&, ofstream&);
int subroutinesScanner(ifstream&, ofstream&);

#endif //ILEX_H