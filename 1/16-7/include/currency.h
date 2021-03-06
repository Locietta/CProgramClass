/**
 * File: currency.h
 * @Locietta
 * -----------------------------------------------------
 * The currency.h file defines the interface for a simple
 * currency database.
 */

#ifndef CURRENCY_H
#define CURRENCY_H

#include <stdio.h>

#define MAX_CURRENCY_NAME 15
#define MAX_CURRENCY_SORTS 1000

typedef struct {
    char name[MAX_CURRENCY_NAME];
    double exchangeRate;
} * currencyT;

typedef struct currencyDB_info *currencyDB;
struct currencyDB_info {
    currencyT currency[MAX_CURRENCY_SORTS];
    int nCurrency;
    /* Private function prototypes */
    int (*add)(currencyDB this, FILE *stream);
    int (*search)(currencyDB this, char *currencyName);
    void (*destory)(currencyDB this);
};

/* Public functions prototypes */

currencyDB dbInit(void);

#endif // (!defined CURRENCY_H)