/**
 * @file sort.c
 * @author Locietta
 * @brief sort.h的实现
 * @details 按照题目要求用的是插排
 * @date 2020-06-02
 *
 */

#include "sort.h"

void sortIntegerArray(int arrayName[], int n, int increasing) {
    if (increasing) {
        for (int i = 1; i < n; ++i) { // 第一个不用排
            if (arrayName[i] < arrayName[i - 1]) {
                int temp = arrayName[i], j;
                arrayName[i] = arrayName[i - 1];
                for (j = i - 2; j >= 0; --j) {
                    if (temp < arrayName[j]) {
                        arrayName[j + 1] = arrayName[j];
                    } else {
                        break;
                    }
                }
                arrayName[j + 1] = temp;
            }
        }
    } else {
        for (int i = 1; i < n; ++i) { // 第一个不用排
            if (arrayName[i] > arrayName[i - 1]) {
                int temp = arrayName[i], j;
                arrayName[i] = arrayName[i - 1];
                for (j = i - 2; j >= 0; --j) {
                    if (temp > arrayName[j]) {
                        arrayName[j + 1] = arrayName[j];
                    } else {
                        break;
                    }
                }
                arrayName[j + 1] = temp;
            }
        }
    }
}

void sortDoubleArray(double arrayName[], int n, int increasing) {
    if (increasing) {
        for (int i = 1; i < n; ++i) { // 第一个不用排
            if (arrayName[i] < arrayName[i - 1]) {
                int temp = arrayName[i], j;
                arrayName[i] = arrayName[i - 1];
                for (j = i - 2; j >= 0; --j) {
                    if (temp < arrayName[j]) {
                        arrayName[j + 1] = arrayName[j];
                    } else {
                        break;
                    }
                }
                arrayName[j + 1] = temp;
            }
        }
    } else {
        for (int i = 1; i < n; ++i) { 
            if (arrayName[i] > arrayName[i - 1]) {
                int temp = arrayName[i], j;
                arrayName[i] = arrayName[i - 1];
                for (j = i - 2; j >= 0; --j) {
                    if (temp > arrayName[j]) {
                        arrayName[j + 1] = arrayName[j];
                    } else {
                        break;
                    }
                }
                arrayName[j + 1] = temp;
            }
        }
    }
}