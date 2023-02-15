#include "ui/widget.h"
#include "core/common.h"

OX_Widget* OX_CreateWidget(int x, int y, int w, int h, const char* text, const char* name, OX_WIDGET type, OX_Container* parent, size_t sz)
{
    if (name == NULL || strlen(name) == 0 || strlen(name) > 31) { OX_DebugError("OX_CreateWidget(%d, %d, %d, %d, %p, %p, %d, %p) - Invalid name", x, y, w, h, text, name, type, parent); return NULL; }    
    OX_Widget* widget = (OX_Widget*)OX_Allocate(sz);
    widget->bounds     = OX_CreateRect(x, y, w, h);
    widget->text       = (text == NULL ? NULL : OX_CreateString(text));
    widget->text_align = OX_TXTALIGN_LEFT;
    widget->type       = type;
    widget->parent     = parent;
    widget->hover      = false;
    widget->down       = false;
    widget->active     = true;
    widget->style      = OX_DEFAULT_STYLE;
    strcpy(widget->name, name);

    widget->e_update  = OX_UpdateWidget;
    widget->e_draw    = OX_DrawWidget;
    widget->e_dispose = OX_FreeWidget;
    return widget;
}

void OX_FreeWidget(OX_Widget* widget)
{
    if (widget == NULL) { OX_DebugError("OX_FreeWidget(00000000) - Null pointer"); return; }
    if (widget->text != NULL) { OX_FreeString(widget->text); }
    OX_Free(widget);
}

OX_Container* OX_CreateContainer(int x, int y, int w, int h, const char* name, OX_Container* parent, size_t sz)
{
    OX_Container* container = (OX_Container*)OX_CreateWidget(x, y, w, h, "", name, OX_WIDGET_CONTAINER, parent, sz);
    container->backbuffer   = OX_CreateImage(w, h);
    container->widgets      = NULL;
    container->widget_count = 0;

    container->base.e_update  = (OX_WidgetEvent)OX_UpdateContainer;
    container->base.e_draw    = (OX_WidgetEvent)OX_DrawContainer;
    container->base.e_dispose = (OX_WidgetEvent)OX_FreeContainer;
    return container;
}

void OX_FreeContainer(OX_Container* container)
{
    if (container == NULL) { OX_DebugError("OX_FreeContainer(00000000) - Null pointer"); return; }
    if (container->widgets != NULL)
    {
        for (size_t i = 0; i < container->widget_count; i++)
        {
            container->widgets[i]->e_dispose(container->widgets[i]);
        }
    }
}


void OX_UpdateWidget(OX_Widget* widget)
{
    if (widget == NULL) { OX_DebugError("OX_UpdateWidget(00000000) - Null pointer"); return; }
}

void OX_DrawWidget(OX_Widget* widget)
{
    if (widget == NULL) { OX_DebugError("OX_DrawWidget(00000000) - Null pointer"); return; }

    OX_Image* target = (widget->parent != NULL ? &widget->parent->backbuffer : OX_GetVideoBackBuffer());

    OX_ImageFillRect(target, widget->bounds.x, widget->bounds.y, widget->bounds.w, widget->bounds.h, widget->style.bg);

    if (widget->style.bdr_sz > 0)
    {
        OX_ImageDrawRect(target, widget->bounds.x, widget->bounds.y, widget->bounds.w, widget->bounds.h, widget->style.bdr_sz, widget->style.bdr);
    }

    if (widget->text != NULL && strlen(widget->text) > 0)
    {
        OX_Point sz  = OX_CreatePoint(OX_GetFontStringWidth(&widget->style.font, widget->text, true), OX_GetFontHeight(&widget->style.font, true));
        OX_Point pos = OX_CreatePoint(widget->bounds.x, widget->bounds.y);

        if (widget->text_align == OX_TXTALIGN_MIDDLE) { pos = OX_CreatePoint(widget->bounds.x + ((widget->bounds.w / 2) - (sz.x / 2)), widget->bounds.y + ((widget->bounds.h / 2) - (sz.y / 2))); }
        else if (widget->text_align == OX_TXTALIGN_RIGHT) { }

        OX_ImageDrawString(target, pos.x, pos.y, widget->text, widget->style.fg, OX_ARGB_TRANSPARENT, &widget->style.font);
    }
}

void OX_UpdateContainer(OX_Container* container)
{
    OX_UpdateWidget((OX_Widget*)container);
}

void OX_DrawContainer(OX_Container* container)
{
    OX_DrawWidget((OX_Widget*)container);
}

void OX_SetWidgetText(OX_Widget* widget, char* txt, bool copy, bool dispose_old)
{
    if (widget == NULL) { OX_DebugError("OX_SetWidgetText(00000000, %p, %d) - Null pointer", txt, copy); return; }
    if (widget->text != NULL && dispose_old) { OX_FreeString(widget->text); }
    widget->text = (copy ? OX_CreateString(txt) : txt);
    OX_Invalidate();
}