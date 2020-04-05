#include <stdio.h>

#include "genlib.h"
#include "simpio.h"
#include "stack.h"

int main(void) {
    stackADT stack_f = NewStack(), stack_m = NewStack();
    int temp, gender; //female gender == 1 / male gender == 0
    
    while (scanf("%d %d", &temp, &gender) != EOF) {
        if (gender) {
            if (IsemptyStack(stack_m)) {
                int *p_int = (int *) malloc(sizeof(int));
                *p_int = temp;
                PushStack(stack_f, p_int);
            } else {
                int *p = (int *) PopStack(stack_m);
                printf("(%d, %d)\n", *p, temp);
                FreeBlock(p);
            }
        } else {
            if (IsemptyStack(stack_f)) {
                int *p_int = (int *) malloc(sizeof(int));
                *p_int = temp;
                PushStack(stack_m, p_int);
            } else {
                int *p = (int *) PopStack(stack_f);
                printf("(%d, %d)\n", *p, temp);
                FreeBlock(p);
            }
        }
    }
    FreeStack(stack_f);
    FreeStack(stack_m);
    return 0;
}