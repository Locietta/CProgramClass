/**
 * @file main.c
 * @author Locietta (https://github.com/Locietta)
 * @brief
 * @version 0.1
 * @date 2020-06-21 22:07
 *
 *
 */

#include <windows.h>
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include "strlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <stdbool.h>

#include "calc.h"
#include "cJSON.h"
#include "polynomial.h"

#include "linkedList.h"

#define BUTTON_NUMBER1 42

#define BUTTON_NUMBER2 20

/**
 * @brief 按钮结构: buttonT
 * 
 */
typedef struct buttonT buttonT;
struct buttonT {
    char *display; //< 按钮显示的文本
    char *output;  //< 非功能键的情况下,按下按钮会向输出缓冲添加的文本

    void (*execute)(buttonT *this); //< 决定按钮按下时的操作,非功能键其值统一为generalButtonExec
};

/**
 * @brief 运算模式标识枚举: calcMode
 * 
 */
typedef enum calcMode { arithmetic, polynomial } calcMode;

/**
 * @brief 布局配置结构: layout
 * @details 包含了计算器布局的几乎所有信息
 */
typedef struct layout {
    char winTitle[36]; //< 窗口的标题
    double winWidth, winHeight; //< 窗口的长宽
    double btn_prop, expr_prop, result_prop; //< 按钮,输入表达式,输出结果三个区域所占比例
    int btn_font_size, expr_font_size, result_font_size; //< 三个区域分别所用的字体大小
    string fontname; //< 使用的字体
    int column1, row1; //< 算术模式下,按钮的行列数 
    int column2, row2; //< 多项式模式下,按钮的行列数

    calcMode mode; //< 标识当前是算术模式还是多项式模式

    string expr; //< 输入表达式缓冲区
    string res; //< 输出结果缓冲区
    int expr_len; //< 素如表达式的长度

    buttonT buttonTable1[BUTTON_NUMBER1], buttonTable2[BUTTON_NUMBER2]; // 算术模式和多项式模式下的按钮数组
} layout;


/**
 * @brief 静态全局变量 calculatorLayout
 * @details 从.json读取布局信息之后储存在这里
 * 
 */
static layout calculatorLayout;

/**
 * @brief 界面更新函数: void update(void)
 * @details 根据calculatorLayout全局变量的状态刷新界面
 */
void update(void);

/**
 * @brief 配置初始化函数: void initLayout(void)
 * @details 从layout.json配置文件中读取界面布局信息
 */
void initLayout(void);

/**
 * @brief 鼠标回调函数 MouseEventProcess()
 * 
 */
void MouseEventProcess(int x, int y, int button, int event);

/**
 * @brief 按钮编号获取器 buttonOrderFetcher()
 * 
 * @param x[in] 鼠标横坐标
 * @param y[in] 鼠标纵坐标
 * @return int 返回相应的按钮序号
 */
int buttonOrderFetcher(double x, double y);

void Main() {
    initLayout(); //< 读入配置

    /* 根据配置设置窗口属性 */
    SetWindowTitle(calculatorLayout.winTitle); 
    SetWindowSize(calculatorLayout.winWidth, calculatorLayout.winHeight);
    InitGraphics();
    SetPenSize(2);
    SetPenColor("Black");
    SetFont(calculatorLayout.fontname);
    update();

    /* 注册鼠标回调函数 */
    registerMouseEvent(MouseEventProcess);
}

void MouseEventProcess(int x, int y, int button, int event) {
    double mx = ScaleXInches(x), my = ScaleYInches(y); //< Pixel转inches
    double total_btn_height = calculatorLayout.btn_prop * calculatorLayout.winHeight; //< 按钮区域的总高度
    switch (event) {
    case BUTTON_DOWN:
        if (button == LEFT_BUTTON) {
            if (my < total_btn_height) { //< 在按钮区域内
                if (calculatorLayout.mode == arithmetic) {
                    buttonT pressed_button =
                        calculatorLayout.buttonTable1[buttonOrderFetcher(mx, my)];
                    pressed_button.execute(&pressed_button);
                    update();
                } else {
                    buttonT pressed_button =
                        calculatorLayout.buttonTable2[buttonOrderFetcher(mx, my)];
                    pressed_button.execute(&pressed_button);
                    update();
                }
            }
        }
    }
}

