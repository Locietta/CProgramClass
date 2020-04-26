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

#define TIMER_BLINK500  1     /*500ms定时器事件标志号*/
#define TIMER_BLINK1000 2     /*1000ms定时器时间标志号*/

const int mseconds500 = 500;   
const int mseconds1000 = 1000; 

static double ccx = 1.0, ccy = 1.0;/*圆心坐标*/
static double radius = 1.0; /*圆半径*/

static char text[100] = {'\0'}; /*输入的字符串缓冲区*/
static int textlen = 0;/*输入的字符串长度*/
static double textx, texty; /*字符串的起始位置*/

static bool isBlink = FALSE;   /*是否闪烁标志*/
static bool isDisplayText = FALSE; /*字符串显示标志*/
static bool isDisplayCircle = TRUE; /*圆显示标志*/

void DrawCenteredCircle(double x, double y, double r);/*画中心圆*/
/*判断点(x0,y0)是否在矩形包围盒(x1, y1) --> (x2, y2)范围内*/
bool inBox(double x0, double y0, double x1, double x2, double y1, double y2);
bool CrossRect(double x11, double y11, double x12, double y12, 
               double x21, double y21, double x22, double y22);/*判断两个矩形是否相交*/
void KeyboardEventProcess(int key,int event);/*键盘消息回调函数*/
void CharEventProcess(char c);/*字符消息回调函数*/
void MouseEventProcess(int x, int y, int button, int event);/*鼠标消息回调函数*/
void TimerEventProcess(int timerID);/*定时器消息回调函数*/

void Main() /*初始化，仅执行一次*/
{
    InitGraphics();        	
	
	registerKeyboardEvent(KeyboardEventProcess);/*注册键盘消息回调函数*/
	registerCharEvent(CharEventProcess);/*注册字符消息回调函数*/
	registerMouseEvent(MouseEventProcess);/*注册鼠标消息回调函数*/
	registerTimerEvent(TimerEventProcess);/*注册定时器消息回调函数*/

	SetPenColor("Red"); /*设置画笔颜色*/
    SetPenSize(1);  /*设置画笔粗细*/
    
    ccx = GetWindowWidth()/2; /*设置圆心坐标为窗口正中心位置*/
    ccy = GetWindowHeight()/2;
    DrawCenteredCircle(ccx, ccy, radius); /*画圆*/
    textx = GetCurrentX(); /*设置鼠标当前坐标为文本显示起始位置*/
    texty = GetCurrentY();
}

