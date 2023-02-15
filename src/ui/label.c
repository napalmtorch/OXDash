#include "ui/label.h"
#include "core/common.h"

OX_Label* OX_CreateLabel(int x, int y, bool autosz, const char* text, const char* name, OX_Container* parent)
{
    return OX_CreateLabelEx(x, y, 92, 22, autosz, text, name, parent);
}

OX_Label* OX_CreateLabelEx(int x, int y, int w, int h, bool autosz, const char* text, const char* name, OX_Container* parent)
{
    OX_Label* label = (OX_Label*)OX_CreateWidget(x, y, w, h, text, name, OX_WIDGET_LABEL, parent, sizeof(OX_Label));
    
    label->base.style = OX_DEFAULT_STYLE_LABEL;
    label->autosize   = autosz;

    label->base.e_update  = (OX_WidgetEvent)OX_UpdateLabel;
    label->base.e_draw    = (OX_WidgetEvent)OX_DrawLabel;
    label->base.e_dispose = (OX_WidgetEvent)OX_FreeLabel;
    return label;
}

void OX_FreeLabel(OX_Label* label)
{
    OX_FreeWidget((OX_Widget*)label);
}

void OX_UpdateLabel(OX_Label* label)
{
    OX_UpdateWidget((OX_Widget*)label);

    if (label->autosize)
    {
        int w = OX_GetFontStringWidth(&label->base.style.font, label->base.text, true);
        label->base.bounds.w = w;
        label->base.bounds.h = OX_GetFontHeight(&label->base.style.font, true);
    }
}

void OX_DrawLabel(OX_Label* label)
{
    OX_DrawWidget((OX_Widget*)label);
}