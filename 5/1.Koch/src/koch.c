#include "koch.h"
#include <math.h>
#include <limits.h>
#include "graphics.h"
#include "extgraph.h"


#define PI 3.14159265358979


/**
 * @brief 用长度和方向画线
 *
 * @param r         线的长度
 * @param theta     线的角度(与x轴正向成的逆时针角，角度制)
 */
static void DrawPolarLine(double r, double theta) {
    double radians;

    radians = theta / 180 * PI;
    DrawLine(r * cos(radians), r * sin(radians));
}

static void paintKochLine(double a, int level, double theta) {
    if (level > 0) {
        paintKochLine(a/3, level-1, theta);
        paintKochLine(a/3, level-1, theta+60);
        paintKochLine(a/3, level-1, theta-60);
        paintKochLine(a/3, level-1, theta);
    } else {
        DrawPolarLine(a, theta);
    }
}

void paintKoch(double a, int n, coord centerPos) {
    coord top = {.x = centerPos.x, .y = centerPos.y+a/sqrt(3)};
    SetPenCoord(top);
    paintKochLine(a, n, -60);
    paintKochLine(a, n, 180);
    paintKochLine(a, n, 60);
}