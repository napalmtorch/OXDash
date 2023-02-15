#pragma once
#include "lib/types.h"

typedef struct OX_Page OX_Page;

typedef void (*OX_PageEvent)(OX_Page*);

struct OX_Page
{
    OX_PageEvent e_update;
    OX_PageEvent e_draw;
    OX_PageEvent e_selected;
    OX_PageEvent e_dispose;
    char*        name;
};

void OX_InitPages();
void OX_FreePages();
void OX_UpdatePages();
void OX_DrawPages();
void OX_GotoLastPage();

void     OX_SetPage(const char* name);
OX_Page* OX_GetPage(const char* name);
OX_Page* OX_GetSelectedPage();

OX_Page* OX_CreatePage(const char* name, size_t sz, OX_PageEvent selected, OX_PageEvent dispose, OX_PageEvent update, OX_PageEvent draw);
void OX_FreePage(OX_Page* page);
void OX_UpdatePage(OX_Page* page);
void OX_DrawPage(OX_Page* page);
void OX_SelectedPage(OX_Page* page);