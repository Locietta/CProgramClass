#include "conio.h"
#include "extgraph.h"
#include "genlib.h"
#include "graphics.h"
#include "simpio.h"
#include <windows.h>
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

#define deltax 0.03
#define deltay 0.03

void MouseEventProcess(int x, int y, int button, int event);

void Main() {
    InitGraphics();
    SetWindowTitle("随笔画: 随便画点什么");
    registerMouseEvent(MouseEventProcess); /*注册鼠标消息回调函数*/

    SetPenColor("Red"); /*设置画笔颜色*/
    SetPenSize(1);      /*设置画笔粗细*/
}

/*(x,y)为鼠标当前屏幕坐标，单位像素；button鼠标按钮；event为鼠标按键状态*/
void MouseEventProcess(int x, int y, int button, int event) {
    static bool drawFlag = false;
    static double omx = 0.0, omy = 0.0; /*前一次鼠标位置*/
    double mx, my;

    mx = ScaleXInches(x); /*鼠标位置：将屏幕坐标转换为设备坐标，pixels --> inches*/
    my = ScaleYInches(y); /*pixels --> inches*/

    switch (event) {
    case BUTTON_DOWN:
        drawFlag = true;
        break;
    case BUTTON_UP:
        drawFlag = false;
        break;
    case MOUSEMOVE:
        if (drawFlag) {
            MovePen(omx,omy);
            DrawLine(mx-omx,my-omy);
        }
        omx = mx;
        omy = my;
        break;
    }
}
