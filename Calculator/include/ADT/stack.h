/**
 * @file stack.h
 * @author Locietta
 * @brief 抽象栈
 * 
 * (defined STACK_H)
 * This header defines an abstract stack
 * 
 * @version 0.1
 * @date 2020-05-08
 * 
 * 
 */

#ifndef STACK_H
#define STACK_H

#include <stddef.h>

/**
 * @brief NULL_QUEUE NULL栈
 * 一个无内容的栈,用于表示栈生成错误等
 */
#define NULL_STACK ((stack){NULL, 0, -1, NULL, NULL, NULL, NULL, NULL, NULL})

/**
 * @brief POP_IN_EMPTY_STACK
 * 标识在空栈中pop的错误
 */
#define POP_IN_EMPTY_STACK 1

typedef struct Node *List;

typedef struct stack_info stack;
struct stack_info {  // container
    List head;       // data
    int size;        // the number of elements in the stack
    size_t elemSize; // size of elements in the stack

    /* Public Function Prototypes */

    /**
     * Function: empty
     * Usage: mystack.empty(&mystack);
     * ---------------------------
     * This function returns 1 if the stack is empty,
     * otherwise, returns 0;
     */

    int (*empty)(stack *this);

    /**
     * Function: top
     * Usage: int n = *(int *) mystack.top(&mystack);
     * ---------------------------
     * This function return a pointer pointing to the last element
     * of the stack. It will NOT delete the last element, and you 
     * can't use this pointer as a lvalue.
     */

    const void *(*top)(stack *this);

    /**
     * Function: pop
     * Usage: mystack.pop(&mystack, &buffer);
     * ---------------------------
     * This function copy the last element into the datafield pointed
     * by &buffer. And then DELETE the last element in the stack.
     * In case of empty stack, it returns a non-zero value (POP_IN_EMPTY_STACK),
     * if everything is OK, it shall return 0
     */

    int (*pop)(stack *this, void *target);

    /**
     * Function: push
     * Usage: mystack.push(&mystack, &input);
     * ---------------------------
     * This function COPY the data pointed by a pointer into
     * a new element in the stack. Later, when variable "input" changes
     * its value, the element in stack will NOT change.
     * if everything is OK, it shall return 0, 
     * otherwise, it returns a non-zero value indicating an error. 
     */

    int (*push)(stack *this, const void *data);

    /**
     * Function: clear
     * Usage: mystack.clear(&mystack);
     * ---------------------------
     * This function clear all the data in the stack.
     * So the stack object can later be reused.
     * If you wanna dispose this stack, DON NOT use it,
     * use destory() instead.
     * (Or you'll get a leakage)
     */

    void (*clear)(stack *this);

    /**
     * Function: destory
     * Usage: mystack.destory(&mystack);
     * ---------------------------
     * This function free all the data in the stack.
     * You have to reinitialize it with newStack() if you
     * wanna use it again.
     * When you want to dispose a stack, use destory() instead of clear()
     */

    void (*destory)(stack *this);
};

/* Construction Function */

/**
 * Function: stackInit
 * Usage: you should use newStack() instead.
 * ---------------------------
 * This function initialize a stack.
 */

stack stackInit(size_t size);

/**
 * Macro: newStack
 * Usage: mystack = newStack(double);
 * ---------------------------
 * This function initialize a new stack with given type.
 * In case of failure to build a stack, it returns a NULL_STACK
 */

#define newStack(type) stackInit(sizeof(type))

#endif // (!defined STACK_H)