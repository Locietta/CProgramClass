/**
 * File: currency.c
 * @Locietta
 * -----------------------------------------------------
 * This file implements the currency.h interface.
 */

#include "currency.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

currencyDB dbInit(void) {
    currencyDB newDB = (currencyDB) malloc(sizeof(struct currencyDB_info));
    newDB->nCurrency = 0;
    newDB->add = addCurrency;
    newDB->search = searchCurrency;
    newDB->destory = dbDestory;
    return newDB;
}

int addCurrency(currencyDB this, FILE *stream) {
    char newName[MAX_CURRENCY_NAME] = "";
    double newExRate;

    int ret = fscanf(stream, "%s %lf", newName, &newExRate);
    if (ret != EOF) {
        for (int i = 0; newName != '\0'; ++i) { // ignore alphabet case
            newName[i] = tolower(newName[i]);
        }

        for (int i = 0; i < this->nCurrency; ++i) {
            if (!strcmp(newName, this->currency[i]->name)) {
                this->currency[i]->exchangeRate = newExRate;
                return;
            }
        }

        currencyT newCurrency = (currencyT) malloc(sizeof(currencyT));
        strcpy(newCurrency->name, newName);
        newCurrency->exchangeRate = newExRate;
        this->currency[this->nCurrency] = newCurrency;
        this->nCurrency++;
    }
    return ret;
}

int searchCurrency(currencyDB this, char *currencyName) {
    int currencyNum = this->nCurrency;
    for (int i = 0; i < currencyNum; ++i) {
        if (!strcmp(this->currency[i]->name, currencyName)) {
            return i;
        }
    }
    return -1; //not found
}

void dbDestory(currencyDB this) {
    int currencyNum = this->nCurrency;
    for (int i = 0; i < currencyNum; ++i) {
        free(this->currency[i]);
    }
    free(this);
}