/**
 * @file calcio.h
 * @author Locietta (https://github.com/Locietta)
 * @brief 用于读入的一些自定义函数
 * 
 * (defined CALCIO_H)
 * 
 * @version 0.1
 * @date 2020-05-10
 * 
 */

#ifndef CALCIO_H
#define CALCIO_H

#include <stdio.h>
#include <ctype.h>

/**
 * @brief Function: Expr_gets 
 * 读入一行算式,空格和\t会被忽略,大写字母会被转换为小写字母
 * 
 * @param[out] s 读入的字符串存放在这里
 * @param[in] n 指明最大读取长度
 * @retval NULL 读入错误/到达文件结尾
 * @retval 其它字符指针 读入成功,返回字符存放首地址
 * @par 示例代码
 * @code
 *  while (Expr_gets(expr, 100)) { 
 *      //TODO 
 *  }
 * @endcode
 * @{
 */
 char* Expr_gets(char *s, int n);
 /** @}*/


#endif // (!defined CALCIO_H)