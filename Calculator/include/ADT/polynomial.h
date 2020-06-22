

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "linkedList.h"

typedef struct _polynomial {
	double coeff, power;
} *PolynomialT;/*ָ������*/

linkedlistADT CreatePolynomial(string format);/*����һ������ʽ����*/
linkedlistADT ClonePolynomial(linkedlistADT head);/*��¡һ������ʽ*/
linkedlistADT AddPolynomial(linkedlistADT h1, linkedlistADT h2);/*��*/
linkedlistADT SubPolynomial(linkedlistADT h1, linkedlistADT h2);/*��*/
linkedlistADT MultiplyNode(linkedlistADT head, double coeff, double power);/*����ʽ����һ��*/
linkedlistADT MultiplyPolynomial(linkedlistADT h1, linkedlistADT h2);/*��*/
void PolynomialInsertNode(linkedlistADT head, PolynomialT p); /*�������p��ָ���Ӹߵ��ͣ��ϲ�ͬ����*/
string FormatPolynomial(string input);
string PolynomialToString(linkedlistADT head); 

#endif // (!defined POLYNOMIAL_H)