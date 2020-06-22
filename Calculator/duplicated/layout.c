/**
 * @file layout.c
 * @author Locietta (https://github.com/Locietta)
 * @brief
 * @version 0.1
 * @date 2020-06-22 06:09
 *
 *
 */

#include <windows.h>
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stddef.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "layout.h"
#include "genlib.h"
#include "simpio.h"
#include "strlib.h"
#include <stdio.h>
#include <string.h>

#include "cJSON.h"

static void generalButtonExec(buttonT *this);

static void memAdd(buttonT *this);

static void memSub(buttonT *this);

static void memUse(buttonT *this);

static void memClear(buttonT *this);

static void clearExpr(buttonT *this);

static void delExpr(buttonT *this);

static void convertMode(buttonT *this);

static void arithCalc(buttonT *this);

static void multi_poly(buttonT *this);
static void add_poly(buttonT *this);
static void sub_poly(buttonT *this);
static void polyCalc(buttonT *this);

void update(void) {
    SetEraseMode(true); // ÇåÆÁ
    StartFilledRegion(0.5);
    MovePen(0, 0);
    DrawLine(calculatorLayout.winWidth, 0);
    DrawLine(0, calculatorLayout.winHeight);
    DrawLine(-calculatorLayout.winWidth, 0);
    DrawLine(0, -calculatorLayout.winHeight);
    EndFilledRegion();
    SetEraseMode(false);

    if (calculatorLayout.mode == arithmetic) {
        SaveGraphicsState();
        SetPenSize(3);
        MovePen(0, calculatorLayout.btn_prop * calculatorLayout.winHeight);
        DrawLine(calculatorLayout.winWidth, 0);
        SetPenSize(1);
        MovePen(0, calculatorLayout.btn_prop * calculatorLayout.winHeight);
        RestoreGraphicsState();
    }
}

