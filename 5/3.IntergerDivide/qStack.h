/**
 * @file queue.h
 * @author Locietta
 * @brief 抽象队列
 *
 * (defined QUEUE_H)
 * This header defines an abstract queue
 *
 * @version 0.1
 * @date 2020-05-09
 *
 *
 */
#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

/**
 * @brief NULL_QUEUE NULL队列
 * 一个无内容的队列,用于表示队列生成错误等
 */
#define NULL_QUEUE                                                             \
    ((queue){NULL, 0, -1, NULL, NULL, NULL, NULL, NULL, NULL, NULL})

/**
 * @brief POP_IN_EMPTY_QUEUE
 * 标识在空队列中pop的错误
 */
#define POP_IN_EMPTY_QUEUE 1

typedef struct Node *List;

typedef struct queue_info queue;

struct queue_info {
    List head;       // data
    int size;        // the number of elements in the queue
    size_t elemSize; // size of elements in the queue

    /* Public Function Prototypes */

    /**
     * Function: empty
     * Usage: myqueue.empty(&myqueue);
     * ---------------------------
     * This function returns 1 if the queue is empty,
     * otherwise, returns 0;
     */

    int (*empty)(queue *this);

    /**
     * Function: front
     * Usage: int n = *(int *) myqueue.front(&myqueue)
     * ---------------------------
     * This function return a pointer pointing to the first element
     * of the queue. It will NOT delete the first element, and you
     * can't use this pointer as a lvalue
     */

    const void *(*front)(queue *this);

    /**
     * Function: back
     * Usage: int n = *(int *) myqueue.back(&myqueue)
     * ---------------------------
     * This function return a pointer pointing to the last element
     * of the queue. It will NOT delete the last element, and you
     * can't use this pointer as a lvalue
     */

    const void *(*back)(queue *this);

    /**
     * Function: push
     * Usage: myqueue.push(&myqueue, &input);
     * ---------------------------
     * This function COPY the data pointed by a pointer into
     * a new element in the queue. Later, when variable "input" changes
     * its value, the element in queue will NOT change.
     * if everything is OK, it shall return 0,
     * otherwise, it returns a non-zero value indicating an error.
     */

    int (*push)(queue *this, const void *data);

    /**
     * Function: pop
     * Usage: myqueue.pop(&myqueue, &buffer);
     * ---------------------------
     * This function copy the first element into the datafield pointed
     * by &buffer. And then DELETE the first element in the queue.
     * In case of empty queue, it returns a non-zero value (POP_IN_EMPTY_QUEUE),
     * if everything is OK, it shall return 0
     */

    int (*pop)(queue *this, void *target);

    /**
     * Function: pop_back
     * Usage: myqueue.pop_back(&myqueue, &buffer);
     * ---------------------------
     * This function copy the last element into the datafield pointed
     * by &buffer. And then DELETE the last element in the queue.
     * In case of empty queue, it returns a non-zero value (POP_IN_EMPTY_QUEUE),
     * if everything is OK, it shall return 0
     */

    int (*pop_back)(queue *this, void *target);

    /**
     * Function: clear
     * Usage: myqueue.clear(&myqueue);
     * ---------------------------
     * This function clear all the data in the queue.
     * So the queue object can later be reused.
     * If you wanna dispose this queue, DON NOT use it,
     * use destory() instead.
     * (Or you'll get a leakage)
     */

    void (*clear)(queue *this);

    /**
     * Function: destory
     * Usage: myqueue.destory(&myqueue);
     * ---------------------------
     * This function free all the data in the queue.
     * You have to reinitialize it with newQueue() if you
     * wanna use it again.
     * When you want to dispose a queue, use destory() instead of clear()
     */

    void (*destory)(queue *this);
};

/* Construction Function */

/**
 * Function: queueInit
 * Usage: you should use newQueue() instead.
 * ---------------------------
 * This function initialize a queue.
 */

queue queueInit(size_t size);

/**
 * Macro: newQueue
 * Usage: myqueue = newQueue(double);
 * ---------------------------
 * This function initialize a new queue with given type.
 * In case of failure to build a queue, it returns a NULL_QUEUE
 */

#define newQueue(type) queueInit(sizeof(type))

#endif // (!defined QUEUE_H)