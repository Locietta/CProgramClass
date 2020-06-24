

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "linkedList.h"

/**
 * @brief �ṹ: ����ʽ��һ�� PolynomialT
 * @details coeff:ϵ�� power:ָ��
 */
typedef struct _polynomial {
	double coeff, power;
} *PolynomialT;/*ָ������*/

/**
 * @brief ����һ������ʽ����
 * 
 * @param format ��׼�����Ķ���ʽ�ַ���
 * @return linkedlistADT ���ش����Ķ���ʽ����
 */

linkedlistADT CreatePolynomial(string format);/*����һ������ʽ����*/
/**
 * @brief 
 * 
 * @param head 
 * @return linkedlistADT 
 */
linkedlistADT ClonePolynomial(linkedlistADT head);/*��¡һ������ʽ*/
/**
 * @brief 
 * 
 * @param h1 
 * @param h2 
 * @return linkedlistADT 
 */
linkedlistADT AddPolynomial(linkedlistADT h1, linkedlistADT h2);/*��*/
/**
 * @brief 
 * 
 * @param h1 
 * @param h2 
 * @return linkedlistADT 
 */
linkedlistADT SubPolynomial(linkedlistADT h1, linkedlistADT h2);/*��*/
/**
 * @brief 
 * 
 * @param head 
 * @param coeff 
 * @param power 
 * @return linkedlistADT 
 */
linkedlistADT MultiplyNode(linkedlistADT head, double coeff, double power);/*����ʽ����һ��*/
/**
 * @brief 
 * 
 * @param h1 
 * @param h2 
 * @return linkedlistADT 
 */
linkedlistADT MultiplyPolynomial(linkedlistADT h1, linkedlistADT h2);/*��*/
/**
 * @brief 
 * 
 * @param head 
 * @param p 
 */
void PolynomialInsertNode(linkedlistADT head, PolynomialT p); /*�������p��ָ���Ӹߵ��ͣ��ϲ�ͬ����*/
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