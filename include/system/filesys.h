#pragma once
#include "lib/types.h"

void OX_InitFileSystem();

void*  OX_ReadFile(const char* path, size_t* sz);
char** OX_ScanDirectory(const char* path, size_t* count);

bool OX_DirectoryExists(const char* path);
bool OX_FileExists(const char* path);