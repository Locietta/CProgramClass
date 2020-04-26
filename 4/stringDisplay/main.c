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
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <wingdi.h>
#include <winuser.h>

static double textx, texty;
static double winHeight = 0, winWidth = 0;

string buf[1000] = "";
static int strlen = 0;

void Main() {
    InitGraphics();

    registerKeyboardEvent(KeyboardEventProcess);
    registerCharEvent(CharEventProcess);
    registerMouseEvent(MouseEventProcess);
    registerTimerEvent(TimerEventProcess);

    SetPenColor("Black");
    SetPenSize(2);

    winHeight = GetWindowHeight();
    winWidth = GetWindowHeight();
    SetFont(20);

    textx = 0;
    texty = winWidth/2;
}

void KeyboardEventProcess(int key, int event) /*每当产生键盘消息，都要执行*/
{
    double oldradius;

    switch (event) {
    case KEY_DOWN:
        switch (key) {
        case VK_LEFT: /*左移键*/

            break;
        case VK_RIGHT: /*右移键*/

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
        break;
    case 27: /*ESC*/
        break;
    case '\b': /*BackSpace*/
        break;
    default:
        MovePen(textx, texty); /*设置文本显示起始坐标*/
        SetEraseMode(TRUE);    /*设置擦除模式*/
        DrawTextString(buf);  /*擦除当前字符串*/
        text[strlen++] = c;   /*将当前字符加入到整个字符缓冲区中*/
        text[strlen] = '\0';
        MovePen(textx, texty); /*设置文本显示起始坐标*/
        SetEraseMode(FALSE);   /*取消擦除模式*/
        DrawTextString(buf);  /*显示当前字符串*/
        break;
    }
}