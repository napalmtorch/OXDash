#include "lib/parsing/lodepng.h"
#include "core/common.h"

uint32_t* OX_DecodePNG(const char* path, uint32_t* w, uint32_t* h) 
{
    uint32_t* img_data = NULL;
    uint32_t  error    = lodepng_decode32_file((char*)&img_data, w, h, path);
    if (error) { OX_DebugError("OX_DecodePNG(%s, %p, %p) - Unexpected failure", path, w, h); return NULL; }

    // convert color order
    for (size_t i = 0; i < *w * *h; i++)
    {
        uint8_t  a = (img_data[i] & 0xFF), b = ((img_data[i] & 0xFF00) >> 8), g = ((img_data[i] & 0xFF0000) >> 16), r = ((img_data[i] & 0xFF000000) >> 24);
        img_data[i] = OX_CreateARGB(a, r, g, b);
    }
    return img_data;  
}

OX_Image OX_LoadPNG(const char* path)
{
    uint32_t  w   = 0, h = 0;
    uint32_t* dat = OX_DecodePNG(path, &w, &h);
    if (dat == NULL) { return OX_NULL_IMAGE; }

    OX_Image img = OX_CreateImageEx(w, h, dat, true);
    OX_Free(dat);

    return img;
}