#pragma once
#include "lib/types.h"
#include "lib/math/rect.h"
#include "gfx/color.h"
#include "gfx/psf.h"
#include "gfx/font.h"
#include "gfx/image.h"

typedef struct OX_Widget OX_Widget;
typedef struct OX_Container OX_Container;

typedef void (*OX_WidgetEvent)(OX_Widget*);

typedef enum
{
    OX_WIDGET_CONTAINER,
    OX_WIDGET_LIST,
    OX_WIDGET_LISTITEM,
    OX_WIDGET_LABEL,
} OX_WIDGET;

typedef enum
{
    OX_TXTALIGN_LEFT,
    OX_TXTALIGN_MIDDLE,
    OX_TXTALIGN_RIGHT,
} OX_TXTALIGN;

typedef struct
{
    OX_ARGB   bg,  bg_hover,  bg_down;
    OX_ARGB   fg,  fg_hover,  fg_down;
    OX_ARGB   bdr, bdr_hover, bdr_down;
    uint8_t   bdr_sz;
    OX_Font   font;
    char      name[32];
} OX_WidgetStyle;

struct OX_Widget
{
    OX_WidgetEvent e_update;
    OX_WidgetEvent e_draw;
    OX_WidgetEvent e_dispose;
    OX_WIDGET      type;
    OX_TXTALIGN    text_align;
    OX_WidgetStyle style;
    OX_Container*  parent;
    OX_Rect        bounds;
    bool           hover, down, active;
    char*          text;
    char           name[32];
};

struct OX_Container
{
    OX_Widget   base;
    OX_Image    backbuffer;
    OX_Widget** widgets;
    size_t      widget_count;
};

static const OX_WidgetStyle OX_DEFAULT_STYLE = (OX_WidgetStyle)
{
    0xFF181818, 0xFF242424, 0xFF121212,
    0xFFCFCFCF, 0xFFFFFFFF, 0xFFAFAFAF,
    0xFFCFCFCF, 0xFFFFFFFF, 0xFFAFAFAF,
    1,
    (OX_Font){ OX_FONTTYPE_PSF, (void*)&OX_DEFAULT_PSF },
    "default",
};

static const OX_WidgetStyle OX_DEFAULT_STYLE_LABEL = (OX_WidgetStyle)
{
    0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF,
    0xFFCFCFCF, 0xFFFFFFFF, 0xFFAFAFAF,
    0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF,
    0,
    (OX_Font){ OX_FONTTYPE_PSF, (void*)&OX_DEFAULT_PSF },
    "default",
};

OX_Widget* OX_CreateWidget(int x, int y, int w, int h, const char* text, const char* name, OX_WIDGET type, OX_Container* parent, size_t sz);
void OX_FreeWidget(OX_Widget* widget);

OX_Container* OX_CreateContainer(int x, int y, int w, int h, const char* name, OX_Container* parent, size_t sz);
void OX_FreeContainer(OX_Container* container);

void OX_UpdateWidget(OX_Widget* widget);
void OX_DrawWidget(OX_Widget* widget);

void OX_UpdateContainer(OX_Container* container);
void OX_DrawContainer(OX_Container* container);

void OX_SetWidgetText(OX_Widget* widget, char* txt, bool copy, bool dispose_old);