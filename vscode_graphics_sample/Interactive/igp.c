#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#define deltax 0.03
#define deltay 0.03

#define TIMER_BLINK500  1     /*500ms��ʱ���¼���־��*/
#define TIMER_BLINK1000 2     /*1000ms��ʱ��ʱ���־��*/

const int mseconds500 = 500;   
const int mseconds1000 = 1000; 

static double ccx = 1.0, ccy = 1.0;/*Բ������*/
static double radius = 1.0; /*Բ�뾶*/

static char text[100] = {'\0'}; /*������ַ���������*/
static int textlen = 0;/*������ַ�������*/
static double textx, texty; /*�ַ�������ʼλ��*/

static bool isBlink = FALSE;   /*�Ƿ���˸��־*/
static bool isDisplayText = FALSE; /*�ַ�����ʾ��־*/
static bool isDisplayCircle = TRUE; /*Բ��ʾ��־*/

void DrawCenteredCircle(double x, double y, double r);/*������Բ*/
/*�жϵ�(x0,y0)�Ƿ��ھ��ΰ�Χ��(x1, y1) --> (x2, y2)��Χ��*/
bool inBox(double x0, double y0, double x1, double x2, double y1, double y2);
bool CrossRect(double x11, double y11, double x12, double y12, 
               double x21, double y21, double x22, double y22);/*�ж����������Ƿ��ཻ*/
void KeyboardEventProcess(int key,int event);/*������Ϣ�ص�����*/
void CharEventProcess(char c);/*�ַ���Ϣ�ص�����*/
void MouseEventProcess(int x, int y, int button, int event);/*�����Ϣ�ص�����*/
void TimerEventProcess(int timerID);/*��ʱ����Ϣ�ص�����*/

void Main() /*��ʼ������ִ��һ��*/
{
    InitGraphics();        	
	
	registerKeyboardEvent(KeyboardEventProcess);/*ע�������Ϣ�ص�����*/
	registerCharEvent(CharEventProcess);/*ע���ַ���Ϣ�ص�����*/
	registerMouseEvent(MouseEventProcess);/*ע�������Ϣ�ص�����*/
	registerTimerEvent(TimerEventProcess);/*ע�ᶨʱ����Ϣ�ص�����*/

	SetPenColor("Red"); /*���û�����ɫ*/
    SetPenSize(1);  /*���û��ʴ�ϸ*/
    
    ccx = GetWindowWidth()/2; /*����Բ������Ϊ����������λ��*/
    ccy = GetWindowHeight()/2;
    DrawCenteredCircle(ccx, ccy, radius); /*��Բ*/
    textx = GetCurrentX(); /*������굱ǰ����Ϊ�ı���ʾ��ʼλ��*/
    texty = GetCurrentY();
}

