#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define extern_c extern "C"

#define inline_asm __asm__ volatile

#define attr_pack        __attribute__((packed))
#define attr_align(x)    __attribute__((aligned(x)))
#define attr_optimize(x) __attribute__((optimize(x)))

#define KILOBYTE (1024L)
#define MEGABYTE (KILOBYTE * 1024L)
#define GIGABYTE (MEGABYTE * 1024L)