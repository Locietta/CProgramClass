#ifndef COORD_H
#define COORD_H


typedef struct coord{
    double x;
    double y;
} coord;

void SetPenCoord(coord pos);

coord GetPenCoord(void);

int isSamePos(coord pos1, coord pos2);


#endif // (!defined COORD_H)