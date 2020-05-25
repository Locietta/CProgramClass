#ifndef KOCH_H
#define KOCH_H

#include "coord.h"

/**
 * @brief 绘制科诺雪花
 *
 * @param a 0阶等边三角形的边长
 * @param n 科诺雪花的阶数
 */
void paintKoch(double a, int n, coord pos);

#endif // (!defined KOCH_H)