/*key表示哪个键—虚拟码，event为按键状态—按下还是抬起*/
void KeyboardEventProcess(int key, int event)/*每当产生键盘消息，都要执行*/
{
	double oldradius;
 	 
	switch (event) {
		case KEY_DOWN:
			switch (key) {
				case VK_UP:/*上移键*/
			    	SetEraseMode(TRUE);/*擦除前一个*/
                    DrawCenteredCircle(ccx, ccy, radius);
					ccy += deltay; /*上移圆心位置*/
					SetEraseMode(FALSE);/*画新的*/
                    DrawCenteredCircle(ccx, ccy, radius);
                    break;
			    case VK_DOWN: /*下移键*/
			        SetEraseMode(TRUE);
                    DrawCenteredCircle(ccx, ccy, radius);
					ccy -= deltay; /*下移圆心位置*/
					SetEraseMode(FALSE);
                    DrawCenteredCircle(ccx, ccy, radius);
                    break;
			    case VK_LEFT: /*左移键*/
			        SetEraseMode(TRUE);
                    DrawCenteredCircle(ccx, ccy, radius);
					ccx -= deltax; /*左移圆心位置*/
					SetEraseMode(FALSE);
                    DrawCenteredCircle(ccx, ccy, radius);
                    break;
			    case VK_RIGHT: /*右移键*/
			        SetEraseMode(TRUE);
                    DrawCenteredCircle(ccx, ccy, radius);
					ccx += deltax; /*右移圆心位置*/
					SetEraseMode(FALSE);
                    DrawCenteredCircle(ccx, ccy, radius);
                    break;
			    case VK_F1:
  					SetEraseMode(TRUE);
                    DrawCenteredCircle(ccx, ccy, radius);
		 	        SetPenSize(GetPenSize()+1); /*增大笔划粗细*/
					SetEraseMode(FALSE);
                    DrawCenteredCircle(ccx, ccy, radius);
					break;
			    case VK_F2:
  					SetEraseMode(TRUE);
                    DrawCenteredCircle(ccx, ccy, radius);
		 	        SetPenSize(GetPenSize()-1); /*减小笔划粗细*/
					SetEraseMode(FALSE);
                    DrawCenteredCircle(ccx, ccy, radius);
                    break;
			    case VK_F3:
			    case VK_PRIOR:
	 	     		SetEraseMode(TRUE);
                    DrawCenteredCircle(ccx, ccy, radius);
                    radius += 0.1; /*半径增大*/
					SetEraseMode(FALSE);
                    DrawCenteredCircle(ccx, ccy, radius);
                    break;
			    case VK_F4:
			    case VK_NEXT:
		 	        SetEraseMode(TRUE);
                    DrawCenteredCircle(ccx, ccy, radius);
                    radius -= 0.1; /*半径减小*/
					SetEraseMode(FALSE);
                    DrawCenteredCircle(ccx, ccy, radius);
                    break;
			    case VK_F9: /*从控制台输入圆半径*/
			        OpenConsole(); /*打开控制台*/
			        oldradius = radius;
			        printf("Input radius: ");
			        radius = GetReal();
			        CloseConsole();/*关闭控制台*/
  					SetEraseMode(TRUE);
                    DrawCenteredCircle(ccx, ccy, oldradius); /*擦除原半径的圆*/
					SetEraseMode(FALSE);
                    DrawCenteredCircle(ccx, ccy, radius); /*重画新半径的圆*/
                    break;
			    case VK_ESCAPE:/*启动或取消定时器*/
                    isBlink = !isBlink; /*切换闪烁状态*/
                    if (isBlink ) { /*闪烁*/
						startTimer(TIMER_BLINK500, mseconds500);/*500ms定时器触发*/
						startTimer(TIMER_BLINK1000, mseconds1000);/*1000ms定时器触发*/
                     } else { /*不闪烁*/
						cancelTimer(TIMER_BLINK500);/*500ms定时器关闭*/
    					cancelTimer(TIMER_BLINK1000);/*500ms定时器关闭*/
					 }
                     break;
			}
			break;
		case KEY_UP:
			break;
	}	 
}

/*c为字符ASCII码*/
void CharEventProcess(char c)
{
   switch (c) {
    	case '\r':  /* 注意：回车在这里返回的字符是'\r'，不是'\n'*/
   	 	   	break;
 		case 27: /*ESC*/
 	       	break;
 	    case '\b': /*BackSpace*/
		 	break; 
    	default:
    		MovePen(textx, texty);/*设置文本显示起始坐标*/
    		SetEraseMode(TRUE); /*设置擦除模式*/
	 	   	DrawTextString(text); /*擦除当前字符串*/
	 	   	text[textlen++] = c;/*将当前字符加入到整个字符缓冲区中*/
	 	   	text[textlen] = '\0';
    		MovePen(textx, texty);/*设置文本显示起始坐标*/
    		SetEraseMode(FALSE); /*取消擦除模式*/
	 	   	DrawTextString(text); /*显示当前字符串*/
			isDisplayText = TRUE;/*设置字符串显示标志*/
	 	   	break;
    }
}

