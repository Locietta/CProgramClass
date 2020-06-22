/**
 * @file polynomial.c
 * @author Locietta (https://github.com/Locietta)
 * @brief
 * @version 0.1
 * @date 2020-06-22 09:28
 *
 *
 */

#include "polynomial.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "strlib.h"

int zero_coeff(void *data) {
    polyItem *i = data;
    return !(i->coeff);
}

static void polynomialInsertItem(list p, polyItem *item) {
    if (item == NULL || item->coeff == 0) return;
    int flag = 1;
    iterator it = p.begin(&p);
    while (it != p.end(&p)) {
        polyItem *obj = it->data;
        if (obj->power == item->power) {
            obj->coeff += item->coeff;
            flag = 0;
            break;
        }
    }
    if (flag == 1) {
        p.push_back(&p, item);
        p.sort(&p);
    } else {
        p.remove_if(&p, zero_coeff);
    }
}

static char *formatPolyString(char *raw) {
    char buffer[100], *p = raw, prech = 0;
    int n = 0;
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

static int polycmp(void *data1, void *data2) {
    polyItem *item1 = data1, *item2 = data2;
    return (item1->power - item2->power);
}

list createPolynomial(char *raw) {
    char *poly_string_formatted = formatPolyString(raw);
    list poly = newList(polyItem);
    poly.setcmp(&poly, polycmp);
    char coeff[20], power[20];

    while (1) {
        int n = sscanf(poly_string_formatted, "%[+-0123456789.]x^(%[^)]", coeff, power);
        if (n != 2) break;
        poly_string_formatted += strlen(coeff) + 3 + strlen(power) + 1;
        polyItem temp = {.coeff = StringToReal(coeff), .power = StringToReal(power)};
        polynomialInsertItem(poly, &temp);
    }
    free(poly_string_formatted);
    return poly;
}

list addPolynomial(list h1, list h2) {
    if (h2.empty(&h2)) return h1;

    iterator p = h2.begin(&h2);
    while (p != h2.end(&h2)) {
        polynomialInsertItem(h1, p->data);
        p = p->next;
    }
    return h1;
}

list subPolynomial(list h1, list h2) {
    if (h2.empty(&h2)) return h1;

    iterator p = h2.begin(&h2);
    while (p != h2.end(&h2)) {
        polyItem temp = {.coeff = -((polyItem *) p->data)->coeff,
                         .power = ((polyItem *) p->data)->power};
        polynomialInsertItem(h1, &temp);
        p = p->next;
    }
    return h2;
}

list multiplySingle(list p, polyItem *item) {
    if (p.empty(&p)) return p;
    list h = newList(polyItem);

    iterator it = p.begin(&p);
    while (it != p.end(&p)) {
        polyItem *node = it->data;
        polyItem temp = {.coeff = node->coeff * item->coeff, .power = node->power + item->power};
        polynomialInsertItem(h, &temp);
        it = it->next;
    }
    return h;
}

list multiplyPolynomial(list p1, list p2) {
    if (p1.empty(&p1)) return p2;
    if (p2.empty(&p2)) return p1;
    list res = newList(polyItem);
    iterator it = p2.begin(&p2);
    while (it != p2.end(&p2)) {
        res = addPolynomial(res, multiplySingle(p1, it->data));
        it = it->next;
    }
    return res;
}

void PrintPolynomial(list p, char *target) {
    int pos = 0;
    iterator nodeptr = p.begin(&p);
    while (nodeptr != p.end(&p)) {
        polyItem *obj = nodeptr->data;
        if (obj->coeff == 0) {
            nodeptr = nodeptr->next;
            continue;
        }

        if (obj->coeff == 1) {
            if (nodeptr != p.begin(&p)) target[pos++] = '+';
        } else if (obj->coeff == -1) {
            target[pos++] = '-';
        } else {
            char temp[20];
            if (nodeptr != p.begin(&p)) {
                sprintf(temp, "%g", obj->coeff);
                pos += strlen(temp);
                strcat(target, temp);
            } else {
                sprintf(temp, "%+g", obj->coeff);
                pos += strlen(temp);
                strcat(target, temp);
            }
        }

        if (obj->power == 0) {
            if (obj->coeff == 1 || obj->coeff == -1) {
                target[pos++] = '1';
            }
        } else if (obj->power == 1) {
            target[pos++] = 'x';
        } else {
            char temp[20];
            if (obj->power < 0) {
                sprintf(temp, "x^(%g)", obj->power);
                pos += strlen(temp);
                strcat(target, temp);
            } else {
                sprintf(temp, "x^%g", obj->power);
                pos += strlen(temp);
                strcat(target, temp);
            }
        }
        nodeptr = nodeptr->next;
    }
    target[pos] = '\0';
}

list clonePolynomial(list p) {
    if (p.empty(&p)) return p;

    list res = newList(polyItem);
    iterator it = p.begin(&p);
    while (it != p.end(&p)) {
        res.push_back(&res, it->data);
        it = it->next;
    }
    return res;
}