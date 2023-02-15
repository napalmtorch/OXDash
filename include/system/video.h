#pragma once
#include "lib/types.h"
#include "gfx/image.h"

typedef struct
{
    uint16_t w, h;
    uint8_t  bpp, refresh;
} OX_VideoMode;

void         OX_InitVideo();
void         OX_SetVideoMode(int w, int h, uint8_t bpp, int refresh);
void         OX_SwapVideoBuffers();
OX_VideoMode OX_GetVideoMode();
OX_Image*    OX_GetVideoFrameBuffer();
OX_Image*    OX_GetVideoBackBuffer();