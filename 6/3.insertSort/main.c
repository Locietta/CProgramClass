#include <stdio.h>
#include <stdlib.h>
#include "runtimeQuery.h"
#include "sort.h"

#define NUM_SORTED 10000

int test1(int data[], int len);
int test2(double data[], int len);

int main(void) {

    /// initialize random number
    int data1[NUM_SORTED];
    double data2[NUM_SORTED];
    srand((unsigned int) interval());
    for (int i = 0; i < NUM_SORTED; i++) { // random int
        data1[i] = rand();
    }
    for (int i = 0; i < NUM_SORTED; i++) { // random double in [-5,5]
        data2[i] = rand() * (10.0 / RAND_MAX) - 5.0;
    }

    /// sort numbers
    interval();
    sortArray(data1, 1);
    printf("sort integers using %.6fs\n",interval());
    if (!test1(data1, NUM_SORTED)) {
        puts("Integer sort success!");
    }

    interval();
    sortArray(data2, 0);
    printf("sort doubles using %.6fs\n",interval());
    if (!test2(data2, NUM_SORTED)){
        puts("Double sort success!");
    }

    return 0;
}

int test1(int data[], int len) {
    for (int i = 1; i < len; i++) {
        if (data[i] < data[i - 1]) {
            printf("ERROR\n");
            return 1;
        }
    }
    return 0;
}

int test2(double data[], int len) {
    for (int i = 1; i < len; i++) {
        if (data[i] > data[i - 1]) {
            printf("ERROR\n");
            return 1;
        }
    }
    return 0;
}