int buttonOrderFetcher(double x, double y) {
    double total_btn_height = calculatorLayout.btn_prop * calculatorLayout.winHeight;
    if (calculatorLayout.mode == arithmetic) {
        double btn_width = calculatorLayout.winWidth / calculatorLayout.column1;
        double btn_height = total_btn_height / calculatorLayout.row1;

        int ix, iy;
        for (ix = 0; ix * btn_width < x; ++ix) continue;
        ix--;
        for (iy = 0; total_btn_height - iy * btn_height > y; ++iy) continue;
        iy--;
        return calculatorLayout.column1 * iy + ix;
    } else {
        double btn_width = calculatorLayout.winWidth / calculatorLayout.column2;
        double btn_height = total_btn_height / calculatorLayout.row2;

        int ix, iy;
        for (ix = 0; ix * btn_width < x; ++ix) continue;
        ix--;
        for (iy = 0; total_btn_height - iy * btn_height > y; ++iy) continue;
        iy--;
        return calculatorLayout.column2 * iy + ix;
    }
}

/**
 * @brief 按钮的操作函数
 * @details buttonT .execute()成员的所有可能值
 * @{
 */

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

/**
 * @}
 * 
 */

void update(void) {
    SetEraseMode(true); // 清屏
    StartFilledRegion(0.5);
    MovePen(0, 0);
    DrawLine(calculatorLayout.winWidth, 0);
    DrawLine(0, calculatorLayout.winHeight);
    DrawLine(-calculatorLayout.winWidth, 0);
    DrawLine(0, -calculatorLayout.winHeight);
    EndFilledRegion();
    SetEraseMode(false);

    SaveGraphicsState();
    SetPenSize(3);
    double total_btn_height = calculatorLayout.btn_prop * calculatorLayout.winHeight;
    MovePen(0, total_btn_height);
    DrawLine(calculatorLayout.winWidth, 0);
    SetPenSize(1);
    MovePen(0, (calculatorLayout.btn_prop + calculatorLayout.result_prop) *
                   calculatorLayout.winHeight);
    DrawLine(calculatorLayout.winWidth, 0);
    RestoreGraphicsState();

    if (calculatorLayout.mode == arithmetic) {
        double btn_width = calculatorLayout.winWidth / calculatorLayout.column1;
        double btn_height = total_btn_height / calculatorLayout.row1;
        for (int i = 1; i < calculatorLayout.column1; ++i) {
            MovePen(i * btn_width, 0);
            DrawLine(0, total_btn_height);
        }
        for (int j = 1; j < calculatorLayout.row1; ++j) {
            MovePen(0, j * btn_height);
            DrawLine(calculatorLayout.winWidth, 0);
        }

        SaveGraphicsState();
        SetPointSize(calculatorLayout.btn_font_size);
        double corner_lu_x = 0.30 * btn_width, corner_lu_y = total_btn_height - 0.70 * btn_height;
        for (int i = 0; i < BUTTON_NUMBER1; ++i) {
            MovePen(corner_lu_x + (i % calculatorLayout.column1) * btn_width,
                    corner_lu_y - (i / calculatorLayout.column1) * btn_height);
            DrawTextString(calculatorLayout.buttonTable1[i].display);
        }
        RestoreGraphicsState();
    } else {
        double btn_width = calculatorLayout.winWidth / calculatorLayout.column2;
        double btn_height = total_btn_height / calculatorLayout.row2;
        for (int i = 1; i < calculatorLayout.column2; ++i) {
            MovePen(i * btn_width, 0);
            DrawLine(0, total_btn_height);
        }
        for (int j = 1; j < calculatorLayout.row2; ++j) {
            MovePen(0, j * btn_height);
            DrawLine(calculatorLayout.winWidth, 0);
        }

        SaveGraphicsState();
        SetPointSize(calculatorLayout.btn_font_size);
        double corner_lu_x = 0.4 * btn_width, corner_lu_y = total_btn_height - 0.6 * btn_height;
        for (int i = 0; i < BUTTON_NUMBER2; ++i) {
            MovePen(corner_lu_x + (i % calculatorLayout.column2) * btn_width,
                    corner_lu_y - (i / calculatorLayout.column2) * btn_height);
            DrawTextString(calculatorLayout.buttonTable2[i].display);
        }
        RestoreGraphicsState();
    }

    SaveGraphicsState();
    SetPointSize(calculatorLayout.result_font_size);
    MovePen(0.05 * calculatorLayout.winWidth, total_btn_height * 1.1);
    DrawTextString(calculatorLayout.res);
    RestoreGraphicsState();

    SaveGraphicsState();
    SetPointSize(calculatorLayout.expr_font_size);
    MovePen(0.05 * calculatorLayout.winWidth,
            (calculatorLayout.btn_prop + calculatorLayout.result_prop) *
                calculatorLayout.winHeight * 1.05);
    DrawTextString(calculatorLayout.expr);
    RestoreGraphicsState();
}

