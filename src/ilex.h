//
// Created by ian on 1/27/25.
//

#ifndef ILEX_H
#define ILEX_H

#include "convert.h"

int ilex(istream&, ostream&);

int definitionsScanner(istream&, ostream&);
int rulesScanner(istream&, ostream&);
int regexScanner(istream&, ostream&);
int subroutinesScanner(istream&, ostream&);
void eatSpace(string);

#endif //ILEX_H