/**
 * @file sort.c
 * @author Locietta
 * @brief qsort.h的实现
 * @details 就是升序快排
 * @date 2020-06-03
 *
 */

#include "qsort.h"
#include "runtimeQuery.h"

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* Private function prototypes */

// 交换两数
#define swap(a, b)    \
    do {              \
        int temp = a; \
        a = b;        \
        b = temp;     \
    } while (0)

// 快速排序三数
#define sort3fast(a, b, c)  \
    if (b < a) {            \
        if (c < a) {        \
            if (c < b) {    \
                swap(a, c); \
            } else {        \
                int h = a;  \
                a = b;      \
                b = c;      \
                c = h;      \
            }               \
        } else {            \
            swap(a, b);     \
        }                   \
    } else {                \
        if (c < b) {        \
            if (c < a) {    \
                int h = c;  \
                c = b;      \
                b = a;      \
                a = h;      \
            } else {        \
                swap(b, c); \
            }               \
        }                   \
    }

static int partition(int arr[], int low, int high);

static int partition1(int arr[], int low, int high);

static void insertSort(int arr[], int low, int high);

static void qusort(int arr[], int low, int high);

static void qusort_insert(int arr[], int low, int high);

static void *sort_thr(void *arg);

static void qusort_multi(int *left, int *right);

static void insert2Sort(int *left, int *right);

/* Implementations */

/* Single-thread Public Functions */

void quicksort(int data[], int len) {
    qusort(data, 0, len - 1);
}

void qsort_insert(int data[], int len) {
    qusort_insert(data, 0, len - 1);
}

void isort(int data[], int len) {
    insertSort(data, 0, len - 1);
}

/* Single-thread Private Functions */

static void qusort_insert(int arr[], int low, int high) {

    int l1 = low, r1 = high, l2, r2; // 循环换迭代，减少一个递归
    while (r1 - l1 >= 50) {
        int p = partition1(arr, l1, r1);
        if ((l1 + r1) / 2 > p) {
            l2 = l1;
            r2 = p - 1;
            l1 = p + 1;
        } else {
            r2 = r1;
            l2 = p + 1;
            r1 = p - 1;
        }

        qusort_insert(arr, l2, r2);
    }
    insertSort(arr, l1, r1);
}

static void qusort(int arr[], int low, int high) {
    if (low < high) {
        int p = partition(arr, low, high);

        qusort(arr, low, p - 1);
        qusort(arr, p + 1, high);
    }
}

static int partition(int arr[], int low, int high) {
    int pivot = arr[high], i = low;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            swap(arr[i], arr[j]);
            i++;
        }
    }
    swap(arr[i], arr[high]);
    return i;
}

static int partition1(int arr[], int low, int high) {
    int l = low + 1, r = high;
    int mid = low + (high - low) / 2;

    int pivot = arr[mid];
    arr[mid] = arr[l];
    sort3fast(arr[low], pivot, arr[high]);
    arr[l] = pivot;

    while (1) {
        while (arr[l] < pivot) {
            ++l;
        }
        do {
            --r;
        } while (arr[r] > pivot);
        if (l >= r) break;
        swap(arr[l], arr[r]);
    }
    arr[low + 1] = arr[r];
    arr[r] = pivot;
    return r;
}

static void insertSort(int arr[], int low, int high) {

    for (int i = low + 1; i <= high; ++i) { // 第一个不用排
        if (arr[i] < arr[i - 1]) {
            int temp = arr[i], j;
            arr[i] = arr[i - 1];
            for (j = i - 2; j >= low; --j) {
                if (temp < arr[j]) {
                    arr[j + 1] = arr[j];
                } else {
                    break;
                }
            }
            arr[j + 1] = temp;
        }
    }
}

/* Multi-thread Function */

// typedef union par {
//     int *arr;
//     int num;
// } PAR;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int max_threads;
static int n_threads;

void qsort_multi(int data[], int len) {
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
    pthread_t thread;
    int **param = malloc(2 * sizeof(int *));
    param[0] = data;
    param[1] = data + len - 1;
    n_threads = 1;
    pthread_create(&thread, NULL, sort_thr, param);

    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);
}

static void *sort_thr(void *arg) {
    int **par = (int **) arg;
    qusort_multi(par[0], par[1]);
    free(arg);
    pthread_mutex_lock(&mutex);
    n_threads -= 1;
    if (n_threads == 0) pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

static void partition_multi(int *left0, int *right0, int **l1, int **r1, int **l2,
                       int **r2) {

    int *left = left0 + 1;
    int *right = right0;

    int *mid = left0 + (right0 - left0) / 2;
    int piv = *mid;
    *mid = *left;
    sort3fast(*left0, piv, *right0);
    *left = piv;

    while (1) {
        do
            left += 1;
        while (*left < piv);
        do
            right -= 1;
        while (*right > piv);
        if (left >= right) break;
        swap(*left, *right);
    }
    *(left0 + 1) = *right;
    *right = piv;

    if (right < mid) {
        *l1 = left0;
        *r1 = right - 1;
        *l2 = right + 1;
        *r2 = right0;
    } else {
        *l1 = right + 1;
        *r1 = right0;
        *l2 = left0;
        *r2 = right - 1;
    }
}

static void qusort_multi(int *left, int *right) {

    while (right - left >= 50) {
        int *l, *r;
        partition_multi(left, right, &l, &r, &left, &right);

        if (right - left > 100000 && n_threads < max_threads) {
            pthread_t thread;
            int **param = malloc(2 * sizeof(int *));
            param[0] = left;
            param[1] = right;
            pthread_mutex_lock(&mutex);
            n_threads += 1;
            pthread_mutex_unlock(&mutex);
            pthread_create(&thread, NULL, sort_thr, param);
            left = l;
            right = r;
        } else {
            qusort_multi(l, r);
        }
    }
    insert2Sort(left, right);
}

static void insert2Sort(int *left, int *right) {

    for (int *pi = left + 1; pi <= right; ++pi) { // 第一个不用排
        if (*pi < *(pi - 1)) {
            int temp = *pi, *pj;
            *pi = *(pi - 1);
            for (pj = pi - 2; pj >= left; --pj) {
                if (temp < *pj) {
                    *(pj + 1) = *pj;
                } else {
                    break;
                }
            }
            *(pj + 1) = temp;
        }
    }
}