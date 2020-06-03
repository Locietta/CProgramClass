/**
 * @file qsort.h
 * @author Locietta 
 * @brief 提供一个简单的快排接口用于排序int
 * @details 升序
 * @date 2020-06-02
 * 
 */

#ifndef QSORT_H
#define QSORT_H

// 纯快排
void quicksort(int data[], int len);

// 插入快排
void qsort_insert(int data[], int len);

// 多线程快排
void qsort_multi(int data[], int len);

// 纯插排
void isort(int data[], int len);

#endif // (!defined QSORT_H)