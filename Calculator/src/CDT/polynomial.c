
#include "polynomial.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "genlib.h"
#include "simpio.h"
#include "strlib.h"
#include "linkedlist.h"

/*多项式插入一个对象p，指数从高到低，合并同类项，删除系数为0的结点*/
void PolynomialInsertNode(linkedlistADT head, PolynomialT p) {
    linkedlistADT nodeptr, prenodeptr, np;
    PolynomialT obj;

    if (p == NULL || p->coeff == 0) return;
    prenodeptr = head;
    nodeptr = NextNode(head, prenodeptr); /*nodeptr指向当前结点，prenodeptr指向前一个结点*/
    while (nodeptr != NULL) {
        obj = (PolynomialT) NodeObj(head, nodeptr); /*获取当前结点的数据对象指针*/
        if (obj->power <= p->power) break; /*找到插入位置：nodeptr之前，prenodeptr之后*/
        prenodeptr = nodeptr;              /*保存当前结点*/
        nodeptr = NextNode(head, nodeptr); /*下一个结点*/
    }
    if (nodeptr == NULL) {                        /*没找到*/
        InsertNode(head, NULL, p);                /*插入新结点到head链表的末尾*/
    } else {                                      /*找到*/
        if (obj->power == p->power) {             /*同类项*/
            obj->coeff += p->coeff;               /*在原数据对象上合并*/
            if (obj->coeff == 0) {                /*系数为0*/
                np = DeleteNode(head, obj, NULL); /*删除该项*/
                FreeBlock(np);                    /*释放该结点空间*/
                FreeBlock(obj);                   /*释放数据对象空间*/
            }
        } else { /*不是同类项，插入到nodeptr之前，prenodeptr之后*/
            InsertNode(head, prenodeptr, p);
        }
    }
    return;
}

/*读取多项式字符串并规范化：-2 x ^ 2 - x + 3 ==> -2x^(2)-1x^(1)+3x^(0)*/
string FormatPolynomial(string input) {
    string p;
    char buffer[200], prech;
    int n;

    p = input;
    n = 0;
    prech = 0; /*当前多项式符号类型*/
    while (*p != '\0') {
        if (*p == '+' || *p == '-') { /*系数符号*/
            buffer[n++] = *p;
        } else if (isdigit(*p) || *p == '.') { /*系数数值*/
            prech = n > 0 ? buffer[n - 1] : 0;
            while (isdigit(*p) || *p == '.') buffer[n++] = *p++;
            p--;
            buffer[n] = 0;
            if ((prech == 0 || prech == '+' || prech == '-') &&
                (*(p + 1) == '+' || *(p + 1) == '-' || *(p + 1) == '\0')) { /*常数项*/
                buffer[n++] = 'x';
                buffer[n++] = '^';
                buffer[n++] = '(';
                buffer[n++] = '0';
                buffer[n++] = ')';
            }
        } else if (*p == 'x') {
            if (!isdigit(buffer[n - 1]) && buffer[n - 1] != '.')
                buffer[n++] = '1'; /*系数为空，规范化为1*/
            buffer[n++] = 'x';
            if (*(p + 1) != '^') { /*指数为空，规范化为1*/
                buffer[n++] = '^';
                buffer[n++] = '(';
                buffer[n++] = '1';
                buffer[n++] = ')';
            }
        } else if (*p == '^') {
            buffer[n++] = *p;
            buffer[n++] = '(';
            if (*(p + 1) == '+' || *(p + 1) == '-') {
                buffer[n++] = *++p;
            }
            while (isdigit(*(p + 1)) || *(p + 1) == '.') buffer[n++] = *++p;
            buffer[n++] = ')';
        }
        p++;
    }
    buffer[n] = '\0';
    return CopyString(buffer);
}

/*从输入的多项式表达式（规范化后）中创建多项式—按指数从高到低, 合并同类项，删除系数为0的项*/
linkedlistADT CreatePolynomial(string format) {
    linkedlistADT head;
    PolynomialT p;
    string polynomialexp, str;
    char coeff[20], power[20];
    int n;

    str = polynomialexp = format;
    if (polynomialexp == NULL) Error("Invalid polynomial expression");
    head = NewLinkedList(); /*创建空表*/
    /*从规范化多项式字符串中读取相应数据：-2x^(2)-1x^(1)+3x^(0)*/
    while (TRUE) {
        n = sscanf(str, "%[+-0123456789.]x^(%[^)]", coeff, power); /*读取一项，如：-2x^(2)*/
        if (n != 2) break;
        str += StringLength(coeff) + 3 + StringLength(power) + 1; /*指针移至下一项*/
        p = New(PolynomialT);           /*申请多项式数据对象空间*/
        p->coeff = StringToReal(coeff); /*设置系数*/
        p->power = StringToReal(power); /*设置指数*/
        PolynomialInsertNode(head, p);  /*将数据对象插入到原多项式链表中*/
    }
    FreeBlock(polynomialexp);
    return head; /*返回表头指针*/
}

/*克隆一个链表*/
linkedlistADT ClonePolynomial(linkedlistADT head) {
    linkedlistADT h, p;
    PolynomialT tmp, obj;

    if (head == NULL) return NULL;
    h = NewLinkedList();      /*New一个空链表，仅含一个哑结点-头结点*/
    p = NextNode(head, head); /*p移至第一个数据结点*/
    while (p != NULL) {
        obj = (PolynomialT) NodeObj(head, p); /*获取p结点数据对象指针*/
        tmp = New(PolynomialT);               /*申请数据对象新的存储空间*/
        *tmp = *obj;                          /*赋值值*/
        InsertNode(h, NULL, tmp);             /*插入到h链表末尾*/
        p = NextNode(head, p);                /*p移至下一个结点*/
    }
    return h; /*返回新链表头指针*/
}

