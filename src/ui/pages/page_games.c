#include "ui/pages/page_games.h"
#include "core/common.h"

void OX_FreePageGames(OX_PageGames* page);
void OX_UpdatePageGames(OX_PageGames* page);
void OX_DrawPageGames(OX_PageGames* page);
void OX_SelectedGamesPage(OX_PageGames* page);

OX_PageGames* OX_CreatePageGames()
{
    OX_PageGames* page = OX_CreatePage("games", sizeof(OX_PageGames), OX_SelectedGamesPage, OX_FreePageGames, OX_UpdatePageGames, OX_DrawPageGames);
    page->menu = OX_CreateMenuList(0, 0, 320, 160, "menu_games", NULL);

    return page;
}

void OX_FreePageGames(OX_PageGames* page)
{
    page->menu->base.e_dispose(page->menu);
    OX_FreePage((OX_Page*)page);
}

void OX_UpdatePageGames(OX_PageGames* page)
{
    OX_UpdatePage((OX_Page*)page);

    page->menu->base.bounds.x = OX_GetOverscan().x + 8;
    page->menu->base.bounds.y = OX_GetOverscan().y + (OX_GetOverscan().h - page->menu->base.bounds.h - 8);
    page->menu->base.active   = (OX_GetSelectedPage() == (OX_Page*)page);
    page->menu->base.e_update(page->menu);

    if (OX_GetGamepadButton(OX_GAMEPAD_1, OX_GAMEPAD_BTN_B) && !OX_IsBackBtnDown())
    {
        OX_SetBackBtn(true);
        OX_SetPage("home");
    }
}

void OX_DrawPageGames(OX_PageGames* page)
{
    OX_DrawPage((OX_Page*)page);

    // rendering objects
    OX_Image* screen = OX_GetVideoBackBuffer();
    OX_Font   font   = (OX_Font){ OX_FONTTYPE_PSF, (void*)&OX_DEFAULT_PSF };
    OX_Rect   bnds   = OX_GetOverscan();

    page->menu->base.e_draw(page->menu);
}

void OX_RefreshPageGames(OX_PageGames* page)
{
    OX_ClearMenuItems(page->menu);

    size_t         count = 0;
    OX_Executable* games = OX_GetGames(&count);

    for (size_t i = 0; i < count; i++)
    {
        OX_AddMenuItem(page->menu, OX_CreateMenuItem(games[i].name, OX_SelectedGamesPageMenu, false));
    }
}

void OX_SelectedGamesPage(OX_PageGames* page)
{
    OX_SelectedPage((OX_Page*)page);
}

void OX_SelectedGamesPageMenu(OX_MenuItem* item)
{
    if ((uint32_t)item->parent->selected >= item->parent->item_count) { return; }

    size_t         count = 0;
    OX_Executable* games = OX_GetGames(&count);

    char* path = OX_CreateStringEx(games[item->parent->selected].path, strlen(games[item->parent->selected].path) + 24);
    strcat(path, "default.xbe");

    if (!OX_FileExists(path)) { OX_DebugError("OX_SelectedGamesPageMenu(%p) - Unable to locate game '%s'", item, games[item->parent->selected].name); return; }
    XLaunchXBE(path);
}