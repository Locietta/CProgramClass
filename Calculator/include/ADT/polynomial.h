

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "linkedList.h"

/**
 * @brief 结构: 多项式的一项 PolynomialT
 * @details coeff:系数 power:指数
 */
typedef struct _polynomial {
	double coeff, power;
} *PolynomialT;/*指针类型*/

/**
 * @brief 创建一个多项式链表
 * 
 * @param format 标准化过的多项式字符串
 * @return linkedlistADT 返回创建的多项式链表
 */

linkedlistADT CreatePolynomial(string format);/*创建一个多项式链表*/
/**
 * @brief 
 * 
 * @param head 
 * @return linkedlistADT 
 */
linkedlistADT ClonePolynomial(linkedlistADT head);/*克隆一个多项式*/
/**
 * @brief 
 * 
 * @param h1 
 * @param h2 
 * @return linkedlistADT 
 */
linkedlistADT AddPolynomial(linkedlistADT h1, linkedlistADT h2);/*加*/
/**
 * @brief 
 * 
 * @param h1 
 * @param h2 
 * @return linkedlistADT 
 */
linkedlistADT SubPolynomial(linkedlistADT h1, linkedlistADT h2);/*减*/
/**
 * @brief 
 * 
 * @param head 
 * @param coeff 
 * @param power 
 * @return linkedlistADT 
 */
linkedlistADT MultiplyNode(linkedlistADT head, double coeff, double power);/*多项式乘以一项*/
/**
 * @brief 
 * 
 * @param h1 
 * @param h2 
 * @return linkedlistADT 
 */
linkedlistADT MultiplyPolynomial(linkedlistADT h1, linkedlistADT h2);/*乘*/
/**
 * @brief 
 * 
 * @param head 
 * @param p 
 */
void PolynomialInsertNode(linkedlistADT head, PolynomialT p); /*插入对象p，指数从高到低，合并同类项*/
/**
 * @brief 
 * 
 * @param input 
 * @return string 
 */
string FormatPolynomial(string input);
/**
 * @brief 
 * 
 * @param head 
 * @return string 
 */
string PolynomialToString(linkedlistADT head); 

#endif // (!defined POLYNOMIAL_H)