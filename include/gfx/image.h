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

/// @brief Image structure for manipulating pixel arrays
typedef struct
{
    /// @brief Width in pixels
    uint16_t  w;
    /// @brief Height in pixels
    uint16_t h;
    /// @brief Raw pixel data
    uint32_t* data;
} OX_Image;

/// @brief Null image structure
#define OX_NULL_IMAGE (OX_Image){ 0, 0, NULL }

/// @brief Create new image @param w Width in pixels @param h Height in pixels @return New image with specified size
OX_Image OX_CreateImage(int w, int h);
/// @brief Create new image @param w Width in pixels @param h Height in pixels @param data Raw pixel data @param copy Copy data or use specified pointer @return New image with specified size and data
OX_Image OX_CreateImageEx(int w, int h, uint32_t* data, bool copy);
/// @brief Dispose image @param img Pointer to image
void OX_FreeImage(OX_Image* img);

/// @brief Resize image @param img Pointer to image @param new_w New width in pixels @param new_h New height in pixels @param cls Clear screen after resizing or keep contents
void OX_ResizeImage(OX_Image* img, int new_w, int new_h, bool cls);
/// @brief Copy image @param dest_img Pointer to destination image @param src_img Pointer to source image @param x Left @param y Top
void OX_CopyImage(OX_Image* dest_img, OX_Image* src_img, int x, int y);
/// @brief Fill entire image with color @param img Pointer to image @param color ARGB color
void OX_ClearImage(OX_Image* img, OX_ARGB color);

/// @brief Draw pixel on image @param img Pointer to image @param x Left @param y Top @param color ARGB color
void OX_ImageDrawPixel(OX_Image* img, int x, int y, OX_ARGB color);
/// @brief Draw pixel array on image @param img Pointer to image @param x Left @param y Top @param pxls_w Source width @param pxls_h Source height @param pixels Raw pixel data
void OX_ImageDrawPixels(OX_Image* img, int x, int y, int pxls_w, int pxls_h, uint32_t* pixels);
/// @brief Draw rectangle outline @param img Pointer to image @param x Left @param y Top @param w Width @param h Height @param thkness Outline thickness @param color ARGB color
void OX_ImageDrawRect(OX_Image* img, int x, int y, int w, int h, int thknss, OX_ARGB color);
/// @brief Draw rectangle outline @param img Pointer to image @param x Left @param y Top @param w Width @param h Height @param color ARGB color
void OX_ImageFillRect(OX_Image* img, int x, int y, int w, int h, OX_ARGB color);
void OX_ImageDrawChar(OX_Image* img, int x, int y, char c, OX_ARGB fg, OX_ARGB bg, OX_Font* font);
void OX_ImageDrawCharPSF(OX_Image* img, int x, int y, char c, OX_ARGB fg, OX_ARGB bg, OX_PCScreenFont* font);
void OX_ImageDrawCharSprite(OX_Image* img, int x, int y, char c, OX_ARGB fg, OX_ARGB bg, OX_SpriteFont* font);
void OX_ImageDrawString(OX_Image* img, int x, int y, const char* str, OX_ARGB fg, OX_ARGB bg, OX_Font* font);
void OX_ImageDrawStringPSF(OX_Image* img, int x, int y, const char* str, OX_ARGB fg, OX_ARGB bg, OX_PCScreenFont* font);
void OX_ImageDrawStringSprite(OX_Image* img, int x, int y, const char* str, OX_ARGB fg, OX_ARGB bg, OX_SpriteFont* font);