void initLayout(layout *obj) {
    FILE *fp = fopen("../layout.json", "r+");
    string str_json = Read_File(fp);
    fclose(fp);
    cJSON *layout_data = cJSON_Parse(str_json);
    free(str_json);

    strcpy(obj->winTitle, cJSON_GetObjectItem(layout_data, "winTitle")->valuestring);
    obj->winWidth = cJSON_GetObjectItem(layout_data, "winWidth")->valuedouble;
    obj->winHeight = cJSON_GetObjectItem(layout_data, "winHeight")->valuedouble;
    obj->btn_prop =
        cJSON_GetObjectItem(cJSON_GetObjectItem(layout_data, "propotion"), "btn")->valuedouble;
    obj->expr_prop =
        cJSON_GetObjectItem(cJSON_GetObjectItem(layout_data, "propotion"), "expr")->valuedouble;
    obj->result_prop =
        cJSON_GetObjectItem(cJSON_GetObjectItem(layout_data, "propotion"), "result")->valuedouble;
    obj->btn_font_size =
        cJSON_GetObjectItem(cJSON_GetObjectItem(layout_data, "font"), "btn")->valueint;
    obj->expr_font_size =
        cJSON_GetObjectItem(cJSON_GetObjectItem(layout_data, "font"), "expr")->valueint;
    obj->result_font_size =
        cJSON_GetObjectItem(cJSON_GetObjectItem(layout_data, "font"), "result")->valueint;

    cJSON *columnTable = cJSON_GetObjectItem(layout_data, "column");

    obj->expr[0] = '\0';
    obj->expr_len = 0;
    obj->res[0] = '0';
    obj->res[1] = '\0';

    obj->buttonTable1 = (buttonT *) malloc(BUTTON_NUMBER1 * sizeof(buttonT));
    int pos = 0;
    obj->buttonTable1[pos++] = (buttonT){"M+", "", memAdd};
    obj->buttonTable1[pos++] = (buttonT){"M-", "", memSub};
    obj->buttonTable1[pos++] = (buttonT){"M", "", memUse};
    obj->buttonTable1[pos++] = (buttonT){"MC", "", memClear};
    obj->buttonTable1[pos++] = (buttonT){"CE", "", clearExpr};
    obj->buttonTable1[pos++] = (buttonT){"DEL", "", delExpr};
    obj->buttonTable1[pos++] = (buttonT){"poly", "", convertMode};

    cJSON *button1Array = cJSON_GetObjectItem(layout_data, "button1");
    int size1 = cJSON_GetArraySize(button1Array);
    for (int i = 0; i < size1; ++i) {
        cJSON *tempNode = cJSON_GetArrayItem(button1Array, i);
        if (cJSON_GetObjectItem(tempNode, "output") != NULL) {
            strcpy(obj->buttonTable2[pos].display,
                   cJSON_GetObjectItem(tempNode, "display")->valuestring);
            strcpy(obj->buttonTable2[pos].output,
                   cJSON_GetObjectItem(tempNode, "output")->valuestring);
            obj->buttonTable2[pos].execute = generalButtonExec;
            pos++;

        } else {
            strcpy(obj->buttonTable2[pos].display,
                   cJSON_GetObjectItem(tempNode, "display")->valuestring);
            strcpy(obj->buttonTable2[pos].output,
                   cJSON_GetObjectItem(tempNode, "display")->valuestring);
            obj->buttonTable2[pos].execute = generalButtonExec;
            pos++;
        }
    }
    obj->buttonTable1[pos] = (buttonT){"=", "", arithCalc};

    obj->buttonTable2 = (buttonT *) malloc(BUTTON_NUMBER2 * sizeof(buttonT));
    pos = 0;
    obj->buttonTable2[pos++] = (buttonT){"Arith", "", convertMode};
    obj->buttonTable2[pos++] = (buttonT){"X", "", multi_poly};
    obj->buttonTable2[pos++] = (buttonT){"+", "", add_poly};
    obj->buttonTable2[pos++] = (buttonT){"-", "", sub_poly};
    obj->buttonTable2[pos++] = (buttonT){"=", "", polyCalc};

    cJSON *button2Array = cJSON_GetObjectItem(layout_data, "button2");
    int size2 = cJSON_GetArraySize(button2Array);

    for (int i = 0; i < size2; ++i) {
        cJSON *tempNode = cJSON_GetArrayItem(button2Array, i);
        if (cJSON_GetObjectItem(tempNode, "output") != NULL) {
            strcpy(obj->buttonTable2[pos].display,
                   cJSON_GetObjectItem(tempNode, "display")->valuestring);
            strcpy(obj->buttonTable2[pos].output,
                   cJSON_GetObjectItem(tempNode, "output")->valuestring);
            obj->buttonTable2[pos].execute = generalButtonExec;
            pos++;

        } else {
            strcpy(obj->buttonTable2[pos].display,
                   cJSON_GetObjectItem(tempNode, "display")->valuestring);
            strcpy(obj->buttonTable2[pos].output,
                   cJSON_GetObjectItem(tempNode, "display")->valuestring);
            obj->buttonTable2[pos].execute = generalButtonExec;
            pos++;
        }
    }
}

static double mem;

static void generalButtonExec(buttonT *this) {
    int len = strlen(this->output);
    strcat(calculatorLayout.expr, this->output);
    calculatorLayout.expr_len += len;
}

static void memAdd(buttonT *this) {
    mem += StringToReal(calculatorLayout.res);
}

static void memSub(buttonT *this) {
    mem -= StringToReal(calculatorLayout.res);
}

static void memUse(buttonT *this) {
    string numChar = RealToString(mem);
    int len = strlen(numChar);
    strcat(calculatorLayout.expr, numChar);
    calculatorLayout.expr_len += len;
    free(numChar);
}

static void memClear(buttonT *this) {
    mem = 0;
}

static void clearExpr(buttonT *this) {
    calculatorLayout.expr[0] = '\0';
    calculatorLayout.expr_len = 0;
}

static void delExpr(buttonT *this) {
    calculatorLayout.expr[--calculatorLayout.expr_len] = '\0';
}

static void convertMode(buttonT *this) {
    calculatorLayout.mode = 1 - calculatorLayout.mode;
}

static void arithCalc(buttonT *this) {
    // do nothing
}

static void multi_poly(buttonT *this) {}

static void add_poly(buttonT *this) {}

static void sub_poly(buttonT *this) {}

static void polyCalc(buttonT *this) {}