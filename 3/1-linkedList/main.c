#include "linkedList.h"
#include <stdio.h>

static int isBigger(void *data1, void *data2) {
    return (*((int *) data1) > *((int *) data2));
}

static void nodePrint(Node *node) {
    printf("%d ", *((int *) node->data));
}

int main(void) {
    int temp;
    List l1 = listInit(), l2 = listInit();
    while (scanf("%d", &temp) != EOF) {
        if (temp == -1) {
            break;
        }
        l1->tailInsert(l1, &temp, sizeof(temp));
    }
    while (scanf("%d", &temp) != EOF) {
        if (temp == -1) {
            break;
        }
        l2->tailInsert(l2, &temp, sizeof(temp));
    }
    l1->merge(l1, l2);
    l2->destory(l2);
    l1->sort(l1, isBigger, 1);

    l1->foreach (l1, nodePrint);
    putchar('\n');

    l1->destory(l1);
    return 0;
}