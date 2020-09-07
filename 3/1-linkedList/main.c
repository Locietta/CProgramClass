#include <stdio.h>
#include "list.h"

#define INPUT_LINES 2

int main(void) {
    list S3 = newlist(int);
    int lines = 0, temp;
    while (scanf("%d", &temp) != EOF) {
        if (temp == -1) {
            if (++lines == INPUT_LINES) 
                break;
            else 
                continue; 
        }
        S3.push_back(&temp);
    }
    S3.setcmp(_int_comparer_).sort();

    if (S3.empty()) {
        puts("NULL");
    } else {
        printf("%d", *(int *) S3.front());
        for (iterator it = next_iterator(S3.begin()); it != S3.end(); it = next_iterator(it)) {
            printf(" %d", deref_iterator(int, it));
        }
    }

    return 0;
}