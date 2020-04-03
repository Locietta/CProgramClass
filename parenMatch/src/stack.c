#include <stdio.h>

#include "stack.h"

#define MAX_STACK_SIZE 1000

struct stackCDT {
    void *array[MAX_STACK_SIZE];
    int len;
};

stackADT NewStack(void) {
    stackADT stack = New(stackADT);
    stack->len = 0;
    return (stack);
}

void FreeStack(stackADT stack) {
    for (int i = 0; i < stack->len; ++i) {
        FreeBlock(stack->array[i]);
    }
    FreeBlock(stack);
}

void PushStack(stackADT stack, void *obj) {
    if (stack->len == MAX_STACK_SIZE) {
        Error("PushStack called on a full stack");
    }
    stack->array[stack->len++] = obj;
}

void *PopStack(stackADT stack) {

    if (stack->len == 0) {
        Error("PopStack of an empty stack");
    }
    return stack->array[--stack->len];
}

bool IsemptyStack(stackADT stack) {
    return !(stack->len);
}

void *TopStack(stackADT stack) {
    return stack->array[stack->len-1];
}