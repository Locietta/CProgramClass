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
    list poly1 = createPolynomial(p1);/*创建第一个多项式链表*/
    printf("%d", poly1.size);
    string p2 = "5+6x";
    list poly2 = createPolynomial(p2);/*创建第二个多项式链表*/
    char temp[300];
    PrintPolynomial(poly1, temp); printf("多项式1：%s\n", temp); /*打印第一个多项式*/
    PrintPolynomial(poly2, temp); printf("多项式2：%s\n", temp); /*打印第二个多项式*/
    list h1 = clonePolynomial(poly1);/*克隆第一个多项式链表到h1*/
    list h2 = clonePolynomial(poly2);/*克隆第二个多项式链表到h2*/
    h1 = addPolynomial(h1, h2);/*两个多项式相加*/
    PrintPolynomial(h1, temp);printf("多项式和：%s\n", temp);/*打印*/

    return 0; 
}