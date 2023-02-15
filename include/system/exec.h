#pragma once
#include "lib/types.h"
#include "gfx/image.h"

typedef struct
{
    char*    path;
    char*    name;
    char*    img_path;
} OX_Executable;

void OX_ScanGames(const char* path);
void OX_Launch(const char* path);
void OX_RegisterGame(OX_Executable* exec);
void OX_RegisterApp(OX_Executable* exec);
void OX_ClearRegisteredGames();
void OX_ClearRegisteredApps();
OX_Executable* OX_GetGames(size_t* count);
OX_Executable* OX_GetApps(size_t* count);

OX_Executable OX_CreateExecutable(const char* path, const char* name, const char* img_path);
void OX_FreeExecutable(OX_Executable* exe);