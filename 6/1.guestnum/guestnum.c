#include <stdio.h>

int main(void) {
    puts("Think of a number between 1 and 100 and I'll guess it.");
    int i = 1, j = 100, mid = 50;
    char answer[10];
    while (i <= j) {
        printf("Is it %d? ", mid);
        scanf("%s", answer);

        if (*answer == 'y') {
            puts("I guess the number!");
            break;
        } else {
            printf("Is it less than %d? ", mid);
            scanf("%s", answer);

            if (*answer == 'y') {
                j = mid - 1;
            } else {
                i = mid + 1;
            }
            mid = (i + j) / 2;
        }
    }
    if (i>j) {
        puts("Oh no... you didn't think of a number between 1 and 100, did you?");
    }
    return 0;
}