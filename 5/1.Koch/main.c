#include "koch.h"

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

void Main() {
    OpenConsole();
    printf("请输入Koch曲线阶数:");
    int n = GetInteger();
    printf("请输入Koch曲线单位长度:");
    double a = GetReal();
    CloseConsole();
    
    InitGraphics();
    SetWindowTitle("Koch雪花曲线");
    SetPenColor("Black"); 
    SetPenSize(2);     
    paintKoch(a, n, (coord) {.x = GetWindowWidth()/2, .y=GetWindowHeight()/2});
}