/*两个多项式链表相加——h2加到h1，h1改变，h2不变*/
linkedlistADT AddPolynomial(linkedlistADT h1, linkedlistADT h2) {
    linkedlistADT p;
    PolynomialT obj;

    if (h2 == NULL) return h1;
    p = NextNode(h2, h2); /*指向h2第一个数据对象结点*/
    while (p != NULL) {   /*对h2的每一个结点*/
        obj = New(PolynomialT);
        *obj = *(PolynomialT) NodeObj(h2, p);
        PolynomialInsertNode(h1, obj); /*插入多项式，考虑排序、同类项合并和删除系数为0的项*/
        p = NextNode(h2, p); /*p指向下一个结点*/
    }
    return h1; /*返回新的链表头指针*/
}

/*两个多项式链表相减——h1减去h2*/
linkedlistADT SubPolynomial(linkedlistADT h1, linkedlistADT h2) {
    linkedlistADT p;
    PolynomialT obj;

    if (h2 == NULL) return h1;
    p = NextNode(h2, h2);
    while (p != NULL) {
        obj = New(PolynomialT);
        obj->coeff = -((PolynomialT) NodeObj(h2, p))->coeff;
        obj->power = ((PolynomialT) NodeObj(h2, p))->power;
        PolynomialInsertNode(h1, obj);
        p = NextNode(h2, p);
    }
    return h1;
}

/*将(coeff，power)项乘以链表head的每个结点，并形成新链表*/
linkedlistADT MultiplyNode(linkedlistADT head, double coeff, double power) {
    linkedlistADT h, p;
    PolynomialT obj;

    if (head == NULL) return NULL;
    h = NewLinkedList();
    p = NextNode(head, head);
    while (p != NULL) {
        obj = New(PolynomialT);
        *obj = *(PolynomialT) NodeObj(head, p);
        obj->coeff *= coeff;
        obj->power += power;
        PolynomialInsertNode(h, obj);
        p = NextNode(head, p);
    }
    return h;
}

/*将链表h2乘到链表h1中——分别将h2中的每个结点乘以链表h1，然后相加*/
linkedlistADT MultiplyPolynomial(linkedlistADT h1, linkedlistADT h2) {
    linkedlistADT h, hh, p;
    PolynomialT obj;

    if (h1 == NULL) return h2;
    if (h2 == NULL) return h1;
    hh = NewLinkedList(); /*New一个空链表*/
    p = NextNode(h2, h2);
    while (p != NULL) {                               /*对h2的每个结点循环*/
        obj = (PolynomialT) NodeObj(h2, p);           /*数据对象*/
        h = MultiplyNode(h1, obj->coeff, obj->power); /*乘以h1多项式*/
        hh = AddPolynomial(hh, h);                    /*加到hh中*/
        p = NextNode(h2, p);                          /*下一个结点*/
    }
    return hh; /*返回新链表头指针*/
}

string PolynomialToString(linkedlistADT head) {
    linkedlistADT nodeptr = head;
    PolynomialT obj;

    char buffer[300] = "";
    int pos = 0;

    nodeptr = NextNode(head, nodeptr);
    while (nodeptr != NULL) {
        obj = (PolynomialT) NodeObj(head, nodeptr);

        if (obj->coeff == 0) { /*系数为0，不输出，直接转到下一项*/
            nodeptr = NextNode(head, nodeptr);
            continue;
        }

        /*处理系数*/
        if (obj->coeff == 1) {                     /*系数为1，只输出+号*/
            if (nodeptr != NextNode(head, head)) { /*第一项不输出+号*/
                buffer[pos++] = '+';
                buffer[pos] = '\0';
            }
        } else if (obj->coeff == -1) { /*系数为-1，只输出-号*/
            buffer[pos++] = '-';
            buffer[pos] = '\0';
        } else { /*带符号输出*/
            char temp[30] = "";
            if (nodeptr == NextNode(head, head)) { /*第一项不输出+号*/
                sprintf(temp, "%g", obj->coeff);
                pos += StringLength(temp);
                strcat(buffer, temp);
                buffer[pos] = '\0';
            } else {
                sprintf(temp, "%+g", obj->coeff);
                pos += StringLength(temp);
                strcat(buffer, temp);
                buffer[pos] = '\0';
            }
        }

        /*处理指数*/
        if (obj->power == 0) { /*若指数为0，则输出常数项*/
            if (obj->coeff == 1 || obj->coeff == -1) {
                buffer[pos++] = '1';
                buffer[pos] = '\0';
            } /*其它常数项在前面已处理*/
        } else if (obj->power == 1) {
            buffer[pos++] = 'x';
            buffer[pos] = '\0';
        } else {
            char temp[30] = "";
            if (obj->power < 0) {
                sprintf(temp, "x^(%g)", obj->power);
                pos += StringLength(temp);
                strcat(buffer, temp);
                buffer[pos] = '\0';
            } else {
                sprintf(temp, "x^%g", obj->power);
                pos += StringLength(temp);
                strcat(buffer, temp);
                buffer[pos] = '\0';
            }
        }

        nodeptr = NextNode(head, nodeptr);
    }
    
    return CopyString(buffer);
}
