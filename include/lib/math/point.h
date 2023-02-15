#pragma once
#include "lib/types.h"

typedef struct
{
    int x, y;
} OX_Point;

static inline OX_Point OX_CreatePoint(int x, int y) { return (OX_Point){ x, y }; }

static inline OX_Point OX_CopyPoint(OX_Point* p) 
{ 
    if (p == NULL) { return (OX_Point){ 0, 0 }; }
    return (OX_Point){ p->x, p->y }; 
}

static inline OX_Point OX_AddPoint(OX_Point p1, OX_Point p2)
{
    return (OX_Point){ p1.x + p2.x, p1.y + p2.y };
}