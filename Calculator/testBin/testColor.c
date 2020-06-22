#include "color.h"
#include <stdio.h>

int main(void) {
    puts("This is default");
    setColor(YELLOW);
    puts("This is yellow");
    colorPrintf(RED, "Try my magical colorPrintf() %d\n", 233);
    printf("this is a normal printf\n");
    return 0;
}