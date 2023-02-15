#include "ui/pages/page_home.h"
#include "core/common.h"

static const char* _menuitems[] = { "Launch DVD", "Games", "Applications", "Settings", "Reboot", "Shutdown" };

void OX_FreePageHome(OX_PageHome* page);
void OX_UpdatePageHome(OX_PageHome* page);
void OX_DrawPageHome(OX_PageHome* page);
void OX_SelectedHomePage(OX_PageHome* page);

OX_PageHome* OX_CreatePageHome()
{
    OX_PageHome* page = OX_CreatePage("home", sizeof(OX_PageHome), OX_SelectedHomePage, OX_FreePageHome, OX_UpdatePageHome, OX_DrawPageHome);

    page->menu = OX_CreateMenuList(0, 0, 320, 160, "menu_main", NULL);
    for (size_t i = 0; i < (sizeof(_menuitems) / sizeof(char*)); i++) { OX_AddMenuItem(page->menu, OX_CreateMenuItem(_menuitems[i], OX_SelectedHomePageMenu, false)); }

    return page;
}

void OX_FreePageHome(OX_PageHome* page)
{
    page->menu->base.e_dispose(page->menu);
    OX_FreePage((OX_Page*)page);
}

void OX_UpdatePageHome(OX_PageHome* page)
{
    OX_UpdatePage((OX_Page*)page);

    page->menu->base.bounds.x = OX_GetOverscan().x + 8;
    page->menu->base.bounds.y = OX_GetOverscan().y + (OX_GetOverscan().h - page->menu->base.bounds.h - 8);
    page->menu->base.active   = (OX_GetSelectedPage() == (OX_Page*)page);
    page->menu->base.e_update(page->menu);
}

void OX_DrawPageHome(OX_PageHome* page)
{
    OX_DrawPage((OX_Page*)page);

    // rendering objects
    OX_Image* screen = OX_GetVideoBackBuffer();
    OX_Font   font   = (OX_Font){ OX_FONTTYPE_PSF, (void*)&OX_DEFAULT_PSF };
    OX_Rect   bnds   = OX_GetOverscan();

    page->menu->base.e_draw(page->menu);
}

void OX_SelectedHomePage(OX_PageHome* page)
{
    OX_SelectedPage((OX_Page*)page);
}

void OX_SelectedHomePageMenu(OX_MenuItem* item)
{
    if (OX_StringEquals(item->text, _menuitems[0]))
    {

    }
    else if (OX_StringEquals(item->text, _menuitems[1])) { OX_SetPage("games"); }
    else if (OX_StringEquals(item->text, _menuitems[2])) { OX_SetPage("apps"); }
    else if (OX_StringEquals(item->text, _menuitems[3])) { OX_SetPage("settings"); }
    else if (OX_StringEquals(item->text, _menuitems[4])) { HalReturnToFirmware(HalRebootRoutine); }
    else if (OX_StringEquals(item->text, _menuitems[4])) { HalInitiateShutdown(); }
}