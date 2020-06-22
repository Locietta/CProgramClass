
#include "polynomial.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "genlib.h"
#include "simpio.h"
#include "strlib.h"
#include "linkedlist.h"

/*����ʽ����һ������p��ָ���Ӹߵ��ͣ��ϲ�ͬ���ɾ��ϵ��Ϊ0�Ľ��*/
void PolynomialInsertNode(linkedlistADT head, PolynomialT p) {
    linkedlistADT nodeptr, prenodeptr, np;
    PolynomialT obj;

    if (p == NULL || p->coeff == 0) return;
    prenodeptr = head;
    nodeptr = NextNode(head, prenodeptr); /*nodeptrָ��ǰ��㣬prenodeptrָ��ǰһ�����*/
    while (nodeptr != NULL) {
        obj = (PolynomialT) NodeObj(head, nodeptr); /*��ȡ��ǰ�������ݶ���ָ��*/
        if (obj->power <= p->power) break; /*�ҵ�����λ�ã�nodeptr֮ǰ��prenodeptr֮��*/
        prenodeptr = nodeptr;              /*���浱ǰ���*/
        nodeptr = NextNode(head, nodeptr); /*��һ�����*/
    }
    if (nodeptr == NULL) {                        /*û�ҵ�*/
        InsertNode(head, NULL, p);                /*�����½�㵽head�����ĩβ*/
    } else {                                      /*�ҵ�*/
        if (obj->power == p->power) {             /*ͬ����*/
            obj->coeff += p->coeff;               /*��ԭ���ݶ����Ϻϲ�*/
            if (obj->coeff == 0) {                /*ϵ��Ϊ0*/
                np = DeleteNode(head, obj, NULL); /*ɾ������*/
                FreeBlock(np);                    /*�ͷŸý��ռ�*/
                FreeBlock(obj);                   /*�ͷ����ݶ���ռ�*/
            }
        } else { /*����ͬ������뵽nodeptr֮ǰ��prenodeptr֮��*/
            InsertNode(head, prenodeptr, p);
        }
    }
    return;
}

