#pragma once
#include "ui/page.h"
#include "ui/menulist.h"

typedef struct
{
    OX_Page      base;
    OX_MenuList* menu;
} OX_PageSettings;

OX_PageSettings* OX_CreatePageSettings();
void OX_SelectedSettingsPageMenu(OX_MenuItem* item);