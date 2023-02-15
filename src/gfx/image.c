#include "gfx/image.h"
#include "core/common.h"

OX_Image OX_CreateImage(int w, int h)
{
    if (w == 0 || h == 0) { OX_DebugError("OX_CreateImage(0, 0) - Invalid size"); return OX_NULL_IMAGE; }

    OX_Image img = (OX_Image){ w, h };
    img.data = OX_Allocate(w * h * 4);
    return img;
}

OX_Image OX_CreateImageEx(int w, int h, uint32_t* data, bool copy)
{
    if (w == 0 || h == 0) { OX_DebugError("OX_CreateImageEx(0, 0, %p, %d) - Invalid size", data, copy); return OX_NULL_IMAGE; }
    if (data == NULL) { OX_DebugError("OX_CreateImageEx(%d, %d, 00000000, %d) - Null pointer", w, h, copy); return OX_NULL_IMAGE; }

    OX_Image img = (OX_Image){ w, h };
    if (copy) 
    { 
        img.data = OX_Allocate(w * h * 4); 
        OX_MemoryCopy(img.data, data, w * h * 4);
    } else { img.data = data; }

    return img;
}

void OX_FreeImage(OX_Image* img)
{
    if (img == NULL) { OX_DebugError("OX_FreeImage(00000000) - Null pointer"); return; }
    OX_Free(img->data);
    *img = OX_NULL_IMAGE;
}

void OX_ResizeImage(OX_Image* img, int new_w, int new_h, bool cls)
{
    if (img == NULL) { OX_DebugError("OX_ResizeImage(00000000, %d, %d, %d) - Null pointer", new_w, new_h, cls); return; }
    OX_DebugPrint("OX_ResizeImage not yet implemented!");
}

void OX_CopyImage(OX_Image* dest_img, OX_Image* src_img, int x, int y)
{
    if (dest_img == NULL || src_img == NULL) { OX_DebugError("OX_CopyImage(%p, %p, %d, %d) - Null pointer", dest_img, src_img, x, y); return; }
    OX_ImageDrawPixels(dest_img, x, y, src_img->w, src_img->h, src_img->data);
}

void OX_ClearImage(OX_Image* img, OX_ARGB color)
{
    if (img == NULL) { OX_DebugError("OX_ClearImage(%p, %p) - Null pointer", img, (uint32_t)color); return; }
    OX_MemoryFill(img->data, (int)color, img->w * img->h * 4);
}

void OX_ImageDrawPixel(OX_Image* img, int x, int y, OX_ARGB color)
{
    if (img == NULL || (uint32_t)x >= img->w || (uint32_t)y >= img->h) { return; }
    img->data[y * img->w + x] = color;
}

void OX_ImageDrawPixels(OX_Image* img, int x, int y, int pxls_w, int pxls_h, uint32_t* pixels)
{
    if (img == NULL || pixels == NULL) { OX_DebugError("OX_ImageDrawPixels(%p, %d, %d, %d, %d, %p) - Null pointer)", img, x, y, pxls_w, pxls_h, pixels); return; }
    if (pxls_w == img->w && pxls_h == img->h && x == 0 && y == 0) { OX_MemoryCopy((void*)img->data, pixels, pxls_w * pxls_h * 4); return; }

    uint32_t iw = pxls_w;
    while (x + pxls_w > (int)img->w) { pxls_w--; }
    while (y + pxls_h > (int)img->h) { pxls_h--; }

    for (int yy = 0; yy < pxls_h; yy++)
    {
        uint8_t* src = (uint8_t*)(pixels + (yy * iw));
        int xx = x;
        if (xx < 0) { xx = 0; }
        while (xx + pxls_w > (int)img->w) { xx--; }
        uint32_t ro = (xx + ((y + yy) * img->w)) * 4;
        uint8_t* dest = (uint8_t*)((uint32_t)img->data + ro);
        if (y + yy >= (int)img->h) { return; }
        if (y + yy >= 0 && (uint32_t)dest >= (uint32_t)img->data && (uint32_t)dest < (uint32_t)((uint32_t)img->data + (img->w * img->h * 4)))
        {
            if (x >= 0) { OX_MemoryCopy(dest, src, pxls_w * 4); }
            else { OX_MemoryCopy(dest, src - (x * 4), (pxls_w + x) * 4); }
        }
    }     
}

