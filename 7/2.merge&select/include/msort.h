/**
 * @file msort.h
 * @author Locietta
 * @brief 归并排序函数的接口
 * @details 升序
 * @date 2020-06-03
 *
 */

#ifndef MSORT_H
#define MSORT_H

#define CUTOFF 33

void sortIntegerArray(int data[], int len);

void mSort(int data[], int len);

void mSort_multi(int data[], int len);

void selectSort(int data[], int len);

#endif // (!defined MSORT_H)