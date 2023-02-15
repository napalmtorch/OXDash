#pragma once
#include "lib/types.h"
#include "gfx/image.h"

    typedef struct
    {
        uint16_t type;
        uint32_t size;
        uint16_t reserved0;
        uint16_t reserved1;
        uint32_t off_bits;
    } attr_pack OX_BMPFileHdr;

    typedef struct
    {
        uint32_t size;
        uint32_t width;
        uint32_t height;
        uint16_t planes;
        uint16_t bit_count;
        uint32_t compression;
        uint32_t size_image;
        uint32_t x_ppm;
        uint32_t y_ppm;
        uint32_t clr_used;
        uint32_t clr_important;
    } OX_BMPInfoHdr;

OX_Image OX_LoadBitmap(const char* path);
