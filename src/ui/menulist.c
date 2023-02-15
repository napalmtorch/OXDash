#include "ui/menulist.h"
#include "core/common.h"

#define OX_NULL_MENUITEM (OX_MenuItem){ NULL, NULL, NULL }

OX_MenuItem OX_CreateMenuItem(const char* text, OX_MenuItemEvent sel, bool copy)
{
    if (text == NULL) { return OX_NULL_MENUITEM; }
    OX_MenuItem item = { NULL, NULL, sel };
    if (copy) { item.text = OX_CreateString(text); }
    else { item.text = text; }
    return item;
}

OX_MenuList* OX_CreateMenuList(int x, int y, int w, int h, const char* name, OX_Container* parent)
{
    OX_MenuList* menu = (OX_MenuList*)OX_CreateWidget(x, y, w, h, "", name, OX_WIDGET_LIST, parent, sizeof(OX_MenuList));
    menu->item_count  = 0;
    menu->items       = NULL;
    menu->autosize_x  = false;
    menu->autosize_y  = false;
    menu->selected    = -1;
    menu->max_visible = 10;
    menu->btn_down    = false;
    menu->btn_up      = false;
    menu->btn_sel     = false;

    menu->base.e_update  = (OX_WidgetEvent)OX_UpdateMenuList;
    menu->base.e_draw    = (OX_WidgetEvent)OX_DrawMenuList;
    menu->base.e_dispose = (OX_WidgetEvent)OX_FreeMenuList;
    return menu;
}

void OX_FreeMenuList(OX_MenuList* menu)
{
    OX_FreeWidget((OX_Widget*)menu);
}

void OX_UpdateMenuList(OX_MenuList* menu)
{
    OX_UpdateWidget((OX_Widget*)menu);

    if (menu->autosize_y) 
    { 
        int fh = OX_GetFontHeight(&menu->base.style.font, true);
        if (menu->item_count == 0) { menu->base.bounds.h = fh + (menu->base.style.bdr_sz * 2); }
        else { menu->base.bounds.h = (menu->item_count * fh) + (menu->base.style.bdr_sz * 2); }
    }
    if (menu->autosize_x)
    {
        size_t i, len, longest = 0;
        for (i = 0; i < menu->item_count; i++)
        {
            len = strlen(menu->items[i].text);
            if (len > longest) { longest = len; }
        }
        menu->base.bounds.w = (longest * OX_GetFontWidth(&menu->base.style.font, true)) + (menu->base.style.bdr_sz * 2);
        if (menu->base.bounds.w < 16 + (menu->base.style.bdr_sz * 2)) { menu->base.bounds.w = 16 + (menu->base.style.bdr_sz * 2); }
    }

    if (menu->base.active && menu->item_count > 0 && menu->items != NULL)
    {
        if (OX_GetGamepadButton(OX_GAMEPAD_1, OX_GAMEPAD_BTN_UP)   && !menu->btn_up)   { OX_MenuListPrevious(menu); menu->btn_up   = true; }
        if (OX_GetGamepadButton(OX_GAMEPAD_1, OX_GAMEPAD_BTN_DOWN) && !menu->btn_down) { OX_MenuListNext(menu);     menu->btn_down = true; }
        if (!OX_GetGamepadButton(OX_GAMEPAD_1, OX_GAMEPAD_BTN_UP))                     { menu->btn_up   = false; }
        if (!OX_GetGamepadButton(OX_GAMEPAD_1, OX_GAMEPAD_BTN_DOWN))                   { menu->btn_down = false; }

        if (OX_GetGamepadButton(OX_GAMEPAD_1, OX_GAMEPAD_BTN_A) && !OX_IsOKBtnDown())
        {
            if (menu->items[menu->selected].e_selected != NULL) { menu->items[menu->selected].e_selected(&menu->items[menu->selected]); }
            OX_SetOKBtn(true);
        }
    }
}

