#pragma once
#include "lib/types.h"
#include <xboxkrnl/xboxkrnl.h>
#include <windows.h>

uint32_t      OX_GetMemoryFree();
uint32_t      OX_GetMemoryUsed();
uint32_t      OX_GetMemoryTotal();
MM_STATISTICS OX_GetMemoryStats();

void* OX_Allocate(size_t sz);
void  OX_Free(void* ptr);

void* OX_MemoryCopy(void* dest, const void* src, size_t sz);
void* OX_MemoryFill(void* dest, int val, size_t sz);