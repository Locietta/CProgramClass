#include "qsort.h"
#include "runtimeQuery.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_SORTED (10 * 10000)

void init(void) {
    srand((unsigned) interval());
    FILE *fp;
    fopen_s(&fp, "random_int", "w+");
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
    // init();
    FILE *raw, *res, *res1, *res2, *res3;
    fopen_s(&raw, "random_int", "r");

    int data[NUM_SORTED];

    // readArr(raw, data, NUM_SORTED);
    // fopen_s(&res, "result_insert", "w");
    // interval();
    // isort(data, NUM_SORTED);
    // fprintf(res, "insertSort %d int costs %.6fs\nResult:", NUM_SORTED, interval());
    // printArr(res, data, NUM_SORTED);
    // fflush(res);
    // fclose(res);
    // fclose(raw);

    readArr(raw, data, NUM_SORTED);
    fopen_s(&res1, "result_q", "w");
    interval();
    quicksort(data, NUM_SORTED);
    fprintf(res1, "ordinary quicksort %d int costs %.6fs\nResult:", NUM_SORTED, interval());
    printArr(res1, data, NUM_SORTED);
    fflush(res1);
    fclose(res1);
    fclose(raw);

    // readArr(raw, data, NUM_SORTED);
    // fopen_s(&res2, "result_insertq", "w");
    // interval();
    // qsort_insert(data, NUM_SORTED);
    // fprintf(res2, "insert quicksort %d int costs %.6fs\nResult:", NUM_SORTED, interval());
    // printArr(res2, data, NUM_SORTED);
    // fflush(res2);
    // fclose(res2);
    // fclose(raw);

    // readArr(raw, data, NUM_SORTED);
    // fopen_s(&res3, "result_multi", "w");
    // interval();
    // qsort_multi(data, NUM_SORTED);
    // fprintf(res3, "multi-thread quicksort %d int costs %.6fs\nResult:", NUM_SORTED, interval());
    // printArr(res3, data, NUM_SORTED);
    // fflush(res3);
    // fclose(res3);
    // fclose(raw);

    return 0;
}
