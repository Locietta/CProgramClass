#include "queue.h"
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

static int queueEmpty(queue *this);

static const void *queueFront(queue *this);

static const void *queueBack(queue *this);

static int queuePush(queue *this, const void *data);

static int queuePop(queue *this, void *target);

static void queueClear(queue *this);

static void queueDestory(queue *this);

/* Function implementations */

/* Public functions */

queue queueInit(size_t size) {
    queue newqueue;
    newqueue.head = (List) calloc(1, sizeof(Node));
    newqueue.elemSize = size;
    newqueue.head->data = malloc(newqueue.elemSize);

    if (newqueue.head && newqueue.head->data) {
        newqueue.head->prev = newqueue.head;
        newqueue.head->next = newqueue.head;
    } else {
        puts("Queue initalization falied");
        return NULL_QUEUE;
    }

    newqueue.size = 0;
    newqueue.empty = queueEmpty;
    newqueue.front = queueFront;
    newqueue.back = queueBack;
    newqueue.push = queuePush;
    newqueue.pop = queuePop;
    newqueue.clear = queueClear;
    newqueue.destory = queueDestory;

    return newqueue;
}

static int queueEmpty(queue *this) {
    return !this->size;
}

static const void *queueFront(queue *this) {
    return (this->size) ? this->head->data : NULL;
}

static const void *queueBack(queue *this) {
    return (this->size) ? this->head->prev->data : NULL;
}

static int queuePush(queue *this, const void *data) {
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
    } else { //empty queue
        memmove(this->head->data, data, this->elemSize);
    }
    ++this->size;
    return 0;
}

static int queuePop(queue *this, void *target) {
    if (this->size) {
        if (target) { // target != NULL
            memmove(target, this->head->data, this->elemSize);
        }

        if (this->size > 1) {
            List temp = this->head;
            this->head->prev->next = this->head->next;
            this->head->next->prev = this->head->prev;
            this->head = this->head->next;
            freeNode(temp);
        }
        --this->size;
    } else {
        // puts("Pop in an empty queue");
        return POP_IN_EMPTY_QUEUE;
    }
    return 0;
}

static void queueClear(queue *this) {
    while (this->head->prev != this->head) {
        List temp = this->head->prev;
        this->head->prev = this->head->prev->prev;
        this->head->prev->next = this->head;
        freeNode(temp);
    }
    this->size = 0;
}

static void queueDestory(queue *this) {
    while (this->head->prev != this->head) {
        List temp = this->head->prev;
        this->head->prev = this->head->prev->prev;
        this->head->prev->next = this->head;
        freeNode(temp);
    }
    freeNode(this->head);
}
