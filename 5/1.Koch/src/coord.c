#include "coord.h"
#include "graphics.h"
#include "extgraph.h"

void SetPenCoord(coord pos) {
    MovePen(pos.x, pos.y);
}

coord GetPenCoord(void) {
    return (coord) {.x=GetCurrentX(), .y=GetCurrentY()};
}

int isSamePos(coord pos1, coord pos2) {
    return (pos1.x == pos2.x && pos1.y == pos2.y);
}