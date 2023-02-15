#include "system/exec.h"
#include "core/common.h"

#define OX_NULL_EXEC (OX_Executable){ NULL, NULL, NULL }

static OX_Executable* _games = NULL;
static OX_Executable* _apps  = NULL;
static size_t         _game_count = 0, _app_count = 0;

void OX_ScanGames(const char* path)
{
    OX_ClearRegisteredApps();
    OX_ClearRegisteredGames();

    size_t count = 0;
    char** dirs  = OX_ScanDirectory(path, &count);
    char*  p     = OX_Allocate(strlen(path) + MAX_PATH + 8);

    for (size_t i = 0; i < count; i++)
    {
        memset(p, 0, strlen(path) + MAX_PATH + 8);
        strcat(p, path);
        strcat(p, dirs[i]);
        strcat(p, "\\");
        char* game_root = (char*)p;
        if (!OX_DirectoryExists(p)) { continue; }

        OX_Executable exec = OX_CreateExecutable(OX_CreateString(p), OX_CreateString(dirs[i]), NULL);

        strcat(p, "default.png");
        if (OX_FileExists(p)) { exec.img_path = OX_CreateString(p); }
        OX_RegisterGame(&exec);
        OX_FreeString(dirs[i]);
    }
    if (dirs != NULL) { OX_Free(dirs); }

    OX_DebugPrint("Finished scanning games - %d detected\n", _game_count);
}

void OX_Launch(const char* path)
{
    
}

void OX_ClearRegisteredGames()
{
    for (size_t i = 0; i < _game_count; i++) { OX_FreeExecutable(&_games[i]); }
    if (_games != NULL) { OX_Free(_games); }

    _games      = NULL;
    _game_count = 0;
}

void OX_ClearRegisteredApps()
{
    for (size_t i = 0; i < _app_count; i++) { OX_FreeExecutable(&_apps[i]); }
    if (_apps != NULL) { OX_Free(_apps); }

    _apps      = NULL;
    _app_count = 0;
}

void OX_RegisterGame(OX_Executable* exec)
{
    if (exec == NULL) { OX_DebugError("OX_RegisterGame(00000000) - Null pointer"); return; }

    OX_Executable* games = (OX_Executable*)OX_Allocate((_game_count + 1) * sizeof(OX_Executable));
    if (_games != NULL) { OX_MemoryCopy(games, _games, _game_count * sizeof(OX_Executable)); OX_Free(_games); }

    _games = games;
    _games[_game_count] = *exec;
    _game_count++;
    OX_DebugPrint("Located game - Img:%s Path:'%s'\n", (exec->img_path != NULL ? "YES" : "NO"), exec->path);
}

void OX_RegisterApp(OX_Executable* exec)
{

}

OX_Executable* OX_GetGames(size_t* count)
{
    *count = _game_count;
    return _games;
}

OX_Executable* OX_GetApps(size_t* count)
{
    *count = _app_count;
    return _apps;
}

OX_Executable OX_CreateExecutable(const char* path, const char* name, const char* img_path)
{
    if (path == NULL) { OX_DebugError("OX_CreateExecutable(%p, %p) - Null pointer", path, img_path); return OX_NULL_EXEC; }
    if (strlen(path) == 0) { OX_DebugError("OX_CreateExecutable(%p, %p) - Invalid path", path, img_path); return OX_NULL_EXEC; }
    OX_Executable exec = (OX_Executable){ path, name, img_path };
    return exec;
}

void OX_FreeExecutable(OX_Executable* exe)
{
    if (exe == NULL) { OX_DebugError("OX_FreeExecutable(00000000) - Null pointer"); return; }
    if (exe->path     != NULL) { OX_FreeString(exe->path); }
    if (exe->name     != NULL) { OX_FreeString(exe->name); }
    if (exe->img_path != NULL) { OX_FreeImage(&exe->img_path); }
}