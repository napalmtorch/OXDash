#pragma once
#include "ui/page.h"
#include "ui/menulist.h"

typedef struct
{
    OX_Page      base;
    OX_MenuList* menu;
} OX_PageHome;

OX_PageHome* OX_CreatePageHome();
void OX_SelectedHomePageMenu(OX_MenuItem* item);