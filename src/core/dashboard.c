#include "core/dashboard.h"
#include "core/common.h"

// ------ misc --------------------------------------
static OX_Image _bg;
static OX_ARGB  _bgcol;
static OX_Rect  _overscan;
static bool     _inv, _btn_back, _btn_ok;
static time_t   _last, _mlast;
static uint32_t _fps, _frames, _timer;

// ------ ui components -----------------------------
static OX_Label* _label_time;

// ------- strings ----------------------------------
static char _str_time[32];
static char _str_fps[16];

void OX_InitDashboard()
{
    _inv      = true;
    _bgcol    = 0xFF003F00;
    _bg       = OX_LoadBitmap("A:\\bg.bmp");
    _overscan = OX_CreateRect(20, 20, 640 - 40, 480 - 40);
    _btn_back = false;

    _label_time = OX_CreateLabel(640 - 100, 0, true, "", "label_tm", NULL);
 
    OX_INIFile file = OX_LoadINI("A:\\config.ini");

    char* vid_mode = OX_ReadStringINI(&file, "VIDEO", "mode");
    int   bpp      = OX_ReadIntINI(&file,    "VIDEO", "bpp");
    int   refresh  = OX_ReadIntINI(&file,    "VIDEO", "refresh");
    char* user     = OX_ReadStringINI(&file, "MISC",  "user");
    OX_DebugPrint("VID:%s BPP:%d REFRESH:%d USER:%s\n", vid_mode, bpp, refresh, user);
    OX_FreeINI(&file);

    OX_DebugPrint("Initialized dashboard\n");
}

void OX_UpdateDashboard()
{
    TIME_FIELDS now = OX_GetXboxTime();

    if (_last != now.Second)
    {
        _last   = now.Second;
        _fps    = _frames;
        _frames = 0;
        _inv    = true;

        sprintf(_str_time, "%02d:%02d:%02d", now.Hour, now.Minute, now.Second);
        sprintf(_str_fps, "%u FPS", _fps);
            
        OX_SetWidgetText(_label_time, _str_time, false, false);
    }

    uint32_t ms = now.Millisecond;
    if (_mlast != ms) { _mlast = ms; _timer++; }
    if (_timer >= 8) { _timer = 0; _inv   = true; }

    if (_label_time != NULL) { _label_time->base.e_update(_label_time); }

    OX_UpdatePages();

    if (!OX_GetGamepadButton(OX_GAMEPAD_1, OX_GAMEPAD_BTN_B)) { _btn_back = false; }
    if (!OX_GetGamepadButton(OX_GAMEPAD_1, OX_GAMEPAD_BTN_A)) { _btn_ok   = false; }
}

void OX_DrawDashboard()
{
    // check for invalidation before redrawing screen
    if (_inv)
    {
        _frames++;

        // reset invalidation flag
        _inv = false;

        // rendering objects
        OX_Image* screen = OX_GetVideoBackBuffer();
        OX_Font   font   = (OX_Font){ OX_FONTTYPE_PSF, (void*)&OX_DEFAULT_PSF };

        if (_bg.data != NULL) { OX_CopyImage(screen, &_bg, 0, 0); }
        else { OX_ClearImage(screen, _bgcol); }

        OX_DrawPages();

        if (_label_time != NULL)
        {
            _label_time->base.bounds.x = _overscan.x + (_overscan.w - _label_time->base.bounds.w);
            _label_time->base.bounds.y = _overscan.y;
            _label_time->base.e_draw(_label_time);
        }

        OX_ImageDrawString(screen, _overscan.x, _overscan.y, _str_fps, 0xFF00FF00, 0xFF000000, &font);

        OX_SwapVideoBuffers();
        XVideoWaitForVBlank();
    }
}

void OX_SetOverscan(int x, int y, int w, int h) { _overscan = OX_CreateRect(x, y, w, h); }

OX_Rect OX_GetOverscan() { return _overscan; }

void OX_Invalidate() { _inv = true; }

bool OX_IsInvalidated() { return _inv; }

void OX_SetBackBtn(bool state) { _btn_back = state; }

bool OX_IsBackBtnDown() { return _btn_back; }

void OX_SetOKBtn(bool state) { _btn_ok = state; }

bool OX_IsOKBtnDown() { return _btn_ok; }