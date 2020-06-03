/**
 * @file sort.h
 * @author Locietta 
 * @brief 提供一个简单的接口用于升序排序数组
 * @details 试了一下泛型重载(
 * @date 2020-06-02
 * 
 */

#ifndef SORT_H
#define SORT_H

void sortIntegerArray(int arrayName[], int n, int increasing);

void sortDoubleArray(double arrayName[], int n, int increasing);

#define sortArray(arrayName, increasing) \
_Generic((arrayName), \
        int*: sortIntegerArray,\
        double*: sortDoubleArray\
)(arrayName, sizeof(arrayName)/(sizeof(*arrayName)), increasing)

#endif // (!defined SORT_H)