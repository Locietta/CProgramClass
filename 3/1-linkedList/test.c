#include "linkedList.h"
#include <stdio.h>

/* #define listPrint(L)          \
        do {                      \
            printf("L: ");        \
            L->foreach (l1, pri); \
            putchar('\n');        \
        } while (0) 
*/

int isBigger(void *data1, void *data2) {
    return (*((int *) data1) > *((int *) data2));
}
int key(void *test_data, void *key_data) {
    return (*((int *) test_data) == *((int *) key_data));
}
void pri(Node *node) {
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
    printf("l1: ");
    l1->foreach (l1, pri);
    putchar('\n');
    printf("l2: ");
    l2->foreach (l2, pri);
    putchar('\n');
    scanf("%d", &temp);
    Node *found = NULL;
    while ((found = l1->find(l1, key, &temp))) {
        printf("Input>");
        int newint;
        scanf("%d", &newint);
        l1->modify(l1, found, &newint, sizeof(newint));
    }
    printf("l1: ");
    l1->foreach (l1, pri);
    putchar('\n');

    l2->sort(l2, isBigger, 1);
    printf("increased l2: ");
    l2->foreach (l2, pri);
    putchar('\n');
    l1->merge(l1, l2);
    printf("l1: ");
    l1->foreach (l1, pri);
    putchar('\n');
    l1->inverse(l1);
    printf("l1: ");
    l1->foreach (l1, pri);
    putchar('\n');
    while (!l1->isempty(l1)) {
        l1->delete (l1, l1->end(l1));
        printf("l1: ");
        l1->foreach (l1, pri);
        putchar('\n');
    }
    while (!l2->isempty(l2)) {
        l2->delete (l2, l2->begin(l2));
        printf("l2: ");
        l2->foreach (l2, pri);
        putchar('\n');
    }

    return 0;
}
