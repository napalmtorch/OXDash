#pragma once
#include "lib/types.h"
#include "lib/math/point.h"

typedef struct
{
    int x, y, w, h;
} OX_Rect;

static inline OX_Rect OX_CreateRect(int x, int y, int w, int h)
{
    return (OX_Rect){ x, y, w, h };
}

static inline OX_Rect OX_CreateRectP(OX_Point pos, OX_Point sz)
{
    return (OX_Rect){ pos.x, pos.y, sz.x, sz.y };
}

static inline bool OX_RectEqualsEx(OX_Rect r, int x, int y, int w, int h)
{
    return (r.x == x && r.y == y && r.w == w && r.h == h);
}

static inline bool OX_RectEquals(OX_Rect r1, OX_Rect r2)
{
    return (r1.x == r2.x && r1.y == r2.y && r1.w == r2.w && r1.h == r2.h);
}

static inline bool OX_RectContains(OX_Rect r, int x, int y)
{
    return (x >= r.x && x < r.x + r.w && y >= r.y && y < r.y + r.h);
}

static inline bool OX_RectContainsP(OX_Rect r, OX_Point pos)
{
    return (pos.x >= r.x && pos.x < r.x + r.w && pos.y >= r.y && pos.y < r.y + r.h);
}

static inline bool OX_RectIntersects(OX_Rect r1, OX_Rect r2) { return OX_RectContains(r1, r2.x, r2.y); }