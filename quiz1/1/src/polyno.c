#include "polyno.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct term {
    double coeff;
    int pow;
} term;

/* Private functions prototypes */

static int key(void *test_data, void *key_data);
static int isBigger(void *data1, void *data2);
static void printTerm(Node *node, int signal);

/* Public functions prototypes */

static void polyRead(Poly this);
static void polyDisplay(Poly this);

static void polyAdd(Poly this, Poly another);
static void polySubstract(Poly this, Poly another);
// static void polyMultiple(Poly this, Poly another);

static void polyDestroy(Poly this);

/* Function implementations */

/* Public functions */

Poly polyInit(void) {
    Poly newPoly = (Poly) malloc(sizeof(struct poly));

    newPoly->L = listInit();

    newPoly->read = polyRead; // remained for user
    newPoly->display = polyDisplay;

    newPoly->add = polyAdd;
    newPoly->substract = polySubstract;
    // newPoly->multiple = polyMultiple;

    newPoly->destory = polyDestroy;

    return newPoly;
}

static void polyRead(Poly this) {
    term newterm;
    while (scanf("%*[( ]%lf%*[, ]%d%*[ )]", &newterm.coeff, &newterm.pow) != EOF) {// fucking scanf
        
        // printf("(%g,%d)", newterm.coeff, newterm.pow);
        
        if (newterm.pow < 0) {
            while (getchar() != '\n'); // fucking scanf
            break;
        } else if (newterm.coeff) {
            this->L->tailInsert(this->L, &newterm, sizeof(term));
        }
    }

    if (!this->L->isempty(this->L)) {
        this->L->sort(this->L, isBigger, 0);
    } else {
        newterm = (term) {0,0};
        this->L->tailInsert(this->L, &newterm, sizeof(term));
    }
}

static void polyDisplay(Poly this) {
    this->L->foreach (this->L, printTerm);
    putchar('\n');
}

static void polyAdd(Poly this, Poly another) {
    Node *current = another->L->head;
    for (int i = 0; i < another->L->len; ++i) {
        Node *found = this->L->find(this->L, key, current->data);
        if (found == NULL) {
            this->L->tailInsert(this->L, current->data, sizeof(term));
        } else if (((term *) found->data)->pow == ((term *) current->data)->pow) {
            term newterm = (term){
                .coeff = ((term *) found->data)->coeff + ((term *) current->data)->coeff,
                .pow = ((term *) found->data)->pow};
            this->L->modify(this->L, found, &newterm, sizeof(term));
        } else {
            this->L->insert(this->L, found, current->data, sizeof(term));
        }
        current = current->next;
    }

    current = this->L->head;
    for (int i = 0; i < this->L->len; ++i) {
        if (((term *) current->data)->coeff == 0) {
            this->L->delete (this->L, current);
        }
    }
}

static void polySubstract(Poly this, Poly another) {
    Node *current = another->L->head;
    for (int i = 0; i < another->L->len; ++i) {
        Node *found = this->L->find(this->L, key, current->data);
        if (found == NULL) {
            this->L->tailInsert(this->L, current->data, sizeof(term));
        } else if (((term *) found->data)->pow == ((term *) current->data)->pow) {
            term newterm = (term){
                .coeff = ((term *) found->data)->coeff - ((term *) current->data)->coeff,
                .pow = ((term *) found->data)->pow};
            this->L->modify(this->L, found, &newterm, sizeof(term));
        } else {
            this->L->insert(this->L, found, current->data, sizeof(term));
        }
        current = current->next;
    }

    current = this->L->head;
    for (int i = 0; i < this->L->len; ++i) {
        if (((term *) current->data)->coeff == 0) {
            this->L->delete (this->L, current);
        }
    }
}

static void polyDestroy(Poly this) {
    this->L->destory(this->L);
    free(this);
}

/* Private Functions */

static int key(void *test_data, void *key_data) {
    return ((term *) test_data)->pow <= ((term *) key_data)->pow;
}

static int isBigger(void *data1, void *data2) {
    return (((term *) data1)->pow > ((term *) data2)->pow);
}

static void printTerm(Node *node, int signal) {
    term *current = (term *) node->data;
    if (signal && current->coeff > 0) {
        putchar('+');
    }
    if ((current->coeff != 1 && current->coeff != -1) || current->pow == 0) {
        printf("%g", current->coeff);
    } else if (current->coeff == -1) {
        putchar('-');
    } 

    if (current->pow > 1) {
        printf("x^%d", current->pow);
    } else if (current->pow == 1) {
        printf("x");
    }
}