#ifndef POLYNO_H
#define POLYNO_H

#include "linkedList.h"

typedef struct poly *Poly;

struct poly {
    List L;

    void (*read)(Poly this);
    void (*display)(Poly this);

    void (*add)(Poly this, Poly another);
    void (*substract)(Poly this, Poly another);
    // void (*multiple)(Poly this, Poly another);

    void (*destory)(Poly this);
};

Poly polyInit(void);

#endif // (!defined POLYNO_H)