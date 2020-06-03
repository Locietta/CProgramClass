/**
 * @file runtimeQuery.h
 * @author Locietta (https://github.com/Locietta)
 * @brief 用于计时(评估性能)的函数
 * @version 0.1
 * @date 2020-06-02
 * 
 */

#ifndef RUN_TIME_QUERY_H
#define RUN_TIME_QUERY_H

/**
 * @brief 区间计时函数 interval()
 * 
 * @return double 返回两次调用interval()的时间差，单位是秒，精确到微秒(大概)
 * @par 示例代码:
 * @code
 *  interval();
 *  // so something
 *  printf("It costed me %.6f seconds.\n", interval());
 * @endcode
 * @note 第一次调用返回当前时间的秒部分
 */
double interval(void);

#endif //(!defined RUN_TIME_QUERY_H)