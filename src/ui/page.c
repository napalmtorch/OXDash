#include "ui/page.h"
#include "core/common.h"

#define OX_PAGE_COUNT 3

static OX_Page* _pages[OX_PAGE_COUNT];
static int      _selpage;
static OX_Page* _lastpage;

void OX_InitPages()
{
    memset(_pages, 0, sizeof(_pages));
    _selpage  = -1;
    _lastpage = NULL;
    _pages[0] = OX_CreatePageHome();
    _pages[1] = OX_CreatePageGames();
    _pages[2] = OX_CreatePageSettings();

    OX_SetPage("home");
    OX_DebugPrint("Initialized page manager\n");
}

void OX_FreePages()
{
    for (size_t i = 0; i < OX_PAGE_COUNT; i++) { _pages[i]->e_dispose(_pages[i]); }
    OX_DebugPrint("Disposed page manager");
}

void OX_UpdatePages()
{
    if ((uint32_t)_selpage >= OX_PAGE_COUNT) { return; }
    _pages[_selpage]->e_update(_pages[_selpage]);
}

void OX_DrawPages()
{
    if ((uint32_t)_selpage >= OX_PAGE_COUNT) { return; }
    _pages[_selpage]->e_draw(_pages[_selpage]);
}

void OX_GotoLastPage()
{
    if (_lastpage == NULL) { OX_SetPage("home"); }
    else { OX_SetPage(_lastpage->name); }
}

void OX_SetPage(const char* name)
{
    if (name == NULL || strlen(name) == 0) { OX_DebugError("OX_SetPage(%s) - Invalid name", name); return; }
    for (size_t i = 0; i < OX_PAGE_COUNT; i++)
    {
        if (OX_StringEquals(_pages[i]->name, name)) 
        { 
            if (_selpage != i) 
            { 
                if ((uint32_t)_selpage < OX_PAGE_COUNT) { _lastpage = _pages[_selpage]; }
                _selpage = i; 
                _pages[i]->e_selected(_pages[i]); 
                OX_DebugPrint("Set page to '%s'\n", _pages[i]->name);
            }
            return;
        }
    }
    OX_DebugError("OX_SetPage(%s) - No page with specified name", name);
}

OX_Page* OX_GetPage(const char* name)
{
    if (name == NULL || strlen(name) == 0) { return false; }
    for (size_t i = 0; i < OX_PAGE_COUNT; i++)
    {
        if (OX_StringEquals(_pages[i]->name, name)) { return _pages[i]; }
    }
    return NULL;
}

OX_Page* OX_GetSelectedPage()
{
    if ((uint32_t)_selpage >= OX_PAGE_COUNT) { return NULL; }
    return _pages[_selpage];
}

// --------------------------------------------------------------------------------------------------------

OX_Page* OX_CreatePage(const char* name, size_t sz, OX_PageEvent selected, OX_PageEvent dispose, OX_PageEvent update, OX_PageEvent draw)
{
    if (name == NULL || strlen(name) == 0) { OX_DebugError("OX_CreatePage(%p, %p, %p, %p, %p) - Invalid name", name, selected, dispose, update, draw); return NULL; }
    if (sz < sizeof(OX_Page)) { sz = sizeof(OX_Page); }

    OX_Page* page = (OX_Page*)OX_Allocate(sz);
    page->name = OX_CreateString(name);
    page->e_selected = (selected == NULL ? OX_SelectedPage : selected);
    page->e_dispose  = (dispose  == NULL ? OX_FreePage     : dispose);
    page->e_update   = (update   == NULL ? OX_UpdatePage   : update);
    page->e_draw     = (draw     == NULL ? OX_DrawPage     : draw);
    return page;
}

void OX_FreePage(OX_Page* page)
{
    if (page == NULL) { OX_DebugError("OX_FreePage(00000000) - Null pointer"); return; }
    if (page->name != NULL) { OX_FreeString(page->name); page->name = NULL; }
    OX_Free(page);
}

void OX_UpdatePage(OX_Page* page)
{
    if (page == NULL) { OX_DebugError("OX_UpdatePage(00000000) - Null pointer"); return; }
}

void OX_DrawPage(OX_Page* page)
{
    if (page == NULL) { OX_DebugError("OX_DrawPage(00000000) - Null pointer"); return; }
}

void OX_SelectedPage(OX_Page* page)
{
    if (page == NULL) { OX_DebugError("OX_SelectedPageChanged(00000000) - Null pointer"); return; }
}
