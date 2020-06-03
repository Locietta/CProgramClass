/**
 * @file runtimeQuery.c
 * @author Locietta (https://github.com/Locietta)
 * @brief 实现runtimeQuery.h
 * @details sys/time.h很好用，但是不确定是不是都有(好像只要gcc编译的话就没问题)...总之windows上用了替代方案
 * @date 2020-06-02
 * 
 */

#include "runtimeQuery.h"

#if defined(_WIN32) || defined(_WIN64) 
#include <windows.h>

static LARGE_INTEGER clock_freq;
static void initQuery(void) {
    QueryPerformanceFrequency(&clock_freq); // init clock frequency
}

double interval(void) {
    static LARGE_INTEGER t0;
    static int timesCalled = 0;
    if (!timesCalled) {
        initQuery();
    }
    long long h = t0.QuadPart;
    QueryPerformanceCounter(&t0);
    return (double) (t0.QuadPart - h) / clock_freq.QuadPart;
}

#else 
// #elif defined(__linux__)
#include <sys/time.h>

double interval(void) {

    static double t0;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    double h = t0;
    t0 = tv.tv_sec + tv.tv_usec / 1000000.0;
    return t0 - h;
}

#endif 