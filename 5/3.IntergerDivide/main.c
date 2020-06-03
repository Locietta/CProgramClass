#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#include "qStack.h"

void intergerDivide(int n, int initial);

int main(void) {
    int n;
    while (EOF != scanf("%d", &n)) {
        intergerDivide(n, 1);
    }
}

void intergerDivide(int n, int initial) {
    static queue queueNum;
    static int limit = INT_MAX, num;
    if (initial) {
        queueNum = newQueue(int);
        num = n;
        for (int i = n - 1; i > 0; --i) {
            int limit_pre = limit;
            limit = i;
            queueNum.push(&queueNum, &i);
            intergerDivide(n - i, 0);
            queueNum.pop_back(&queueNum, NULL);
            limit = limit_pre;
        }
    } else {
        if (n) {
            for (int i = (limit < n) ? limit : n; i > 0; --i) {
                int limit_pre = limit;
                limit = i;
                queueNum.push(&queueNum, &i);
                intergerDivide(n - i, 0);
                queueNum.pop_back(&queueNum, NULL);
                limit = limit_pre;
            }
        } else {
            printf("%d =", num);
            int temp;
            queue queueBuf = newQueue(int);
            queueNum.pop(&queueNum, &temp);
            queueBuf.push(&queueBuf, &temp);
            printf(" %d", temp);
            while (!queueNum.empty(&queueNum)) {
                int temp;
                queueNum.pop(&queueNum, &temp);
                queueBuf.push(&queueBuf, &temp);
                printf(" + %d", temp);
            }
            putchar('\n');
            queueNum.destory(&queueNum);
            queueNum = queueBuf;
            return;
        }
    }
}