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

void KeyboardEventProcess(int key, int event) /*ÿ������������Ϣ����Ҫִ��*/
{
    double oldradius;

    switch (event) {
    case KEY_DOWN:
        switch (key) {
        case VK_LEFT: /*���Ƽ�*/

            break;
        case VK_RIGHT: /*���Ƽ�*/

            break;
        }
        break;
    case KEY_UP:
        break;
    }
}

void CharEventProcess(char c) {
    switch (c) {
    case '\r': /* ע�⣺�س������ﷵ�ص��ַ���'\r'������'\n'*/
        break;
    case 27: /*ESC*/
        break;
    case '\b': /*BackSpace*/
        break;
    default:
        MovePen(textx, texty); /*�����ı���ʾ��ʼ����*/
        SetEraseMode(TRUE);    /*���ò���ģʽ*/
        DrawTextString(buf);  /*������ǰ�ַ���*/
        text[strlen++] = c;   /*����ǰ�ַ����뵽�����ַ���������*/
        text[strlen] = '\0';
        MovePen(textx, texty); /*�����ı���ʾ��ʼ����*/
        SetEraseMode(FALSE);   /*ȡ������ģʽ*/
        DrawTextString(buf);  /*��ʾ��ǰ�ַ���*/
        break;
    }
}