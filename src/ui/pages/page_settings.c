#include "ui/pages/page_settings.h"
#include "core/common.h"

static const char* _menuitems[] = { "Video", "Audio", "Storage", "Network", "System" };

void OX_FreePageSettings(OX_PageSettings* page);
void OX_UpdatePageSettings(OX_PageSettings* page);
void OX_DrawPageSettings(OX_PageSettings* page);
void OX_SelectedSettingsPage(OX_PageSettings* page);

OX_PageSettings* OX_CreatePageSettings()
{
    OX_PageSettings* page = OX_CreatePage("settings", sizeof(OX_PageSettings), OX_SelectedSettingsPage, OX_FreePageSettings, OX_UpdatePageSettings, OX_DrawPageSettings);

    page->menu = OX_CreateMenuList(0, 0, 320, 160, "menu_settings", NULL);
    page->menu->max_visible = 6;
    for (size_t i = 0; i < (sizeof(_menuitems) / sizeof(char*)); i++) { OX_AddMenuItem(page->menu, OX_CreateMenuItem(_menuitems[i], OX_SelectedSettingsPageMenu, false)); }

    return page;
}

void OX_FreePageSettings(OX_PageSettings* page)
{
    page->menu->base.e_dispose(page->menu);
    OX_FreePage((OX_Page*)page);
}

void OX_UpdatePageSettings(OX_PageSettings* page)
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

void OX_DrawPageSettings(OX_PageSettings* page)
{
    OX_DrawPage((OX_Page*)page);

    // rendering objects
    OX_Image* screen = OX_GetVideoBackBuffer();
    OX_Font   font   = (OX_Font){ OX_FONTTYPE_PSF, (void*)&OX_DEFAULT_PSF };
    OX_Rect   bnds   = OX_GetOverscan();

    page->menu->base.e_draw(page->menu);
}

void OX_SelectedSettingsPage(OX_PageSettings* page)
{
    OX_SelectedPage((OX_Page*)page);
}

void OX_SelectedSettingsPageMenu(OX_MenuItem* item)
{
    
}