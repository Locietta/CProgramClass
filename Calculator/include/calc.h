/**
 * @file calc.h
 * @author Locietta (https://github.com/Locietta)
 * @brief 提供表达式计算函数的接口
 * 
 * (defined CALC_H)
 * This header defines several functions needed for calculation
 * 
 * @version 0.1
 * @date 2020-05-10
 * 
 */

#ifndef CALC_H
#define CALC_H

#include "error.h"

/**
 * Function: Calc
 * Usage: Calc(expr, &result);
 * ---------------------------
 * This function calculate the expression given by 'expr'
 * its result will be put into the 'result'
 * If everything is OK, it shall return 0
 * Otherwise, it returns a non-zero value indicating errors
 */

ERROR_INFO Calc(char *expr, double *result);


#endif // (!defined CALC_H)