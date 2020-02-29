/**
 * File: rational.h
 * @Locietta
 * -----------------------------------------------------
 * The rational.h file defines the interface for a simple
 * rational number library.
 */

#ifndef RATIONAL_H
#define RATIONAL_H

typedef struct {
    int num; //numerator, can be negative
    int den; //denominator, always positive
} rationalT;

rationalT CreateRational(int num, int den);

rationalT AddRational(rationalT r1, rationalT r2);

rationalT MultiplyRational(rationalT r1, rationalT r2);

rationalT GetRational(void);

void PrintRational(rationalT r);

#endif // (!defined RATIONAL_H)