#include "system/memory.h"
#include "core/common.h"

uint32_t OX_GetMemoryFree() { return OX_GetMemoryTotal() - OX_GetMemoryUsed(); }

uint32_t OX_GetMemoryUsed()
{
    MM_STATISTICS ms = OX_GetMemoryStats();
    return (ms.TotalPhysicalPages - ms.AvailablePages) * 4096;
}

uint32_t OX_GetMemoryTotal()
{
    MM_STATISTICS ms = OX_GetMemoryStats();
    return ms.TotalPhysicalPages * 4096;
}

MM_STATISTICS OX_GetMemoryStats()
{
    MM_STATISTICS ms;
    ms.Length = sizeof(MM_STATISTICS);
    MmQueryStatistics(&ms);
    return ms;
}

void* OX_Allocate(size_t sz)
{
    if (sz == 0) { OX_DebugError("OX_Allocate(0) - Attempt to allocate 0 bytes"); return NULL; }
    void* ptr = malloc(sz);
    OX_MemoryFill(ptr, 0, sz);

    #ifdef DEBUGGING_MEM
        OX_DebugPrint("[MALLOC] Ptr:%p Size:%lu bytes\n", ptr, sz);
    #endif
    return ptr;
}

void OX_Free(void* ptr)
{
    if (ptr == NULL) { OX_DebugError("OX_Free(00000000) - Attempt to free null pointer"); return; }

    #ifdef DEBUGGING_MEM
        OX_DebugPrint("[ FREE ] Ptr:%p\n", ptr);
    #endif
    
    free(ptr);
}

void* OX_MemoryCopy(void* dest, const void* src, size_t sz)
{
    uint32_t  num_dwords = sz / 4;
    uint32_t  num_bytes  = sz % 4;
    uint32_t* dest32     = (uint32_t*)dest;
    uint32_t* src32      = (uint32_t*)src;
    uint8_t*  dest8      = ((uint8_t*)dest) + num_dwords * 4;
    uint8_t*  src8       = ((uint8_t*)src) + num_dwords * 4;
    inline_asm("cld;rep movsl" : "+D"(dest32), "+S"(src32), "+c"(num_dwords) : : "memory");
    inline_asm("cld;rep movsb" : "+D"(dest8),  "+S"(src8),  "+c"(num_bytes)  : : "memory");
    return dest;
}

void* OX_MemoryFill(void* dest, int val, size_t sz)
{
    uint32_t  num_dwords = sz / 4;
    uint32_t  num_bytes  = sz % 4;
    uint32_t* dest32     = (uint32_t*)dest;
    uint8_t*  dest8      = ((uint8_t*)dest) + num_dwords * 4;
    inline_asm("cld;rep stosl" : "+D"(dest32), "+c"(num_dwords) : "a"(val) : "memory");
    inline_asm("cld;rep stosb" : "+D"(dest8),  "+c"(num_bytes)  : "a"(val) : "memory");
    return dest;
}