void initLayout(void) {
    FILE *fp = fopen("layout.json", "r+");
    string str_json = Read_File(fp);
    fclose(fp);
    cJSON *layout_data = cJSON_Parse(str_json);
    free(str_json);

    strcpy(calculatorLayout.winTitle, cJSON_GetObjectItem(layout_data, "winTitle")->valuestring);
    calculatorLayout.winWidth = cJSON_GetObjectItem(layout_data, "winWidth")->valuedouble;
    calculatorLayout.winHeight = cJSON_GetObjectItem(layout_data, "winHeight")->valuedouble;
    calculatorLayout.btn_prop =
        cJSON_GetObjectItem(cJSON_GetObjectItem(layout_data, "propotion"), "btn")->valuedouble;
    calculatorLayout.expr_prop =
        cJSON_GetObjectItem(cJSON_GetObjectItem(layout_data, "propotion"), "expr")->valuedouble;
    calculatorLayout.result_prop =
        cJSON_GetObjectItem(cJSON_GetObjectItem(layout_data, "propotion"), "result")->valuedouble;
    calculatorLayout.btn_font_size =
        cJSON_GetObjectItem(cJSON_GetObjectItem(layout_data, "font"), "btn")->valueint;
    calculatorLayout.expr_font_size =
        cJSON_GetObjectItem(cJSON_GetObjectItem(layout_data, "font"), "expr")->valueint;
    calculatorLayout.result_font_size =
        cJSON_GetObjectItem(cJSON_GetObjectItem(layout_data, "font"), "result")->valueint;
    calculatorLayout.fontname = CopyString(
        cJSON_GetObjectItem(cJSON_GetObjectItem(layout_data, "font"), "name")->valuestring);

    cJSON *columnTable = cJSON_GetObjectItem(layout_data, "column");
    calculatorLayout.column1 = cJSON_GetArrayItem(columnTable, 0)->valueint;
    calculatorLayout.column2 = cJSON_GetArrayItem(columnTable, 1)->valueint;

    cJSON *rowTable = cJSON_GetObjectItem(layout_data, "row");
    calculatorLayout.row1 = cJSON_GetArrayItem(rowTable, 0)->valueint;
    calculatorLayout.row2 = cJSON_GetArrayItem(rowTable, 1)->valueint;

    calculatorLayout.expr = CopyString("");
    calculatorLayout.expr_len = 0;
    calculatorLayout.res = CopyString("0");

    int pos = 0;
    calculatorLayout.buttonTable1[pos++] = (buttonT){"M+", "", memAdd};
    calculatorLayout.buttonTable1[pos++] = (buttonT){"M-", "", memSub};
    calculatorLayout.buttonTable1[pos++] = (buttonT){"M", "", memUse};
    calculatorLayout.buttonTable1[pos++] = (buttonT){"MC", "", memClear};
    calculatorLayout.buttonTable1[pos++] = (buttonT){"CE", "", clearExpr};
    calculatorLayout.buttonTable1[pos++] = (buttonT){"DEL", "", delExpr};
    calculatorLayout.buttonTable1[pos++] = (buttonT){"poly", "", convertMode};

    cJSON *button1Array = cJSON_GetObjectItem(layout_data, "button1");
    int size1 = cJSON_GetArraySize(button1Array);
    for (int i = 0; i < size1; ++i) {
        cJSON *tempNode = cJSON_GetArrayItem(button1Array, i);
        if (cJSON_GetObjectItem(tempNode, "output") != NULL) {
            calculatorLayout.buttonTable1[pos].display =
                CopyString(cJSON_GetObjectItem(tempNode, "display")->valuestring);
            calculatorLayout.buttonTable1[pos].output =
                CopyString(cJSON_GetObjectItem(tempNode, "output")->valuestring);
            calculatorLayout.buttonTable1[pos].execute =
                generalButtonExec; // 草.. 同一个地方跌倒两次
            pos++;

        } else {
            calculatorLayout.buttonTable1[pos].display =
                CopyString(cJSON_GetObjectItem(tempNode, "display")->valuestring);
            calculatorLayout.buttonTable1[pos].output =
                CopyString(cJSON_GetObjectItem(tempNode, "display")->valuestring);
            calculatorLayout.buttonTable1[pos].execute =
                generalButtonExec; // 写成Table2了，debug了一个多小时（
            pos++;
        }
    }
    calculatorLayout.buttonTable1[pos] = (buttonT){"=", "", arithCalc};

    pos = 0;
    calculatorLayout.buttonTable2[pos++] = (buttonT){"Arith", "", convertMode};
    calculatorLayout.buttonTable2[pos++] = (buttonT){"X", "", multi_poly};
    calculatorLayout.buttonTable2[pos++] = (buttonT){"+", "", add_poly};
    calculatorLayout.buttonTable2[pos++] = (buttonT){"-", "", sub_poly};
    calculatorLayout.buttonTable2[pos++] = (buttonT){"=", "", polyCalc};

    cJSON *button2Array = cJSON_GetObjectItem(layout_data, "button2");
    int size2 = cJSON_GetArraySize(button2Array);

    for (int i = 0; i < size2; ++i) {
        cJSON *tempNode = cJSON_GetArrayItem(button2Array, i);
        if (cJSON_GetObjectItem(tempNode, "output") != NULL) {
            calculatorLayout.buttonTable2[pos].display =
                CopyString(cJSON_GetObjectItem(tempNode, "display")->valuestring);
            calculatorLayout.buttonTable2[pos].output =
                CopyString(cJSON_GetObjectItem(tempNode, "output")->valuestring);
            calculatorLayout.buttonTable2[pos].execute = generalButtonExec;
            pos++;

        } else {
            calculatorLayout.buttonTable2[pos].display =
                CopyString(cJSON_GetObjectItem(tempNode, "display")->valuestring);
            calculatorLayout.buttonTable2[pos].output =
                CopyString(cJSON_GetObjectItem(tempNode, "display")->valuestring);
            calculatorLayout.buttonTable2[pos].execute = generalButtonExec;
            pos++;
        }
    }
}

