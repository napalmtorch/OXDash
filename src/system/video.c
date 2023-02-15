#include "system/video.h"
#include "core/common.h"

static OX_Image _framebuffer;
static OX_Image _backbuffer;

void OX_InitVideo()
{
    const int w = 640, h = 480;

    OX_SetVideoMode(w, h, 32, REFRESH_DEFAULT);

    _framebuffer = OX_CreateImageEx(w, h, (uint32_t*)XVideoGetFB(), false);
    _backbuffer  = OX_CreateImage(w, h);

    OX_DebugPrint("Initialized video device manager\n");
}

void OX_SwapVideoBuffers()
{
    if (_framebuffer.w != _backbuffer.w || _framebuffer.h != _backbuffer.h) { OX_DebugError("OX_SwapVideoBuffer() - Size mismatch"); return; }
    OX_MemoryCopy(_framebuffer.data, _backbuffer.data, _framebuffer.w * _framebuffer.h * 4);
}

void OX_SetVideoMode(int w, int h, uint8_t bpp, int refresh)
{
    bool res = XVideoSetMode(w, h, bpp, refresh);
    if (!res) { OX_DebugError("OX_SetVideoMode(%d, %d, %d, %d) - Invalid mode", w, h, bpp, refresh); return; }

    OX_DebugPrint("Set video mode to %dx%dx%dbpp %dHz\n", w, h, bpp, XVideoGetMode().refresh);
}

OX_VideoMode OX_GetVideoMode()
{
    VIDEO_MODE   xmode = XVideoGetMode();
    OX_VideoMode out   = { xmode.width, xmode.height, xmode.bpp, xmode.refresh };
    return out;
}

OX_Image* OX_GetVideoFrameBuffer() { return &_framebuffer; }

OX_Image* OX_GetVideoBackBuffer() { return &_backbuffer; }
