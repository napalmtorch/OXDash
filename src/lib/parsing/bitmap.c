#include "lib/parsing/bitmap.h"
#include "core/common.h"


OX_Image OX_LoadBitmap(const char* path)
{
    size_t   fsz  = 0;
    uint8_t* fdat = OX_ReadFile(path, &fsz);

    OX_BMPFileHdr* file_hdr = (OX_BMPFileHdr*)fdat;
    OX_BMPInfoHdr* info_hdr = (OX_BMPInfoHdr*)(fdat + sizeof(OX_BMPFileHdr));
    if (info_hdr->width == 0 || info_hdr->height == 0) { OX_DebugError("OX_LoadBitmap(%s) - Invalid size", path); return OX_NULL_IMAGE; }
    if (info_hdr->bit_count < 24) { OX_DebugError("OX_LoadBitmap(%s) - Unsupported bpp %d", path, info_hdr->bit_count); return OX_NULL_IMAGE; }

    OX_Image img      = OX_CreateImage(info_hdr->width, info_hdr->height);
    uint8_t* img_data = (uint8_t*)((uint32_t)fdat + file_hdr->off_bits);
    uint32_t w        = info_hdr->width, h = info_hdr->height;
    size_t   sz       = w * h * 4;
    
    for (int yy = h - 1; yy >= 0; yy--)
    {
        for (int xx = 0; xx < w; xx++)
        {
            uint32_t o = ((info_hdr->bit_count == 32 ? 4 : 3)) * (yy * w + xx);
            uint32_t c = (0xFF << 24) | (img_data[o + 2] << 16) | (img_data[o + 1] << 8) | img_data[o];
            uint32_t dest = xx + ((h- yy - 1) * w);
            img.data[dest] = c;
        }
    }
    
    OX_Free(fdat);
    return img;

}