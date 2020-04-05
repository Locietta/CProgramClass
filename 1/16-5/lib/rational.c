/**
 * File: rational.c
 * @Locietta
 * -----------------------------------------------------
 * This file implements the rational.h interface.
 */

#include "rational.h"
#include <stdio.h>
#include <stdlib.h>

/* Private function prototypes */

static int gcd(int a, int b);

/* Library function implementation */

/* Section 1 -- Basic fraction operations */

rationalT CreateRational(int num, int den) {
    if (den == 0) {
        puts("denominator of fraction can't be 0!");
        exit(1);
    }
    int sharedFactor = gcd(num, den);
    if (sharedFactor > 1) {
        num /= sharedFactor;
        den /= sharedFactor;
    }
    if (den < 0) {
        den = -den;
        num = -num;
    }
    return (rationalT){.num = num, .den = den};
}

rationalT AddRational(rationalT r1, rationalT r2) {
    int newnum = r1.num * r2.den + r1.den * r2.num;
    int newden = r1.den * r2.den;
    int sharedFactor = gcd(newden, newnum);
    if (sharedFactor > 1) {
        newnum /= sharedFactor;
        newden /= sharedFactor;
    }
    return (rationalT){.num = newnum, .den = newden};
}

rationalT MultiplyRational(rationalT r1, rationalT r2) {
    int sharedFactor = gcd(r1.num, r2.den);
    if (sharedFactor > 1) {
        r1.num /= sharedFactor;
        r2.den /= sharedFactor;
    }
    if ((sharedFactor = gcd(r1.den, r2.num)) > 1) {
        r1.den /= sharedFactor;
        r2.num /= sharedFactor;
    }
    return (rationalT){.num = r1.num * r2.num, .den = r1.den * r2.den};
}

/* Section 2 -- fraction I/O functions */

rationalT GetRational(void) {
    int num, den;
    scanf("%d/%d", &num, &den);
    return CreateRational(num, den);
}

void PrintRational(rationalT r) {
    if (r.den > 1 && r.num) {
        printf("%d/%d", r.num, r.den);
    } else {  // r.den == 1 or r.num == 0, it's an interger
        printf("%d", r.num);
    }
}

/* Private functions */

static int gcd(int a, int b) {
    return (b ? gcd(b, a % b) : abs(a));
}