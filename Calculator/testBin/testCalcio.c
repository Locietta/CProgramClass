#include "calcio.h"
#include "color.h"

int main(void) {
    char buffer[100];
    while (Expr_gets(buffer, 100)) {
        colorPrintf(PINK, buffer);
        putchar('\n');
    }
    return 0;
}