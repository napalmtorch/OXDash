#pragma once
#include "ui/widget.h"

typedef struct OX_MenuList OX_MenuList;
typedef struct OX_MenuItem OX_MenuItem;

typedef void (*OX_MenuItemEvent)(OX_MenuItem* item);

struct OX_MenuItem
{
    OX_MenuList*     parent;
    char*            text;  
    OX_MenuItemEvent e_selected;
};

struct OX_MenuList
{
    OX_Widget    base;
    OX_MenuItem* items;
    size_t       item_count;
    bool         autosize_x;
    bool         autosize_y;
    bool         btn_down, btn_up, btn_sel;
    int          selected, scroll;
    int          max_visible;
};

OX_MenuItem OX_CreateMenuItem(const char* text, OX_MenuItemEvent e_sel, bool copy);

OX_MenuList* OX_CreateMenuList(int x, int y, int w, int h, const char* name, OX_Container* parent);
void OX_FreeMenuList(OX_MenuList* menu);

void OX_UpdateMenuList(OX_MenuList* menu);
void OX_DrawMenuList(OX_MenuList* menu);

void OX_ClearMenuItems(OX_MenuList* menu);
void OX_AddMenuItem(OX_MenuList* menu, OX_MenuItem item);;
void OX_RemoveMenuItem(OX_MenuList* menu, OX_MenuItem* item);
void OX_RemoveMenuItemAt(OX_MenuList* menu, int index);
OX_MenuItem* OX_GetMenuItem(OX_MenuList* menu, int index);

void OX_MenuListNext(OX_MenuList* menu);
void OX_MenuListPrevious(OX_MenuList* menu);