/*(x,y)为鼠标当前屏幕坐标，单位像素；button鼠标按钮；event为鼠标按键状态*/
void MouseEventProcess(int x, int y, int button, int event)
{
	static bool isMoveCircle = FALSE;/*移动圆标志*/
 	static bool isMoveText = FALSE; /*移动文本标志*/ 
	static bool isChangeRadius = FALSE;/*改变圆半径标志*/
	static double omx = 0.0, omy = 0.0; /*前一次鼠标位置*/
    double mx, my;

 	mx = ScaleXInches(x);/*鼠标位置：将屏幕坐标转换为设备坐标，pixels --> inches*/
 	my = ScaleYInches(y);/*pixels --> inches*/

    switch (event) {
    	case BUTTON_DOWN:
   			if (button == LEFT_BUTTON) { /*左按钮*/
				if (inBox(mx, my, ccx-radius, ccx+radius, ccy-radius, ccy+radius)){
					isMoveCircle = TRUE; /*设置移动圆标志*/
				} else if (inBox(mx, my, textx, textx+TextStringWidth(text), 
				                           texty, texty+GetFontHeight())){
				  	isMoveText = TRUE;  /*设置移动文本标志*/
				}
			} else if (button == RIGHT_BUTTON) { /*右按钮*/
				isChangeRadius = TRUE; /*设置改变圆半径标志*/
			}
		    omx = mx; omy = my;
            break;
        case BUTTON_UP:
  		 	if (button == LEFT_BUTTON) {
  		 		isMoveCircle = FALSE;/*取消圆移动标志*/
  		 	  	isMoveText = FALSE; /*取消文本移动标志*/
  		 	} else if (button == RIGHT_BUTTON) {
  		 		isChangeRadius = FALSE; /*取消改变圆半径标志*/
  		 	} 
            break;
        case MOUSEMOVE: /*鼠标移动*/
			if (isMoveCircle) { /*移动圆*/
                SetEraseMode(TRUE);/*擦除前一个*/
                DrawCenteredCircle(ccx, ccy, radius);
				ccx += mx - omx;
				ccy += my - omy;
				omx = mx;
				omy = my;
				SetEraseMode(FALSE);/*画新的*/
                DrawCenteredCircle(ccx, ccy, radius);
			} else if (isChangeRadius) { /*改变圆半径*/
            	SetEraseMode(TRUE);/*擦除前一个*/
                DrawCenteredCircle(ccx, ccy, radius);
				radius += mx - omx;
				omx = mx;
				omy = my;
				SetEraseMode(FALSE);/*画新的*/
                DrawCenteredCircle(ccx, ccy, radius);
			} else if (isMoveText) { /*移动文本*/
				SetEraseMode(TRUE);
	          	MovePen(textx, texty);/*起始位置*/
				DrawTextString(text);
	 			textx += mx - omx;
				texty += my - omy;
				omx = mx;
				omy = my;				
				SetEraseMode(FALSE);
				MovePen(textx, texty);/*起始位置*/
				DrawTextString(text);
			}
							
			/*若圆与文本相交，则刷新*/
			if ((isMoveCircle || isChangeRadius || isMoveText) && 
			    CrossRect(ccx-radius, ccy-radius, ccx+radius, ccy+radius,
				          textx, texty, textx+TextStringWidth(text), texty+GetFontHeight())) {
				MovePen(textx, texty);/*文本起始位置*/
				DrawTextString(text);/*刷新文本*/
				DrawCenteredCircle(ccx, ccy, radius);/*刷新圆*/
			} 
            break;
    }	
}

/*timerID为定时器号*/
void TimerEventProcess(int timerID)
{
	bool erasemode;

	switch (timerID) {
		case TIMER_BLINK500: /*500ms文本闪烁定时器*/
			if (!isBlink) return; /*未设置闪烁标志*/
	    	erasemode = GetEraseMode();
          	MovePen(textx, texty);/*起始位置*/
		  	SetEraseMode(isDisplayText);/*根据当前显示标志来决定是显示还是隐藏字符串*/
		  	DrawTextString(text);/*当前位置会随字符串后移*/
	      	SetEraseMode(erasemode);
		  	isDisplayText = !isDisplayText;/*交替显示/隐藏字符串符号*/
		  	break;
		case TIMER_BLINK1000: /*1000ms圆闪烁定时器*/
			if (!isBlink) return; /*未设置闪烁标志*/
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

/*以(x,y)为圆心，r为半径画空心圆*/
void DrawCenteredCircle(double x, double y, double r)
{
    MovePen(x + r, y);
    DrawArc(r, 0.0, 360.0);
}

/*判断点(x0,y0)是否在矩形(x1,y1,x2,y2)内*/
bool inBox(double x0, double y0, double x1, double x2, double y1, double y2)
{
	return (x0 >= x1 && x0 <= x2 && y0 >= y1 && y0 <= y2);
}

/*判断两个矩形是否相交*/ 
bool CrossRect(double x11, double y11, double x12, double y12, 
               double x21, double y21, double x22, double y22)
{
	if (fabs((x11+x12)/2-(x21+x22)/2) < fabs((x12+x22-x11-x21)/2) &&
	    fabs((y11+y12)/2-(y21+y22)/2) < fabs((y12+y22-y11-y21)/2))
	    return TRUE;
	return FALSE;
}
