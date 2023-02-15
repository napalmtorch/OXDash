#include "gfx/psf.h"
#include "core/common.h"

OX_PCScreenFont OX_CreatePSF(const uint8_t* data, uint8_t sx, uint8_t sy)
{
    OX_PCScreenFont psf;
    psf.data      = (uint8_t*)data;
    psf.hdr       = (OX_PSFHdr*)data;
    psf.spacing_x = sx;
    psf.spacing_y = sy;
    return psf;
}

uint8_t* OX_GetPSFGlyphs(OX_PCScreenFont* psf)
{
    if (psf == NULL) { OX_DebugError("OX_GetPSFGlyphs(%p) - Null pointer", psf); return NULL; }
    return ((uint8_t*)(uintptr_t)psf->data + sizeof(OX_PSFHdr));
}

OX_Point OX_GetPSFSize(OX_PCScreenFont* psf, bool spacing)
{
    if (psf == NULL) { OX_DebugError("OX_GetPSFSize(%p, %d) - Null pointer", psf, spacing); return OX_CreatePoint(0, 0); }
    return OX_CreatePoint(8 + (spacing ? psf->spacing_x : 0), psf->hdr->charsz + (spacing ? psf->spacing_y : 0));
}

int OX_GetPSFStringWidth(OX_PCScreenFont* psf, const char* str, bool spacing)
{
    if (psf == NULL) { OX_DebugError("OX_GetPSFStringWidth(%p, %p, %d) - Null pointer", psf, str, spacing); return 0; }
    return OX_GetPSFWidth(psf, spacing) * strlen(str);
}

int OX_GetPSFWidth(OX_PCScreenFont* psf, bool spacing)
{
    if (psf == NULL) { OX_DebugError("OX_GetPSFWidth(%p, %d) - Null pointer", psf, spacing); return 0; }
    return 8 + (spacing ? psf->spacing_x : 0);
}

int OX_GetPSFHeight(OX_PCScreenFont* psf, bool spacing)
{
    if (psf == NULL) { OX_DebugError("OX_GetPSFHeight(%p, %d) - Null pointer", psf, spacing); return 0; }
    return psf->hdr->charsz + (spacing ? psf->spacing_y : 0);
}