void OX_DrawMenuList(OX_MenuList* menu)
{
    OX_DrawWidget((OX_Widget*)menu);

    OX_Image* target = OX_GetVideoBackBuffer();
    if (menu->base.parent != NULL) { target = &menu->base.parent->backbuffer; }

    int ih = OX_GetFontHeight(&menu->base.style.font, true);
    int xx = menu->base.bounds.x + menu->base.style.bdr_sz, yy = menu->base.bounds.y + menu->base.style.bdr_sz;
    for (size_t i = menu->scroll; i < menu->scroll + menu->max_visible; i++)
    {
        if (i >= menu->item_count) { break; }
        OX_ARGB bg = OX_ARGB_TRANSPARENT, fg = menu->base.style.fg;
        if (i == menu->selected) { bg = menu->base.style.bg_hover; fg = menu->base.style.fg_hover; }

        if (bg != OX_ARGB_TRANSPARENT) { OX_ImageFillRect(target, xx, yy, menu->base.bounds.w - (menu->base.style.bdr_sz * 2), OX_GetFontHeight(&menu->base.style.font, true), bg); }
        OX_ImageDrawString(target, xx, yy, menu->items[i].text, fg, OX_ARGB_TRANSPARENT, &menu->base.style.font);
        yy += ih;
    }
}

void OX_ClearMenuItems(OX_MenuList* menu)
{
    if (menu == NULL) { OX_DebugError("OX_ClearMenuItems(00000000) - Null pointer"); return; }
    if (menu->items != NULL)
    {
        for (size_t i = 0; i < menu->item_count; i++)
        {
            if (menu->items[i].text != NULL)
            {
                if (menu->items[i].text != NULL) { OX_FreeString(menu->items[i].text); }
            }
        }
        OX_Free(menu->items);
    }
    menu->item_count = 0;
}

void OX_AddMenuItem(OX_MenuList* menu, OX_MenuItem item)
{
    if (menu == NULL) { OX_DebugError("OX_AddMenuItem(%p, %p) - Null pointer", menu, item); return; }
    
    OX_MenuItem* items = OX_Allocate((menu->item_count + 1) * sizeof(OX_MenuItem));
    if (menu->items != NULL) { OX_MemoryCopy(items, menu->items, menu->item_count * sizeof(OX_MenuItem)); OX_Free(menu->items); }

    item.parent = menu;

    menu->items = items;
    menu->items[menu->item_count] = item;
    menu->item_count++;

    if ((uint32_t)menu->selected >= menu->item_count) { menu->selected = menu->item_count - 1; }
}

void OX_RemoveMenuItem(OX_MenuList* menu, OX_MenuItem* item)
{
    if (menu == NULL || item  == NULL) { OX_DebugError("OX_RemoveMenuItem(%p, %p) - Null pointer", menu, item); return; }
    
    for (size_t i = 0; i < menu->item_count; i++)
    {
        if (&menu->items[i] == item) { OX_RemoveMenuItemAt(menu, i); return; }
    }
    OX_DebugError("OX_RemoveMenuItem(%p, %p) - Unexpected failure", menu, item);
}

void OX_RemoveMenuItemAt(OX_MenuList* menu, int index)
{
    if (menu == NULL) { OX_DebugError("OX_RemoveMenuItemAt(%p, %d) - Null pointer", menu, index); return; }
    if ((uint32_t)index >= menu->item_count) { OX_DebugError("OX_RemoveMenuItemAt(%p, %d) - Index out of bounds", menu, index); return; }
    if (menu->item_count == 1 && index == 0) { OX_ClearMenuItems(menu); return; }

    OX_MenuItem* items = OX_Allocate((menu->item_count - 1) * sizeof(OX_MenuItem));

    size_t i, j = 0;
    for (i = 0; i < menu->item_count; i++) { if (i != index) { items[j++] = menu->items[i]; } }
    OX_Free(menu->items);

    menu->items = items;
    menu->item_count--;
}

OX_MenuItem* OX_GetMenuItem(OX_MenuList* menu, int index)
{
    if (menu == NULL) { OX_DebugError("OX_GetMenuItem(00000000, %d) - Null pointer", index); return NULL; }
    if ((uint32_t)index >= menu->item_count) { OX_DebugError("OX_GetMenuItem(%p, %d) - Index out of bounds", menu, index); return NULL; }
    return &menu->items[index];
}

void OX_MenuListNext(OX_MenuList* menu)
{
    if (menu == NULL) { OX_DebugError("OX_MenuListNext(00000000) - Null pointer"); return; }
    if (menu->selected < menu->item_count - 1) 
    { 
        menu->selected++; 
        
        if (menu->selected >= menu->max_visible / 2) { menu->scroll++; }
    }
}

void OX_MenuListPrevious(OX_MenuList* menu)
{
    if (menu == NULL) { OX_DebugError("OX_MenuListPrevious(00000000) - Null pointer"); return; }
    if (menu->selected > 0) 
    { 
        menu->selected--; 
        if (menu->scroll > 0) { menu->scroll--; }
    }
}