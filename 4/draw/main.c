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
    SetWindowTitle("��ʻ�: ��㻭��ʲô");
    registerMouseEvent(MouseEventProcess); /*ע�������Ϣ�ص�����*/

    SetPenColor("Red"); /*���û�����ɫ*/
    SetPenSize(1);      /*���û��ʴ�ϸ*/
}

/*(x,y)Ϊ��굱ǰ��Ļ���꣬��λ���أ�button��갴ť��eventΪ��갴��״̬*/
void MouseEventProcess(int x, int y, int button, int event) {
    static bool drawFlag = false;
    static double omx = 0.0, omy = 0.0; /*ǰһ�����λ��*/
    double mx, my;

    mx = ScaleXInches(x); /*���λ�ã�����Ļ����ת��Ϊ�豸���꣬pixels --> inches*/
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
