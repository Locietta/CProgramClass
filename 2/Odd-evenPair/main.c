#include <stdio.h>

#include "genlib.h"
#include "queue.h"
#include "simpio.h"

int main(void) {
    queueADT queue_odd = NewQueue(), queue_even = NewQueue();
    int temp = 0;
    printf("Input>");
    while (scanf("%d", &temp) != EOF) {
        // int *p_int = (int *) malloc(sizeof(int));
        // *p_int = temp;
        if (temp & 1) {
            if (QueueLength(queue_even) > 0) {
                int *p = (int *) Dequeue(queue_even);
                printf("(%d, %d)\n", temp, *p);
                FreeBlock(p);
            } else {
                int *p_int = (int *) malloc(sizeof(int));
                *p_int = temp;
                Enqueue(queue_odd, p_int);
            }
        } else {
            if (QueueLength(queue_odd) > 0) {
                int *p = (int *) Dequeue(queue_odd);
                printf("(%d, %d)\n", *p, temp);
                FreeBlock(p);
            } else {
                int *p_int = (int *) malloc(sizeof(int));
                *p_int = temp;
                Enqueue(queue_even, p_int);
            }
        }
        printf("Input>");
    }
    return 0;
}