/*��ȡ����ʽ�ַ������淶����-2 x ^ 2 - x + 3 ==> -2x^(2)-1x^(1)+3x^(0)*/
string FormatPolynomial(string input) {
    string p;
    char buffer[200], prech;
    int n;

    p = input;
    n = 0;
    prech = 0; /*��ǰ����ʽ��������*/
    while (*p != '\0') {
        if (*p == '+' || *p == '-') { /*ϵ������*/
            buffer[n++] = *p;
        } else if (isdigit(*p) || *p == '.') { /*ϵ����ֵ*/
            prech = n > 0 ? buffer[n - 1] : 0;
            while (isdigit(*p) || *p == '.') buffer[n++] = *p++;
            p--;
            buffer[n] = 0;
            if ((prech == 0 || prech == '+' || prech == '-') &&
                (*(p + 1) == '+' || *(p + 1) == '-' || *(p + 1) == '\0')) { /*������*/
                buffer[n++] = 'x';
                buffer[n++] = '^';
                buffer[n++] = '(';
                buffer[n++] = '0';
                buffer[n++] = ')';
            }
        } else if (*p == 'x') {
            if (!isdigit(buffer[n - 1]) && buffer[n - 1] != '.')
                buffer[n++] = '1'; /*ϵ��Ϊ�գ��淶��Ϊ1*/
            buffer[n++] = 'x';
            if (*(p + 1) != '^') { /*ָ��Ϊ�գ��淶��Ϊ1*/
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

/*������Ķ���ʽ���ʽ���淶�����д�������ʽ����ָ���Ӹߵ���, �ϲ�ͬ���ɾ��ϵ��Ϊ0����*/
linkedlistADT CreatePolynomial(string format) {
    linkedlistADT head;
    PolynomialT p;
    string polynomialexp, str;
    char coeff[20], power[20];
    int n;

    str = polynomialexp = format;
    if (polynomialexp == NULL) Error("Invalid polynomial expression");
    head = NewLinkedList(); /*�����ձ�*/
    /*�ӹ淶������ʽ�ַ����ж�ȡ��Ӧ���ݣ�-2x^(2)-1x^(1)+3x^(0)*/
    while (TRUE) {
        n = sscanf(str, "%[+-0123456789.]x^(%[^)]", coeff, power); /*��ȡһ��磺-2x^(2)*/
        if (n != 2) break;
        str += StringLength(coeff) + 3 + StringLength(power) + 1; /*ָ��������һ��*/
        p = New(PolynomialT);           /*�������ʽ���ݶ���ռ�*/
        p->coeff = StringToReal(coeff); /*����ϵ��*/
        p->power = StringToReal(power); /*����ָ��*/
        PolynomialInsertNode(head, p);  /*�����ݶ�����뵽ԭ����ʽ������*/
    }
    FreeBlock(polynomialexp);
    return head; /*���ر�ͷָ��*/
}

/*��¡һ������*/
linkedlistADT ClonePolynomial(linkedlistADT head) {
    linkedlistADT h, p;
    PolynomialT tmp, obj;

    if (head == NULL) return NULL;
    h = NewLinkedList();      /*Newһ������������һ���ƽ��-ͷ���*/
    p = NextNode(head, head); /*p������һ�����ݽ��*/
    while (p != NULL) {
        obj = (PolynomialT) NodeObj(head, p); /*��ȡp������ݶ���ָ��*/
        tmp = New(PolynomialT);               /*�������ݶ����µĴ洢�ռ�*/
        *tmp = *obj;                          /*��ֵֵ*/
        InsertNode(h, NULL, tmp);             /*���뵽h����ĩβ*/
        p = NextNode(head, p);                /*p������һ�����*/
    }
    return h; /*����������ͷָ��*/
}

/*��������ʽ������ӡ���h2�ӵ�h1��h1�ı䣬h2����*/
linkedlistADT AddPolynomial(linkedlistADT h1, linkedlistADT h2) {
    linkedlistADT p;
    PolynomialT obj;

    if (h2 == NULL) return h1;
    p = NextNode(h2, h2); /*ָ��h2��һ�����ݶ�����*/
    while (p != NULL) {   /*��h2��ÿһ�����*/
        obj = New(PolynomialT);
        *obj = *(PolynomialT) NodeObj(h2, p);
        PolynomialInsertNode(h1, obj); /*�������ʽ����������ͬ����ϲ���ɾ��ϵ��Ϊ0����*/
        p = NextNode(h2, p); /*pָ����һ�����*/
    }
    return h1; /*�����µ�����ͷָ��*/
}

/*��������ʽ�����������h1��ȥh2*/
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

/*��(coeff��power)���������head��ÿ����㣬���γ�������*/
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

/*������h2�˵�����h1�С����ֱ�h2�е�ÿ������������h1��Ȼ�����*/
linkedlistADT MultiplyPolynomial(linkedlistADT h1, linkedlistADT h2) {
    linkedlistADT h, hh, p;
    PolynomialT obj;

    if (h1 == NULL) return h2;
    if (h2 == NULL) return h1;
    hh = NewLinkedList(); /*Newһ��������*/
    p = NextNode(h2, h2);
    while (p != NULL) {                               /*��h2��ÿ�����ѭ��*/
        obj = (PolynomialT) NodeObj(h2, p);           /*���ݶ���*/
        h = MultiplyNode(h1, obj->coeff, obj->power); /*����h1����ʽ*/
        hh = AddPolynomial(hh, h);                    /*�ӵ�hh��*/
        p = NextNode(h2, p);                          /*��һ�����*/
    }
    return hh; /*����������ͷָ��*/
}

string PolynomialToString(linkedlistADT head) {
    linkedlistADT nodeptr = head;
    PolynomialT obj;

    char buffer[300] = "";
    int pos = 0;

    nodeptr = NextNode(head, nodeptr);
    while (nodeptr != NULL) {
        obj = (PolynomialT) NodeObj(head, nodeptr);

        if (obj->coeff == 0) { /*ϵ��Ϊ0���������ֱ��ת����һ��*/
            nodeptr = NextNode(head, nodeptr);
            continue;
        }

        /*����ϵ��*/
        if (obj->coeff == 1) {                     /*ϵ��Ϊ1��ֻ���+��*/
            if (nodeptr != NextNode(head, head)) { /*��һ����+��*/
                buffer[pos++] = '+';
                buffer[pos] = '\0';
            }
        } else if (obj->coeff == -1) { /*ϵ��Ϊ-1��ֻ���-��*/
            buffer[pos++] = '-';
            buffer[pos] = '\0';
        } else { /*���������*/
            char temp[30] = "";
            if (nodeptr == NextNode(head, head)) { /*��һ����+��*/
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

        /*����ָ��*/
        if (obj->power == 0) { /*��ָ��Ϊ0�������������*/
            if (obj->coeff == 1 || obj->coeff == -1) {
                buffer[pos++] = '1';
                buffer[pos] = '\0';
            } /*������������ǰ���Ѵ���*/
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
