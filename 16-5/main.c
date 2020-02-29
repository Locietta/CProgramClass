#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "rational.h"

int main(void)
{
    while (1) {
        puts("Please type two fractions with the format num/den...");
        rationalT r1 = GetRational(), r2 = GetRational();
        puts("Addition result:");
        PrintRational(AddRational(r1, r2));
        puts("\nMultiplation result:");
        PrintRational(MultiplyRational(r1, r2));
        puts("\nDo you want to try again?(y/n)");
        char c;
        do {
            c = getchar();
        } while (c == '\n');
        while (getchar() != '\n')
            continue;
        if (c == 'n') break;
    }
}
