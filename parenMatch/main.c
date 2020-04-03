#include <stdio.h>

#include "genlib.h"
#include "simpio.h"
#include "stack.h"

int main(void) {
    stackADT stack = NewStack();
    int c;
    while ((c = getchar()) != '\n') {
        if (c == '{') {
            int *p1 = New(int *);
            *p1 = 1;
            PushStack(stack, p1);
        } else if (c == '[') {
            int *p2 = New(int *);
            *p2 = 2;
            PushStack(stack, p2);
        } else if (c == '(') {
            int *p3 = New(int *);
            *p3 = 3;
            PushStack(stack, p3);
        } else if (c == ')') {
            int *p4 = (int *) PopStack(stack);
            if (*p4 != 3) {
                puts("No!");
                return 0;
            }
        } else if (c == ']') {
            int *p5 = (int *) PopStack(stack);
            if (*p5 != 2) {
                puts("No!");
                return 0;
            }
        } else if (c == '}') {
            int *p6 = (int *) PopStack(stack);
            if (*p6 != 1) {
                puts("No!");
                return 0;
            }
        }
    }

    puts("Yes!");
    FreeStack(stack);
    return 0;
}