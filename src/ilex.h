//
// Created by ian on 1/27/25.
//

#ifndef ILEX_H
#define ILEX_H

#include "convert.h"

int ilex(istream&, ostream&);

int definitionsScanner(istream&, ostream&);
int rulesScanner(istream&, ostream&);
int subroutinesScanner(istream&, ostream&);

#endif //ILEX_H