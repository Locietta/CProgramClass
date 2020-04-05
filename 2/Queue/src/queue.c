/*
 * File: queue.c
 * -------------
 * This file implements the queue.h abstraction using an array.
 */

#include <stdio.h>

#include "genlib.h"
#include "queue.h"

/* Exported entries */

/*
 * Function: NewQueue
 * ------------------
 * This function allocates and initializes the storage for a
 * new queue.
 */

queueADT NewQueue(void) {
    queueADT queue;

    queue = New(queueADT);
    queue->head = queue->tail = queue->len = 0;
    return (queue);
}

/*
 * Function: FreeQueue
 * -------------------
 * This function frees the queue storage.
 */

void FreeQueue(queueADT queue) {
    int upper = (queue->tail < queue->head) ? (queue->tail + MaxQueueSize) : queue->tail;

    for (int i = queue->head; i < upper; i++)
        FreeBlock(queue->array[i % MaxQueueSize]);
    FreeBlock(queue);
}

/*
 * Function: Enqueue
 * -----------------
 * This function adds a new element to the queue.
 */

void Enqueue(queueADT queue, void *obj) {
    if ((queue->tail + 1) % MaxQueueSize == queue->head) {
        Error("Enqueue called on a full queue");
    }
    queue->array[queue->tail++] = obj;
    queue->tail %= MaxQueueSize;
    queue->len++;
}

/*
 * Function: Dequeue
 * -----------------
 * This function removes and returns the data value at the
 * head of the queue.
 */

void *Dequeue(queueADT queue) {
    void *result;
    int i;

    if (queue->head == queue->tail) Error("Dequeue of empty queue");
    result = queue->array[queue->head++];
    queue->head %= MaxQueueSize;
    queue->len--;
    return (result);
}

/*
 * Function: QueueLength
 * ---------------------
 * This function returns the number of elements in the queue.
 */

int QueueLength(queueADT queue) {
    return (queue->len);
}
