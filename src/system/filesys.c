#include "system/filesys.h"
#include "core/common.h"

#define APP_ROOT 'A'

static WIN32_FIND_DATA _find_data;

void OX_InitFileSystem()
{
    char targetPath[MAX_PATH];
    nxGetCurrentXbeNtPath(targetPath);
    char *finalSeparator = strrchr(targetPath, '\\');
    *(finalSeparator + 1) = '\0';
    nxMountDrive(APP_ROOT, targetPath);
    OX_DebugPrint("Mounted current directory to %c:\n", APP_ROOT);

    nxMountDrive('D', "\\Device\\CdRom0");
    nxMountDrive('C', "\\Device\\Harddisk0\\Partition2\\");
    nxMountDrive('E', "\\Device\\Harddisk0\\Partition1\\");
    nxMountDrive('F', "\\Device\\Harddisk0\\Partition6\\");
    nxMountDrive('G', "\\Device\\Harddisk0\\Partition7\\");
    nxMountDrive('X', "\\Device\\Harddisk0\\Partition3\\");
    nxMountDrive('Y', "\\Device\\Harddisk0\\Partition4\\");
    nxMountDrive('Z', "\\Device\\Harddisk0\\Partition5\\");
    OX_DebugPrint("Initialized filesystem\n");
}

void* OX_ReadFile(const char* path, size_t* sz)
{
    FILE* fptr = fopen(path, "rb");
    if (fptr == NULL) { OX_DebugError("OX_ReadFile(%s, %p) - Unable to locate file", path, sz); return NULL; }

    fseek(fptr, 0, SEEK_END);
    *sz = ftell(fptr);
    rewind(fptr);

    void* data = OX_Allocate(*sz);
    fread(data, *sz, 1, fptr);
    fclose(fptr);

    return data;
}

size_t OX_ScanDirectoryCount(char* path)
{
    path[strlen(path) - 1] = 0;
    if (!OX_DirectoryExists(path)) { return 0; }
    path[strlen(path)] = '*';

    HANDLE hFind;
    memset(&_find_data, 0, sizeof(_find_data));
    size_t count = 0;

    hFind = FindFirstFile(path, &_find_data);
    if (strlen(_find_data.cFileName) > 0) { count++; }
    while (FindNextFile(hFind, &_find_data)) { if (strlen(_find_data.cFileName) > 0) { count++; } }
    FindClose(hFind);
    return count;
}

char** OX_ScanDirectory(const char* path, size_t* count)
{
    if (!OX_DirectoryExists(path)) { *count = 0; return NULL; }
    char* p = OX_CreateStringEx(path, strlen(path) + 2);
    char end = '*';
    strncat(p, &end, 1);

    size_t c = OX_ScanDirectoryCount(p);
    if (c == 0) { return NULL; }
    *count = c;

    char** list = (char**)OX_Allocate(sizeof(char*) * c);

    HANDLE hFind;
    memset(&_find_data, 0, sizeof(_find_data));
    size_t i = 0;

    hFind = FindFirstFile(p, &_find_data);
    if (strlen(_find_data.cFileName) > 0) { list[i++] = OX_CreateString(_find_data.cFileName); }
    while (FindNextFile(hFind, &_find_data)) { if (strlen(_find_data.cFileName) > 0) { list[i++] = OX_CreateString(_find_data.cFileName); } }
    FindClose(hFind);
    OX_FreeString(p);
    return list;
}

bool OX_DirectoryExists(const char* path)
{
    DWORD dwAttrib = GetFileAttributes(path);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool OX_FileExists(const char* path)
{
    DWORD dwAttrib = GetFileAttributes(path);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}