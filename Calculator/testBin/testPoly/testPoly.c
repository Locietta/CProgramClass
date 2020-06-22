/**
 * @file testPoly.c
 * @author Locietta (https://github.com/Locietta)
 * @brief 
 * @version 0.1
 * @date 2020-06-22 12:03
 * 
 * 
 */

#include "polynomial.h"

#include "simpio.h"

int main(void) {
    
    string p1 = "1+2x";
    list poly1 = createPolynomial(p1);/*������һ������ʽ����*/
    printf("%d", poly1.size);
    string p2 = "5+6x";
    list poly2 = createPolynomial(p2);/*�����ڶ�������ʽ����*/
    char temp[300];
    PrintPolynomial(poly1, temp); printf("����ʽ1��%s\n", temp); /*��ӡ��һ������ʽ*/
    PrintPolynomial(poly2, temp); printf("����ʽ2��%s\n", temp); /*��ӡ�ڶ�������ʽ*/
    list h1 = clonePolynomial(poly1);/*��¡��һ������ʽ����h1*/
    list h2 = clonePolynomial(poly2);/*��¡�ڶ�������ʽ����h2*/
    h1 = addPolynomial(h1, h2);/*��������ʽ���*/
    PrintPolynomial(h1, temp);printf("����ʽ�ͣ�%s\n", temp);/*��ӡ*/

    return 0; 
}