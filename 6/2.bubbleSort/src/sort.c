/**
 * @file sort.c
 * @author Locietta
 * @brief sort.h的实现
 * @details 按照题目要求用的是冒泡
 * @date 2020-06-02
 *
 */

#include "sort.h"

void sortIntegerArray(int arrayName[], int n, int increasing) {
    if (increasing) {
        for (int i = 0; i < n - 1; ++i) { // 最后一个不用排
            for (int j = n - 1; j > i; --j) {
                if (arrayName[j - 1] > arrayName[j]) {
                    int temp = arrayName[j - 1];
                    arrayName[j - 1] = arrayName[j];
                    arrayName[j] = temp;
                }
            }
        }
    } else {
        for (int i = 0; i < n - 1; ++i) { 
            for (int j = n - 1; j > i; --j) {
                if (arrayName[j - 1] < arrayName[j]) {
                    int temp = arrayName[j - 1];
                    arrayName[j - 1] = arrayName[j];
                    arrayName[j] = temp;
                }
            }
        }
    }
}

void sortDoubleArray(double arrayName[], int n, int increasing) {
    if (increasing) {
        for (int i = 0; i < n - 1; ++i) { // 最后一个不用排
            for (int j = n - 1; j > i; --j) {
                if (arrayName[j - 1] > arrayName[j]) {
                    double temp = arrayName[j - 1];
                    arrayName[j - 1] = arrayName[j];
                    arrayName[j] = temp;
                }
            }
        }
    } else {
        for (int i = 0; i < n - 1; ++i) { 
            for (int j = n - 1; j > i; --j) {
                if (arrayName[j - 1] < arrayName[j]) {
                    double temp = arrayName[j - 1];
                    arrayName[j - 1] = arrayName[j];
                    arrayName[j] = temp;
                }
            }
        }
    }
}