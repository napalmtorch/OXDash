#include "gfx/font.h"
#include "core/common.h"

OX_Point OX_GetFontSize(OX_Font* font, bool spacing)
{
    if (font == NULL) { OX_DebugError("OX_GetFontSize(00000000, %d) - Null pointer", spacing); return OX_CreatePoint(0, 0); }
    OX_Point sz = OX_CreatePoint(0, 0);

    if (font->type == OX_FONTTYPE_PSF)
    {
        OX_PCScreenFont* psf = (OX_PCScreenFont*)font->obj;
        sz.x = (OX_PSF_WIDTH + psf->spacing_x);
        sz.y = (psf->hdr->charsz + psf->spacing_y);
    }
    else if (font->type == OX_FONTTYPE_SPR) { }

    return sz;
}

int OX_GetFontStringWidth(OX_Font* font, const char* str, bool spacing)
{
    if (font == NULL) { OX_DebugError("OX_GetFontStringWidth(00000000, %p, %d) - Null pointer", str, spacing); return 0; }
    return (str == NULL ? 0 : (OX_GetFontWidth(font, spacing) * strlen(str)));
}

int OX_GetFontWidth(OX_Font* font, bool spacing)
{
    if (font == NULL) { OX_DebugError("OX_GetFontWidth(00000000, %d) - Null pointer", spacing); return 0; }
    if (font->type == OX_FONTTYPE_PSF)
    {
        OX_PCScreenFont* psf = (OX_PCScreenFont*)font->obj;
        return (OX_PSF_WIDTH + psf->spacing_x);
    }
    else if (font->type == OX_FONTTYPE_SPR) { }

    return 0;
}

int OX_GetFontHeight(OX_Font* font, bool spacing)
{
    if (font == NULL) { OX_DebugError("OX_GetFontHeight(00000000, %d) - Null pointer", spacing); return 0; }
    if (font->type == OX_FONTTYPE_PSF)
    {
        OX_PCScreenFont* psf = (OX_PCScreenFont*)font->obj;
        return (psf->hdr->charsz + psf->spacing_y);
    }
    else if (font->type == OX_FONTTYPE_SPR) { }

    return 0;
}