/*key��ʾ�ĸ����������룬eventΪ����״̬�����»���̧��*/
void KeyboardEventProcess(int key, int event)/*ÿ������������Ϣ����Ҫִ��*/
{
	double oldradius;
 	 
	switch (event) {
		case KEY_DOWN:
			switch (key) {
				case VK_UP:/*���Ƽ�*/
			    	SetEraseMode(TRUE);/*����ǰһ��*/
                    DrawCenteredCircle(ccx, ccy, radius);
					ccy += deltay; /*����Բ��λ��*/
					SetEraseMode(FALSE);/*���µ�*/
                    DrawCenteredCircle(ccx, ccy, radius);
                    break;
			    case VK_DOWN: /*���Ƽ�*/
			        SetEraseMode(TRUE);
                    DrawCenteredCircle(ccx, ccy, radius);
					ccy -= deltay; /*����Բ��λ��*/
					SetEraseMode(FALSE);
                    DrawCenteredCircle(ccx, ccy, radius);
                    break;
			    case VK_LEFT: /*���Ƽ�*/
			        SetEraseMode(TRUE);
                    DrawCenteredCircle(ccx, ccy, radius);
					ccx -= deltax; /*����Բ��λ��*/
					SetEraseMode(FALSE);
                    DrawCenteredCircle(ccx, ccy, radius);
                    break;
			    case VK_RIGHT: /*���Ƽ�*/
			        SetEraseMode(TRUE);
                    DrawCenteredCircle(ccx, ccy, radius);
					ccx += deltax; /*����Բ��λ��*/
					SetEraseMode(FALSE);
                    DrawCenteredCircle(ccx, ccy, radius);
                    break;
			    case VK_F1:
  					SetEraseMode(TRUE);
                    DrawCenteredCircle(ccx, ccy, radius);
		 	        SetPenSize(GetPenSize()+1); /*����ʻ���ϸ*/
					SetEraseMode(FALSE);
                    DrawCenteredCircle(ccx, ccy, radius);
					break;
			    case VK_F2:
  					SetEraseMode(TRUE);
                    DrawCenteredCircle(ccx, ccy, radius);
		 	        SetPenSize(GetPenSize()-1); /*��С�ʻ���ϸ*/
					SetEraseMode(FALSE);
                    DrawCenteredCircle(ccx, ccy, radius);
                    break;
			    case VK_F3:
			    case VK_PRIOR:
	 	     		SetEraseMode(TRUE);
                    DrawCenteredCircle(ccx, ccy, radius);
                    radius += 0.1; /*�뾶����*/
					SetEraseMode(FALSE);
                    DrawCenteredCircle(ccx, ccy, radius);
                    break;
			    case VK_F4:
			    case VK_NEXT:
		 	        SetEraseMode(TRUE);
                    DrawCenteredCircle(ccx, ccy, radius);
                    radius -= 0.1; /*�뾶��С*/
					SetEraseMode(FALSE);
                    DrawCenteredCircle(ccx, ccy, radius);
                    break;
			    case VK_F9: /*�ӿ���̨����Բ�뾶*/
			        OpenConsole(); /*�򿪿���̨*/
			        oldradius = radius;
			        printf("Input radius: ");
			        radius = GetReal();
			        CloseConsole();/*�رտ���̨*/
  					SetEraseMode(TRUE);
                    DrawCenteredCircle(ccx, ccy, oldradius); /*����ԭ�뾶��Բ*/
					SetEraseMode(FALSE);
                    DrawCenteredCircle(ccx, ccy, radius); /*�ػ��°뾶��Բ*/
                    break;
			    case VK_ESCAPE:/*������ȡ����ʱ��*/
                    isBlink = !isBlink; /*�л���˸״̬*/
                    if (isBlink ) { /*��˸*/
						startTimer(TIMER_BLINK500, mseconds500);/*500ms��ʱ������*/
						startTimer(TIMER_BLINK1000, mseconds1000);/*1000ms��ʱ������*/
                     } else { /*����˸*/
						cancelTimer(TIMER_BLINK500);/*500ms��ʱ���ر�*/
    					cancelTimer(TIMER_BLINK1000);/*500ms��ʱ���ر�*/
					 }
                     break;
			}
			break;
		case KEY_UP:
			break;
	}	 
}

/*cΪ�ַ�ASCII��*/
void CharEventProcess(char c)
{
   switch (c) {
    	case '\r':  /* ע�⣺�س������ﷵ�ص��ַ���'\r'������'\n'*/
   	 	   	break;
 		case 27: /*ESC*/
 	       	break;
 	    case '\b': /*BackSpace*/
		 	break; 
    	default:
    		MovePen(textx, texty);/*�����ı���ʾ��ʼ����*/
    		SetEraseMode(TRUE); /*���ò���ģʽ*/
	 	   	DrawTextString(text); /*������ǰ�ַ���*/
	 	   	text[textlen++] = c;/*����ǰ�ַ����뵽�����ַ���������*/
	 	   	text[textlen] = '\0';
    		MovePen(textx, texty);/*�����ı���ʾ��ʼ����*/
    		SetEraseMode(FALSE); /*ȡ������ģʽ*/
	 	   	DrawTextString(text); /*��ʾ��ǰ�ַ���*/
			isDisplayText = TRUE;/*�����ַ�����ʾ��־*/
	 	   	break;
    }
}

