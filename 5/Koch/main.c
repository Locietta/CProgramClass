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
    printf("������Koch���߽���:");
    int n = GetInteger();
    printf("������Koch���ߵ�λ����:");
    double a = GetReal();
    CloseConsole();
    
    InitGraphics();
    SetWindowTitle("Kochѩ������");
    SetPenColor("Black"); 
    SetPenSize(2);     
    paintKoch(a, n, (coord) {.x = GetWindowWidth()/2, .y=GetWindowHeight()/2});
}
