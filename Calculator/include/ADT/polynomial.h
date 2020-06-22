

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "linkedList.h"

typedef struct _polynomial {
	double coeff, power;
} *PolynomialT;/*指针类型*/

linkedlistADT CreatePolynomial(string format);/*创建一个多项式链表*/
linkedlistADT ClonePolynomial(linkedlistADT head);/*克隆一个多项式*/
linkedlistADT AddPolynomial(linkedlistADT h1, linkedlistADT h2);/*加*/
linkedlistADT SubPolynomial(linkedlistADT h1, linkedlistADT h2);/*减*/
linkedlistADT MultiplyNode(linkedlistADT head, double coeff, double power);/*多项式乘以一项*/
linkedlistADT MultiplyPolynomial(linkedlistADT h1, linkedlistADT h2);/*乘*/
void PolynomialInsertNode(linkedlistADT head, PolynomialT p); /*插入对象p，指数从高到低，合并同类项*/
string FormatPolynomial(string input);
string PolynomialToString(linkedlistADT head); 

#endif // (!defined POLYNOMIAL_H)