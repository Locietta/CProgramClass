#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node { // Node of circular linked list
    List prev, next;
    void *data;
} Node;

/* Private Functions Prototypes */

#define freeNode(node)    \
    do {                  \
        free(node->data); \
        free(node);       \
    } while (0)

/* Public Functions Prototypes */

static int stackEmpty(stack *this);

static const void *stackTop(stack *this);

static int stackPop(stack *this, void *target);

static int stackPush(stack *this, const void *data);

static void stackClear(stack *this);

static void stackDestory(stack *this);

/* Function implementations */

/* Public functions */

stack stackInit(size_t size) {
    stack newstack;
    newstack.head = (List) calloc(1, sizeof(Node));
    newstack.elemSize = size;
    newstack.head->data = malloc(newstack.elemSize);

    if (newstack.head && newstack.head->data) {
        newstack.head->prev = newstack.head;
        newstack.head->next = newstack.head;
    } else {
        puts("Stack initalization falied");
        return NULL_STACK;
    }

    newstack.size = 0;
    newstack.empty = stackEmpty;
    newstack.top = stackTop;
    newstack.pop = stackPop;
    newstack.push = stackPush;
    newstack.clear = stackClear;
    newstack.destory = stackDestory;

    return newstack;
}

static int stackEmpty(stack *this) {
    return !this->size;
}

static const void *stackTop(stack *this) {
    return (this->size) ? this->head->prev->data : NULL;
}

static int stackPop(stack *this, void *target) {
    if (this->size) {
        if (target) { // target != NULL
            memmove(target, this->head->prev->data, this->elemSize);
        }

        if (this->size > 1) {
            List temp = this->head->prev;
            this->head->prev = this->head->prev->prev;
            this->head->prev->next = this->head;
            freeNode(temp);
        }
        --this->size;
    } else {
        // puts("Pop in an empty stack");
        return POP_IN_EMPTY_STACK;
    }
    return 0;
}

static int stackPush(stack *this, const void *data) {
    if (this->size) {
        List newNode = (List) calloc(1, sizeof(Node));
        if (newNode == NULL) {
            // puts("new node allocation failed");
            return 1;
        }
        newNode->data = malloc(this->elemSize);
        if (newNode->data == NULL) {
            // puts("new datafield allocation failed");
            return 2;
        }
        memmove(newNode->data, data, this->elemSize);

        newNode->next = this->head;
        newNode->prev = this->head->prev;
        newNode->prev->next = newNode;
        newNode->next->prev = newNode;
    } else { // empty stack
        memmove(this->head->data, data, this->elemSize);
    }
    ++this->size;
    return 0;
}

static void stackClear(stack *this) {
    while (this->head->prev != this->head) {
        List temp = this->head->prev;
        this->head->prev = this->head->prev->prev;
        this->head->prev->next = this->head;
        freeNode(temp);
    }
    this->size = 0;
}

static void stackDestory(stack *this) {
    while (this->head->prev != this->head) {
        List temp = this->head->prev;
        this->head->prev = this->head->prev->prev;
        this->head->prev->next = this->head;
        freeNode(temp);
    }
    freeNode(this->head);
}
