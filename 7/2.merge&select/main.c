#include "msort.h"
#include "runtimeQuery.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_SORTED (100000)

#define USE_MERGE

// #define MULTI

void init(void) {
    srand((unsigned) interval());
    FILE *fp;
    fopen_s(&fp, "random_int", "w");
    for (int i = 0; i < NUM_SORTED; ++i) {
        fprintf(fp, " %d", rand());
    }
    fclose(fp);
}

void readArr(FILE *stream, int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        fscanf(stream, "%d", arr + i);
    }
}

void printArr(FILE *stream, int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        fprintf(stream, " %d", arr[i]);
    }
}

int main(void) {
    init();
    FILE *raw, *res, *time;
    fopen_s(&raw, "random_int", "r");
    fopen_s(&res, "result", "w");
    fopen_s(&time, "time", "a");
    int data[NUM_SORTED];

#ifdef USE_MERGE
#ifndef MULTI
    readArr(raw, data, NUM_SORTED);
    fclose(raw);
    interval();
    mSort(data, NUM_SORTED);
    fprintf(time, "modified mergeSort with CUTOFF %d sort %d int costs %.6fs\n",
            CUTOFF, NUM_SORTED, interval());
    printArr(res, data, NUM_SORTED);
    fclose(res);
#else
    readArr(raw, data, NUM_SORTED);
    fclose(raw);
    interval();
    mSort(data, NUM_SORTED);
    fprintf(time, "multi-thread mergeSort with CUTOFF %d sort %d int costs %.6fs\n",
            CUTOFF, NUM_SORTED, interval());
    printArr(res, data, NUM_SORTED);
    fclose(res);
#endif
#else
    readArr(raw, data, NUM_SORTED);
    fclose(raw);
    interval();
    selectSort(data, NUM_SORTED);
    fprintf(time, "selecSort %d int costs %.6fs\n", NUM_SORTED, interval());
    printArr(res, data, NUM_SORTED);
    fclose(res);
#endif
    return 0;
}
