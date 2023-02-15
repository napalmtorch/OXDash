#pragma once
#include "lib/types.h"
#include "lib/math/point.h"

typedef enum
{
    OX_FONTTYPE_PSF,
    OX_FONTTYPE_SPR,
} OX_FONTTYPE;

typedef struct
{
    OX_FONTTYPE type;
    void*       obj;
} OX_Font;

OX_Point OX_GetFontSize(OX_Font* font, bool spacing);
int OX_GetFontStringWidth(OX_Font* font, const char* str, bool spacing);
int OX_GetFontWidth(OX_Font* font, bool spacing);
int OX_GetFontHeight(OX_Font* font, bool spacing);