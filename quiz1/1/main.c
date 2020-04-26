#include "linkedList.h"
#include "polyno.h"
#include <stdio.h>

int main(void) {
    Poly p1 = polyInit(), p2 = polyInit();
    p1->read(p1);
    p2->read(p2);
    p1->add(p1, p2);
    p2->substract(p2, p1);
    p1->display(p1);
    p2->display(p2);

    p1->destory(p1);
    p2->destory(p2);
    return 0;
}