static double mem;

static void generalButtonExec(buttonT *this) {
    int len = strlen(this->output);
    calculatorLayout.expr_len += len;
    string temp = calculatorLayout.expr;
    calculatorLayout.expr = Concat(temp, this->output);
    free(temp);
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
    calculatorLayout.expr_len += len;
    string temp = calculatorLayout.expr;
    calculatorLayout.expr = Concat(temp, numChar);
    free(numChar);
}

static void memClear(buttonT *this) {
    mem = 0;
}

static void clearExpr(buttonT *this) {
    free(calculatorLayout.expr);
    calculatorLayout.expr = CopyString("");
    calculatorLayout.expr_len = 0;
}

static void delExpr(buttonT *this) {
    string temp = calculatorLayout.expr;
    if (strlen(temp) == 0) return;
    temp[--calculatorLayout.expr_len] = '\0';
    calculatorLayout.expr = CopyString(temp);
    free(temp);
}

static void convertMode(buttonT *this) {
    clearExpr(this);
    calculatorLayout.mode = 1 - calculatorLayout.mode;
}

static void arithCalc(buttonT *this) {
    double result;
    Calc(calculatorLayout.expr, &result);
    string temp = calculatorLayout.res;
    calculatorLayout.res = RealToString(result);
    free(temp);
    clearExpr(this);
}


