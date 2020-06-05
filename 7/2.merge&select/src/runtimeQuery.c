/**
 * @file runtimeQuery.c
 * @author Locietta (https://github.com/Locietta)
 * @brief 实现runtimeQuery.h
 * @details sys/time.h很好用，但是不确定是不是都有(好像只要gcc编译的话就没问题)...总之windows上写了替代方案
 * @version 0.2
 * @date 2020-06-05
 * 
 */

#include "runtimeQuery.h"

#define _FORCED_SYS_TIME_H

#if (defined(_WIN32) || defined(_WIN64)) && !defined(_FORCED_SYS_TIME_H)
#include <windows.h>

static LARGE_INTEGER clock_freq;
static void initQuery(void) {
    QueryPerformanceFrequency(&clock_freq); // init clock frequency
}

double interval(void) {
    static LARGE_INTEGER t0;
    static int firstCalled = 1;
    if (firstCalled) {
        initQuery();
        firstCalled = 0;
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