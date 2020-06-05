/**
 * @file msort.c
 * @author Locietta
 * @brief msort.h的实现
 * @date 2020-06-03
 *
 */

#include "msort.h"
#include "runtimeQuery.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <pthread.h>
#include <windows.h>
/* Private function prototypes */

// 交换两数
#define swap(a, b)                                                             \
    do {                                                                       \
        int temp = a;                                                          \
        a = b;                                                                 \
        b = temp;                                                              \
    } while (0)

static void select_sort(int data[], int left, int right);

static void merge(int arr[], int tempArr[], int left, int mid, int right);

static void mergeSort(int arr[], int tempArr[], int left, int right);

static void *sort_thr(void *arg);

static void mergeSort_multi(int *left, int *right, int tempArr[]);

static void merge_multi(int *left, int *mid, int *right, int tempArr[]);

static void select_sort_multi(int *left, int *right);

/* Implementations */

/* Public Functions */

void mSort(int data[], int len) {
    int *tempArr = (int *) malloc(len * sizeof(int));
    mergeSort(data, tempArr, 0, len - 1);
    free(tempArr);
}

void selectSort(int data[], int len) {
    select_sort(data, 0, len - 1);
}

void sortIntegerArray(int data[], int len) {
    mSort(data, len);
}

/* Private Functions */

static void mergeSort(int arr[], int tempArr[], int left, int right) {
    if (right - left > CUTOFF) {
        int mid = left + (right - left) / 2; // 溢出回避
        mergeSort(arr, tempArr, left, mid);
        mergeSort(arr, tempArr, mid + 1, right);
        merge(arr, tempArr, left, mid, right);
    } else {
        select_sort(arr, left, right);
    }
}

static void merge(int arr[], int tempArr[], int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;
    while (i != mid + 1 && j != right + 1) {
        tempArr[k++] = (arr[i] > arr[j]) ? arr[j++] : arr[i++];
    }
    while (i != mid + 1) tempArr[k++] = arr[i++];
    while (j != right + 1) tempArr[k++] = arr[j++];
    for (i = left; i <= right; ++i) arr[i] = tempArr[i];
    // memcpy(arr + left, tempArr, size); // 居然比直接循环慢...
}

static void select_sort(int data[], int left, int right) {
    for (int i = left; i < right; ++i) {
        int tag = i;
        for (int j = i + 1; j <= right; ++j) {
            if (data[j] < data[tag]) {
                tag = j;
            }
        }
        if (tag != i) {
            swap(data[tag], data[i]);
        }
    }
}

/* multi-thread */

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int max_threads;
static int n_threads;

void mSort_multi(int data[], int len) {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    int n_cpus = si.dwNumberOfProcessors;
    if (n_cpus > 0) {
        max_threads = n_cpus * 2;
    } else {
        max_threads = 8;
    }
    printf("%d core %d threads\n", n_cpus, 2 * n_cpus);

    interval();
    int *tempArr = (int *) malloc(len * sizeof(int));

    pthread_t thread;
    int **param = malloc(3 * sizeof(int *));
    param[0] = data;
    param[1] = data + len - 1;
    param[2] = tempArr;
    n_threads = 1;
    pthread_create(&thread, NULL, sort_thr, param);

    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

    free(tempArr);
}

static void *sort_thr(void *arg) {
    int **par = (int **) arg;
    mergeSort_multi(par[0], par[1], par[2]);
    free(arg);
    pthread_mutex_lock(&mutex);
    n_threads -= 1;
    if (n_threads == 0) pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

static void mergeSort_multi(int *left, int *right, int tempArr[]) {
    if (right - left > CUTOFF) {
        int *mid = left + (right - left) / 2;
        mergeSort_multi(left, mid, tempArr);

        if (right - left > 10000 && n_threads < max_threads) {
            pthread_t thread;
            int **param = malloc(3 * sizeof(int *));
            param[0] = mid+1;
            param[1] = right;
            param[2] = tempArr;
            pthread_mutex_lock(&mutex);
            n_threads += 1;
            pthread_mutex_unlock(&mutex);
            pthread_create(&thread, NULL, sort_thr, param);
        }
        merge_multi(left, mid, right, tempArr);
    } else {
        select_sort_multi(left, right);
    }
}

static void merge_multi(int *left, int *mid, int *right, int tempArr[]) {
    int *i = left, *j = mid + 1, k = 0;
    while (i != mid + 1 && j != right + 1) {
        tempArr[k++] = (*i > *j) ? *j++ : *i++;
    }
    while (i != mid + 1) tempArr[k++] = *i++;
    while (j != right + 1) tempArr[k++] = *j++;
    for (i = left, k = 0; i <= right;) *i++ = tempArr[k++];
}

static void select_sort_multi(int *left, int *right) {
    for (int *pi = left; pi < right; ++pi) {
        int *tag = pi;
        for (int *pj = pi + 1; pj <= right; ++pj) {
            if (*pj < *tag) {
                tag = pj;
            }
        }
        if (tag != pi) {
            swap(*tag, *pi);
        }
    }
}