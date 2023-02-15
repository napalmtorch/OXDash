#include "core/debug.h"
#include "core/common.h"

static char _buff[128];

void OX_DebugPrint(const char* str, ...)
{
    va_list args;
    va_start(args, str);
    memset(_buff, 0, sizeof(_buff));
    vsprintf(_buff, str, args);
    va_end(args);
    debugPrint("%s", _buff);
}

void OX_DebugError(const char* str, ...)
{
    va_list args;
    va_start(args, str);
    memset(_buff, 0, sizeof(_buff));
    vsprintf(_buff, str, args);
    va_end(args);
    debugPrint("%s\n", _buff);
    debugPrint("Exiting...\n");
    Sleep(5000);
    exit(0);
}