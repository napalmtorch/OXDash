#pragma once
#include "lib/types.h"
#include "gfx/color.h"
#include "gfx/font.h"
#include "gfx/psf.h"
#include "gfx/sprfont.h"

/// @brief Image formats
typedef enum
{
    OX_IMGTYPE_INVALID,     // Invalid image format
    OX_IMGTYPE_BMP,         // Bitmap
    OX_IMGTYPE_PNG,         // Portable Network Graphics
} OX_IMGTYPE;

typedef struct
{
    uint16_t  w, h;
    uint32_t* data;
} OX_Image;

#define OX_NULL_IMAGE (OX_Image){ 0, 0, NULL };

OX_Image OX_CreateImage(int w, int h);
OX_Image OX_CreateImageEx(int w, int h, uint32_t* data, bool copy);
void OX_FreeImage(OX_Image* img);

void OX_ResizeImage(OX_Image* img, int new_w, int new_h, bool cls);
void OX_CopyImage(OX_Image* dest_img, OX_Image* src_img, int x, int y);
void OX_ClearImage(OX_Image* img, OX_ARGB color);

void OX_ImageDrawPixel(OX_Image* img, int x, int y, OX_ARGB color);
void OX_ImageDrawPixels(OX_Image* img, int x, int y, int pxls_w, int pxls_h, uint32_t* pixels);
void OX_ImageDrawRect(OX_Image* img, int x, int y, int w, int h, int thknss, OX_ARGB color);
void OX_ImageFillRect(OX_Image* img, int x, int y, int w, int h, OX_ARGB color);
void OX_ImageDrawChar(OX_Image* img, int x, int y, char c, OX_ARGB fg, OX_ARGB bg, OX_Font* font);
void OX_ImageDrawCharPSF(OX_Image* img, int x, int y, char c, OX_ARGB fg, OX_ARGB bg, OX_PCScreenFont* font);
void OX_ImageDrawCharSprite(OX_Image* img, int x, int y, char c, OX_ARGB fg, OX_ARGB bg, OX_SpriteFont* font);
void OX_ImageDrawString(OX_Image* img, int x, int y, const char* str, OX_ARGB fg, OX_ARGB bg, OX_Font* font);
void OX_ImageDrawStringPSF(OX_Image* img, int x, int y, const char* str, OX_ARGB fg, OX_ARGB bg, OX_PCScreenFont* font);
void OX_ImageDrawStringSprite(OX_Image* img, int x, int y, const char* str, OX_ARGB fg, OX_ARGB bg, OX_SpriteFont* font);
