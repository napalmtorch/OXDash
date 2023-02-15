#pragma once
#include "lib/types.h"
#include "lib/math/point.h"

/// @brief Font rendering type
typedef enum
{
    OX_FONTTYPE_PSF,    // PC screen font
    OX_FONTTYPE_SPR,    // Sprite font
} OX_FONTTYPE;

/// @brief Structure for managing abstraction of font rendering
typedef struct
{
    /// @brief Font rendering type
    OX_FONTTYPE type;
    /// @brief Pointer to font object
    void*       obj;
} OX_Font;

/// Get size of font @param font Font @param spacing Including spacing in return value @return Font size in pixels
OX_Point OX_GetFontSize(OX_Font* font, bool spacing);

/// Measure the width of a string @param font Font @param str Pointer to string @param spacing Including spacing in return value @return String width in pixels
int OX_GetFontStringWidth(OX_Font* font, const char* str, bool spacing);

/// Get width of font @param font Font @param spacing Including spacing in return value @return Font width in pixels
int OX_GetFontWidth(OX_Font* font, bool spacing);

/// Get height of font @param font Font @param spacing Including spacing in return value @return Font height in pixels
int OX_GetFontHeight(OX_Font* font, bool spacing);