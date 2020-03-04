#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "currency.h"

void Test(char *filename);

int main(int argc, char *argv[]) {
    switch (argc) {
    case 1: Test("exchange.dat"); break;
    case 2: Test(argv[1]); break;
    default:
        puts("Error: too many arguments");
        exit(EXIT_FAILURE);
        break;
    }
    return 0;
}

void Test(char *filename) {
    currencyDB currencyLib = dbInit();
    FILE *fp;
    if ((fp = fopen(filename, "r")) != NULL) {
        while ((currencyLib->add(currencyLib, fp)) != EOF) continue;
        fclose(fp);
    } else {
        printf("Error: fail to open the file \"%s\"\n", filename);
        exit(EXIT_FAILURE);
    }

    puts("---------------------------------------------");
    puts("Currency now in database: ");
    for (int i = 0; i < currencyLib->nCurrency; ++i) {
        printf("%s\t%g\n", currencyLib->currency[i]->name,
               currencyLib->currency[i]->exchangeRate);
    }
    puts("---------------------------------------------");

    char currencyName[2][MAX_CURRENCY_NAME];
    int position[2];
    double unitAmount;

    printf("Convert from: ");
    scanf("%s", currencyName[0]);
    while ((position[0] = currencyLib->search(currencyLib, currencyName[0])) < 0) {
        printf("currency \"%s\" isn't on database!\n", currencyName[0]);
        printf("Convert from: ");
        scanf("%s", currencyName[0]);
    }

    printf("Into: ");
    scanf("%s", currencyName[1]);
    while ((position[1] = currencyLib->search(currencyLib, currencyName[1])) < 0) {
        printf("currency \"%s\" isn't on database!\n", currencyName[1]);
        printf("Into: ");
        scanf("%s", currencyName[1]);
    }

    printf("How many units of %s? ", currencyName[0]);
    scanf("%lf", &unitAmount);
    printf("%g %s = %.1f %s", unitAmount, currencyName[0],
           unitAmount * currencyLib->currency[position[0]]->exchangeRate / currencyLib->currency[position[1]]->exchangeRate,
           currencyName[1]);
    currencyLib->destory(currencyLib);
}