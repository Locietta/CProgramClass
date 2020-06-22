/**
 * @file polynomial.h
 * @author Locietta (https://github.com/Locietta)
 * @brief 
 * @version 0.1
 * @date 2020-06-22 09:28
 * 
 * 
 */

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "list.h"

typedef struct poly_item {
    double coeff, power;
} polyItem;

list createPolynomial(char *poly_string_formatted);

list addPolynomial(list h1, list h2);

list subPolynomial(list h1, list h2);

list multiplyPolynomial(list p1, list p2);

void PrintPolynomial(list p, char *target);

list clonePolynomial(list p);

#endif // (!defined POLYNOMIAL_H)