void OX_ImageDrawRect(OX_Image* img, int x, int y, int w, int h, int thknss, OX_ARGB color)
{
    if (OX_GetARGBChannel(color, OX_ARGB_CH_A) == 0) { return; }
    OX_ImageFillRect(img, x, y, thknss, h, color);
    OX_ImageFillRect(img, x + w - thknss, y, thknss, h, color);
    OX_ImageFillRect(img, x, y, w, thknss, color);
    OX_ImageFillRect(img, x, y + h - thknss, w, thknss, color);
}

void OX_ImageFillRect(OX_Image* img, int x, int y, int w, int h, OX_ARGB color)
{
    if (OX_GetARGBChannel(color, OX_ARGB_CH_A) == 0) { return; }
    for (int i = 0; i < w * h; i++)
    {
        int xx = x + (i % w), yy = y + (i / w);
        if ((uint32_t)yy >= img->h) { return; }
        if ((uint32_t)xx >= img->w) { continue; }
        img->data[yy * img->w + xx] = color;
    }
}

void OX_ImageDrawChar(OX_Image* img, int x, int y, char c, OX_ARGB fg, OX_ARGB bg, OX_Font* font)
{
    if (font == NULL || ((uint32_t)font->type > OX_FONTTYPE_SPR)) { return; }
    if (font->type == OX_FONTTYPE_PSF) { OX_ImageDrawCharPSF(img, x, y, c, fg, bg, (OX_PCScreenFont*)font->obj); return; }
    OX_ImageDrawCharSprite(img, x, y, c, fg, bg, (OX_SpriteFont*)font->obj);
}

void OX_ImageDrawCharPSF(OX_Image* img, int x, int y, char c, OX_ARGB fg, OX_ARGB bg, OX_PCScreenFont* font)
{
    int fh = OX_GetPSFHeight(font, false);
    uint8_t* glyphs = OX_GetPSFGlyphs(font);

    if (OX_GetARGBChannel(bg, OX_ARGB_CH_A) > 0) { OX_ImageFillRect(img, x, y, OX_GetPSFWidth(font, true), OX_GetPSFHeight(font, true), bg); }

    int xx = x;
    for (int j = 0; j < fh; j++)
    {
        int glyph = glyphs[c * fh + j];
        for (int i = 0; i < 8; i++)
        {
            if ((glyph & 0x80) >= 1) { OX_ImageDrawPixel(img, xx, y, fg); }
            glyph <<= 1;
            xx++;
        }
        y++;
        xx = x;
    }
}

void OX_ImageDrawCharSprite(OX_Image* img, int x, int y, char c, OX_ARGB fg, OX_ARGB bg, OX_SpriteFont* font)
{
    
}

void OX_ImageDrawString(OX_Image* img, int x, int y, const char* str, OX_ARGB fg, OX_ARGB bg, OX_Font* font)
{
    if (str == NULL || strlen(str) == 0) { return; }
    if (font == NULL || ((uint32_t)font->type > OX_FONTTYPE_SPR)) { return; }
    if (font->type == OX_FONTTYPE_PSF) { OX_ImageDrawStringPSF(img, x, y, str, fg, bg, (OX_PCScreenFont*)font->obj); return; }
    OX_ImageDrawStringSprite(img, x, y, str, fg, bg, (OX_SpriteFont*)font->obj);
}

void OX_ImageDrawStringPSF(OX_Image* img, int x, int y, const char* str, OX_ARGB fg, OX_ARGB bg, OX_PCScreenFont* font)
{
    if (str == NULL || strlen(str) == 0) { return; }
    int i = 0, xx = x, yy = y, fw = OX_GetPSFWidth(font, true), fh = OX_GetPSFHeight(font, true);
    while (str[i] != 0)
    {
        if (str[i] == '\n') { xx = x; yy += fh; }
        else { OX_ImageDrawCharPSF(img, xx, yy, str[i], fg, bg, font); xx += fw; }
        i++;
    }
}

void OX_ImageDrawStringSprite(OX_Image* img, int x, int y, const char* str, OX_ARGB fg, OX_ARGB bg, OX_SpriteFont* font)
{

}
