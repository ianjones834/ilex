//
// Created by ian on 1/27/25.
//

#ifndef ILEX_H
#define ILEX_H

#include <iostream>
using namespace std;

int ilex(istream&, ofstream&);

int definitionsScanner(istream&, ofstream&);
int rulesScanner(istream&, ofstream&);
int subroutinesScanner(istream&, ofstream&);

#endif //ILEX_H