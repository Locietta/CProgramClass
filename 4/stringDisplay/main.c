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

#define TIMER_BLINK350 1
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
    SetWindowTitle("�����");
    SetWindowSize(winWidth, winHeight);
    InitGraphics();
    registerKeyboardEvent(KeyboardEventProcess);
    registerCharEvent(CharEventProcess);
    registerTimerEvent(TimerEventProcess);

    SetPenColor("Black");
    SetPenSize(2);
    SetFont("Arial");
    SetPointSize(100);

    startTimer(TIMER_BLINK350, 350);
    textx = 0;
    texty = winHeight / 2 - 0.3;
}

void KeyboardEventProcess(int key, int event) {

    switch (event) {
    case KEY_DOWN:
        switch (key) {
        case VK_LEFT: 
            if (crusor > 0) {
                bool erasemode = GetEraseMode();
                SetEraseMode(true);
                DrawCrusor(crusor);
                SetEraseMode(erasemode);
                crusor--;
            }
            break;
        case VK_RIGHT: 
            if (crusor < textlen) {
                bool erasemode = GetEraseMode();
                SetEraseMode(true);
                DrawCrusor(crusor);
                SetEraseMode(erasemode);
                crusor++;
            }
            break;
        case VK_DELETE: 
            if (crusor < textlen) {
                eraseAll();
                for (int i = crusor; i < textlen - 1; ++i) {
                    buf[i] = buf[i + 1];
                }
                textlen--;
                buf[textlen] = '\0';
                MovePen(textx, texty); 
                SetEraseMode(FALSE);   
                DrawTextString(buf);   
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
    case '\r': /* Windows : [Enter]=='\r''\n' */
        OpenConsole();
        printf(buf);
        // ExitGraphics();
        break;
    case 27: /* ESC */
        ExitGraphics();
        break;
    case '\b': /* BackSpace */
        if (crusor) {
            eraseAll();
            for (int i = crusor - 1; i < textlen - 1; ++i) {
                buf[i] = buf[i + 1];
            }
            textlen--;
            crusor--;
            buf[textlen] = '\0';
            MovePen(textx, texty); 
            SetEraseMode(FALSE);   
            DrawTextString(buf);   
        }
        break;
    default:
        eraseAll();
        for (int i = textlen; i > crusor; --i) {
            buf[i] = buf[i-1]; 
        }
        buf[crusor++] = c;
        buf[++textlen] = '\0';
        MovePen(textx, texty); 
        SetEraseMode(FALSE);   
        DrawTextString(buf);   
        break;
    }
}

void TimerEventProcess(int timerID) {
    bool erasemode;

    switch (timerID) {
    case TIMER_BLINK350:
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