/*(x,y)Ϊ��굱ǰ��Ļ���꣬��λ���أ�button��갴ť��eventΪ��갴��״̬*/
void MouseEventProcess(int x, int y, int button, int event)
{
	static bool isMoveCircle = FALSE;/*�ƶ�Բ��־*/
 	static bool isMoveText = FALSE; /*�ƶ��ı���־*/ 
	static bool isChangeRadius = FALSE;/*�ı�Բ�뾶��־*/
	static double omx = 0.0, omy = 0.0; /*ǰһ�����λ��*/
    double mx, my;

 	mx = ScaleXInches(x);/*���λ�ã�����Ļ����ת��Ϊ�豸���꣬pixels --> inches*/
 	my = ScaleYInches(y);/*pixels --> inches*/

    switch (event) {
    	case BUTTON_DOWN:
   			if (button == LEFT_BUTTON) { /*��ť*/
				if (inBox(mx, my, ccx-radius, ccx+radius, ccy-radius, ccy+radius)){
					isMoveCircle = TRUE; /*�����ƶ�Բ��־*/
				} else if (inBox(mx, my, textx, textx+TextStringWidth(text), 
				                           texty, texty+GetFontHeight())){
				  	isMoveText = TRUE;  /*�����ƶ��ı���־*/
				}
			} else if (button == RIGHT_BUTTON) { /*�Ұ�ť*/
				isChangeRadius = TRUE; /*���øı�Բ�뾶��־*/
			}
		    omx = mx; omy = my;
            break;
        case BUTTON_UP:
  		 	if (button == LEFT_BUTTON) {
  		 		isMoveCircle = FALSE;/*ȡ��Բ�ƶ���־*/
  		 	  	isMoveText = FALSE; /*ȡ���ı��ƶ���־*/
  		 	} else if (button == RIGHT_BUTTON) {
  		 		isChangeRadius = FALSE; /*ȡ���ı�Բ�뾶��־*/
  		 	} 
            break;
        case MOUSEMOVE: /*����ƶ�*/
			if (isMoveCircle) { /*�ƶ�Բ*/
                SetEraseMode(TRUE);/*����ǰһ��*/
                DrawCenteredCircle(ccx, ccy, radius);
				ccx += mx - omx;
				ccy += my - omy;
				omx = mx;
				omy = my;
				SetEraseMode(FALSE);/*���µ�*/
                DrawCenteredCircle(ccx, ccy, radius);
			} else if (isChangeRadius) { /*�ı�Բ�뾶*/
            	SetEraseMode(TRUE);/*����ǰһ��*/
                DrawCenteredCircle(ccx, ccy, radius);
				radius += mx - omx;
				omx = mx;
				omy = my;
				SetEraseMode(FALSE);/*���µ�*/
                DrawCenteredCircle(ccx, ccy, radius);
			} else if (isMoveText) { /*�ƶ��ı�*/
				SetEraseMode(TRUE);
	          	MovePen(textx, texty);/*��ʼλ��*/
				DrawTextString(text);
	 			textx += mx - omx;
				texty += my - omy;
				omx = mx;
				omy = my;				
				SetEraseMode(FALSE);
				MovePen(textx, texty);/*��ʼλ��*/
				DrawTextString(text);
			}
							
			/*��Բ���ı��ཻ����ˢ��*/
			if ((isMoveCircle || isChangeRadius || isMoveText) && 
			    CrossRect(ccx-radius, ccy-radius, ccx+radius, ccy+radius,
				          textx, texty, textx+TextStringWidth(text), texty+GetFontHeight())) {
				MovePen(textx, texty);/*�ı���ʼλ��*/
				DrawTextString(text);/*ˢ���ı�*/
				DrawCenteredCircle(ccx, ccy, radius);/*ˢ��Բ*/
			} 
            break;
    }	
}

/*timerIDΪ��ʱ����*/
void TimerEventProcess(int timerID)
{
	bool erasemode;

	switch (timerID) {
		case TIMER_BLINK500: /*500ms�ı���˸��ʱ��*/
			if (!isBlink) return; /*δ������˸��־*/
	    	erasemode = GetEraseMode();
          	MovePen(textx, texty);/*��ʼλ��*/
		  	SetEraseMode(isDisplayText);/*���ݵ�ǰ��ʾ��־����������ʾ���������ַ���*/
		  	DrawTextString(text);/*��ǰλ�û����ַ�������*/
	      	SetEraseMode(erasemode);
		  	isDisplayText = !isDisplayText;/*������ʾ/�����ַ�������*/
		  	break;
		case TIMER_BLINK1000: /*1000msԲ��˸��ʱ��*/
			if (!isBlink) return; /*δ������˸��־*/
	    	erasemode = GetEraseMode();
			SetEraseMode(isDisplayCircle);
        	DrawCenteredCircle(ccx, ccy, radius);
	    	SetEraseMode(erasemode);
			isDisplayCircle = !isDisplayCircle;
			break;
		default:
			break;
	}
}

/*��(x,y)ΪԲ�ģ�rΪ�뾶������Բ*/
void DrawCenteredCircle(double x, double y, double r)
{
    MovePen(x + r, y);
    DrawArc(r, 0.0, 360.0);
}

/*�жϵ�(x0,y0)�Ƿ��ھ���(x1,y1,x2,y2)��*/
bool inBox(double x0, double y0, double x1, double x2, double y1, double y2)
{
	return (x0 >= x1 && x0 <= x2 && y0 >= y1 && y0 <= y2);
}

/*�ж����������Ƿ��ཻ*/ 
bool CrossRect(double x11, double y11, double x12, double y12, 
               double x21, double y21, double x22, double y22)
{
	if (fabs((x11+x12)/2-(x21+x22)/2) < fabs((x12+x22-x11-x21)/2) &&
	    fabs((y11+y12)/2-(y21+y22)/2) < fabs((y12+y22-y11-y21)/2))
	    return TRUE;
	return FALSE;
}
