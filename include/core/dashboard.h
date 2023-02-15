#pragma once
#include "lib/types.h"
#include "lib/math/rect.h"
#include "ui/label.h"

void OX_InitDashboard();
void OX_UpdateDashboard();
void OX_DrawDashboard();

void OX_SetOverscan(int x, int y, int w, int h);
OX_Rect OX_GetOverscan();

void OX_Invalidate();
bool OX_IsInvalidated();

void OX_SetBackBtn(bool state);
bool OX_IsBackBtnDown();

void OX_SetOKBtn(bool state);
bool OX_IsOKBtnDown();