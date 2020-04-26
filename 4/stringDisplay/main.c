#define FUCK 114514
#ifdef FUCK          // F U C K
#include <windows.h> // why on earth does the clang-format sort the header
#endif               // thus causing problems because the include order is changed

#include "conio.h"
#include "extgraph.h"
#include "genlib.h"
#include "graphics.h"
#include "simpio.h"
#include <math.h>
#include <mmsystem.h>
#include <ocidl.h>
#include <ole2.h>
#include <olectl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <wingdi.h>
#include <winuser.h>

#define TIMER_BLINK500 1
#define TIMER_BLINK1000 2

static double textx, texty;
static double winHeight = 1.1, winWidth = 12;

static char buf[1000] = "", temp[1000] = "";
static int textlen = 0, crusor = 0, isDisplay = 0;

void eraseAll(void);
void DrawCrusor(int crusor);
void strcut(string str, string des, int len);

void KeyboardEventProcess(int key, int event);
void CharEventProcess(char c);
void TimerEventProcess(int timerID);

void Main() {
    SetWindowTitle("(没完成的)输入框");
    SetWindowSize(winWidth, winHeight);
    InitGraphics();
    registerKeyboardEvent(KeyboardEventProcess);
    registerCharEvent(CharEventProcess);
    registerTimerEvent(TimerEventProcess);

    SetPenColor("Black");
    SetPenSize(2);
    SetFont("Arial");
    SetPointSize(100);

    textx = 0;
    texty = winHeight / 2 - 0.3;
}

/*每当产生键盘消息，都要执行*/
void KeyboardEventProcess(int key, int event) {

    switch (event) {
    case KEY_DOWN:
        switch (key) {
        case VK_LEFT: /*左移键*/
            if (crusor > 0) {
                crusor--;
            }
            break;
        case VK_RIGHT: /*右移键*/
            if (crusor < textlen) {
                crusor++;
            }
            break;
        }
        break;
    case KEY_UP:
        break;
    }
}

void CharEventProcess(char c) {
    switch (c) {
    case '\r': /* 注意：回车在这里返回的字符是'\r'，不是'\n'*/
        OpenConsole();
        printf(buf);
        // ExitGraphics();
        break;
    case 27: /* ESC */
        ExitGraphics();
        break;
    case '\b': /* BackSpace */
        eraseAll();
        
        textlen--;
        buf[textlen] = '\0';
        MovePen(textx, texty); /*设置文本显示起始坐标*/
        SetEraseMode(FALSE);   /*取消擦除模式*/
        DrawTextString(buf);   /*显示当前字符串*/
        break;
    case 0x2e: /* Delete */

        break;
    default:
        eraseAll();
        buf[textlen++] = c; /*将当前字符加入到整个字符缓冲区中*/
        buf[textlen] = '\0';
        MovePen(textx, texty); /*设置文本显示起始坐标*/
        SetEraseMode(FALSE);   /*取消擦除模式*/
        DrawTextString(buf);   /*显示当前字符串*/
        break;
    }
}

/*timerID为定时器号*/
void TimerEventProcess(int timerID) {
    bool erasemode;

    switch (timerID) {
    case TIMER_BLINK500:
        erasemode = GetEraseMode();
        SetEraseMode(isDisplay);
        DrawCrusor(crusor);
        SetEraseMode(erasemode);
        isDisplay = !isDisplay;
        break;
    default:
        break;
    }
}

void eraseAll(void) {
    SaveGraphicsState();
    SetEraseMode(true);
    StartFilledRegion(0);
    MovePen(0, 0);
    DrawLine(0.0, winHeight);
    DrawLine(winWidth, 0.0);
    DrawLine(0.0, -winHeight);
    DrawLine(-winWidth, 0.0);
    EndFilledRegion();
    SetEraseMode(false);
    RestoreGraphicsState();
}

void strcut(string str, string des, int len) {
    for (int i = 0; i < len; ++i) {
        des[i] = str[i];
    }
    des[len] = '\0';
}

void DrawCrusor(int crusor) {
    strcut(buf, temp, crusor);
    double width = TextStringWidth(temp);
    MovePen(width, 0.1);
    DrawLine(0.0, 0.9);
}