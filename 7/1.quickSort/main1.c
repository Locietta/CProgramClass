#include "qsort.h"
#include "runtimeQuery.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_SORTED (400 * 1000000)

void init(int *data, int size) {
    for (int i = 0; i < size; i++) { // generate test data randomly
        data[i] = rand();
    }
}

int test(int *data, int len) {
    for (int i = 1; i < len; i++) {
        if (data[i] < data[i - 1]) {
            printf("ERROR\n");
            return 1;
        }
    }
    return 0;
}

int data[NUM_SORTED];

int main(void) {
    srand((unsigned) interval());
    init(data, NUM_SORTED);
    printf("Sorting %d million numbers with Multi-thread Quicksort ...\n",
           NUM_SORTED / 1000000);
    qsort_multi(data, NUM_SORTED);
    printf("%.6fs\n", interval());
    if (!test(data, NUM_SORTED)) {
        puts("Sort success!!!");
    }

    // init(data, NUM_SORTED);
    // printf("Sorting %d million numbers with insert Quicksort ...\n",
    //        NUM_SORTED / 1000000);
    // qsort_insert(data, NUM_SORTED);
    // printf("%.6fs\n", interval());
    // if (!test(data, NUM_SORTED)) {
    //     puts("Sort success!!!");
    // }

    // init(data, NUM_SORTED);
    // printf("Sorting %d million numbers with Quicksort ...\n",
    //        NUM_SORTED / 1000000);
    // quicksort(data, NUM_SORTED);
    // printf("%.6fs\n", interval());
    // if (!test(data, NUM_SORTED)) {
    //     puts("Sort success!!!");
    // }

    // init(data, NUM_SORTED);
    // printf("Sorting %d million numbers with insertSort ...\n",
    //        NUM_SORTED / 1000000);
    // isort(data, NUM_SORTED);
    // printf("%.6fs\n", interval());
    // if (!test(data, NUM_SORTED)) {
    //     puts("Sort success!!!");
    // }

    return 0;
}