/**
 * @brief 静态全局变量 多项式缓冲区
 * 
 */
static linkedlistADT poly1, poly2;


/**
 * @brief 标识现在正在读入哪一个多项式(poly1/poly2)
 * 
 */
static int which_poly = 1;

/**
 * @brief 枚举: 多项式操作的种类 poly_calc_type
 * 
 */
typedef enum poly_calc_type {NOTHING, ADD, SUB, MULTI } poly_calc_type;
poly_calc_type poly_operation;

/**
 * @brief 三个多项式运算函数有相同的类型
 * 
 */
typedef linkedlistADT (*polyOperationT)(linkedlistADT, linkedlistADT);

polyOperationT operationSelector(poly_calc_type operation_type) { // 返回值是一个函数指针
    switch (operation_type) {
    case ADD: return AddPolynomial; break;
    case SUB: return SubPolynomial; break;
    case MULTI: return MultiplyPolynomial; break;
    default: return NULL; break;
    }
}

static void multi_poly(buttonT *this) {
    string format = FormatPolynomial(calculatorLayout.expr);
    if (which_poly == 1) {
        poly1 = CreatePolynomial(format);
        poly_operation = MULTI;
    } else {
        poly2 = CreatePolynomial(format);
    }
    which_poly = 3 - which_poly; // switch between 1 and 2
    if (which_poly == 1) {
        linkedlistADT res = operationSelector(poly_operation)(poly1, poly2); // operationSelector(poly_operation)是一个函数指针(函数名)
        string temp = calculatorLayout.res;
        calculatorLayout.res = PolynomialToString(res);
        free(temp);
        FreeLinkedList(poly1);
        FreeLinkedList(poly2);
        poly1 = res;
        poly_operation = MULTI;
        which_poly = 2;
    }
    clearExpr(this);
}

static void add_poly(buttonT *this) {
    string format = FormatPolynomial(calculatorLayout.expr);
    if (which_poly == 1) {
        poly1 = CreatePolynomial(format);
        poly_operation = ADD;
    } else {
        poly2 = CreatePolynomial(format);
    }
    which_poly = 3 - which_poly; // switch between 1 and 2
    if (which_poly == 1) {
        linkedlistADT res = operationSelector(poly_operation)(poly1, poly2); 
        string temp = calculatorLayout.res;
        calculatorLayout.res = PolynomialToString(res);
        free(temp);
        FreeLinkedList(poly1);
        FreeLinkedList(poly2);
        poly1 = res;
        poly_operation = ADD;
        which_poly = 2;
    }
    clearExpr(this);
}

static void sub_poly(buttonT *this) {
    string format = FormatPolynomial(calculatorLayout.expr);
    if (which_poly == 1) {
        poly1 = CreatePolynomial(format);
        poly_operation = SUB;
    } else {
        poly2 = CreatePolynomial(format);
    }
    which_poly = 3 - which_poly; // switch between 1 and 2
    if (which_poly == 1) {
        linkedlistADT res = operationSelector(poly_operation)(poly1, poly2); 
        string temp = calculatorLayout.res;
        calculatorLayout.res = PolynomialToString(res);
        free(temp);
        FreeLinkedList(poly1);
        FreeLinkedList(poly2);
        poly1 = res;
        poly_operation = SUB;
        which_poly = 2;
    }
    clearExpr(this);
}

static void polyCalc(buttonT *this) {
    if (which_poly == 2) {
        string format = FormatPolynomial(calculatorLayout.expr);
        poly2 = CreatePolynomial(format);
        linkedlistADT res = operationSelector(poly_operation)(poly1, poly2);
        string temp = calculatorLayout.res;
        calculatorLayout.res = PolynomialToString(res);
        free(temp);
        FreeLinkedList(poly1);
        FreeLinkedList(poly2);
        // FreeLinkedList(res); // ??? 内存泄漏和闪退二选一
        which_poly = 1;
    }
    clearExpr(this);
}