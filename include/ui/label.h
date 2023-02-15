#pragma once
#include "ui/widget.h"

typedef struct OX_Label OX_Label;

struct OX_Label
{
    OX_Widget base;
    bool      autosize;
};

OX_Label* OX_CreateLabel(int x, int y, bool autosz, const char* text, const char* name, OX_Container* parent);
OX_Label* OX_CreateLabelEx(int x, int y, int w, int h, bool autosz, const char* text, const char* name, OX_Container* parent);
void OX_FreeLabel(OX_Label* label);

void OX_UpdateLabel(OX_Label* label);
void OX_DrawLabel(OX_Label* label);