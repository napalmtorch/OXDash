#pragma once
#include "lib/types.h"

/// @brief 32-bit packed ARGB color
typedef unsigned int OX_ARGB;

static const OX_ARGB OX_ARGB_TRANSPARENT        = 0x00FFFFFF;
static const OX_ARGB OX_ARGB_BLACK              = 0xFF000000;
static const OX_ARGB OX_ARGB_WHITE              = 0xFFFFFFFF;
static const OX_ARGB OX_ARGB_RED                = 0xFFFF0000;
static const OX_ARGB OX_ARGB_GREEN              = 0xFF00FF00;
static const OX_ARGB OX_ARGB_BLUE               = 0xFF0000FF;
static const OX_ARGB OX_ARGB_YELLOW             = 0xFFFFFF00;
static const OX_ARGB OX_ARGB_CYAN               = 0xFF00FFFF;
static const OX_ARGB OX_ARGB_MAGENTA            = 0xFFFF00FF;

/// @brief Alpha channel of ARGB value
#define OX_ARGB_CH_A 24
/// @brief Red channel of ARGB value
#define OX_ARGB_CH_R 16
/// @brief Green channel of ARGB value
#define OX_ARGB_CH_G 8
/// @brief Blue channel of ARGB value
#define OX_ARGB_CH_B 0

/// @brief Create packed ARGB color from individual attributes @param a Alpha @param r Red @param g Green @param b Blue @return Packed ARGB color value
static inline OX_ARGB OX_CreateARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b) 
{ 
    return (OX_ARGB)((a << 24) | (r << 16) | (g << 8) | b); 
}

/// @brief Extract value of color channel from packed ARGB color value @param color Packed color @param ch Color channel @return Color channel value (0-255)
static inline uint8_t OX_GetARGBChannel(OX_ARGB color, uint8_t ch) 
{ 
    return (uint8_t)((uint32_t)color >> ch);
}