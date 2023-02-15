#pragma once
#include "ui/page.h"
#include "ui/menulist.h"

typedef struct
{
    OX_Page      base;
    OX_MenuList* menu;
} OX_PageGames;

OX_PageGames* OX_CreatePageGames();
void OX_RefreshPageGames(OX_PageGames* page);
void OX_SelectedGamesPageMenu(OX_MenuItem* item);