/**
 * @file layout.h
 * @author Locietta (https://github.com/Locietta)
 * @brief
 * @version 0.1
 * @date 2020-06-21 22:58
 *
 *
 */

#ifndef CALCULATOR_LAYOUT_H
#define CALCULATOR_LAYOUT_H

#include <stdbool.h>

#define BUTTON_NUMBER1 42

#define BUTTON_NUMBER2 20

typedef struct buttonT buttonT;

struct buttonT {
    char display[10];
    char output[10];

    void (*execute)(buttonT *this);
};

typedef enum calcMode { arithmetic, polynomial } calcMode;

typedef struct layout {
    char winTitle[36];
    double winWidth, winHeight;
    double btn_prop, expr_prop, result_prop;
    int btn_font_size, expr_font_size, result_font_size;
    int column1, row1, column2, row2;

    calcMode mode;

    char expr[500], res[100];
    int expr_len;

    buttonT *buttonTable1, *buttonTable2;
} layout;

extern layout calculatorLayout;

void update(void);

void initLayout(layout *obj);

#endif // (!defined CALCULATOR_LAYOUT_H)