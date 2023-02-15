#include "system/input.h"
#include "core/common.h"

static SDL_GameController* _gamepads[OX_GAMEPAD_MAX];

void OX_InitInput()
{
    memset(&_gamepads, 0, sizeof(_gamepads));

    if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0) { OX_DebugError("OX_InitInput() - %s", SDL_GetError()); return; }

    OX_DebugPrint("Initialized input manager\n");
}

void OX_HandleInputEvents(SDL_Event* e)
{
    switch (e->type)
    {
        case SDL_CONTROLLERDEVICEADDED:
        {
            OX_DebugPrint("Detected gamepad %d\n", e->cdevice.which);
            SDL_GameController* gc = SDL_GameControllerOpen(e->cdevice.which);
            OX_RegisterGamepad(gc);
            break;
        }
        case SDL_CONTROLLERDEVICEREMOVED:
        {
            SDL_GameController* gc = SDL_GameControllerFromInstanceID(e->cdevice.which);
            OX_UnregisterGamepad(gc);
            break;
        }
    }
}

bool OX_RegisterGamepad(SDL_GameController* gc)
{
    if (gc == NULL) { OX_DebugError("OX_RegisterGamepad(%p) - Null pointer", gc); return false; }
    for (uint8_t i = 0; i < OX_GAMEPAD_MAX; i++)
    {
        if (_gamepads[i] == NULL)
        {
            _gamepads[i] = gc;
            OX_DebugPrint("Successfully registered gamepad %p to index %d\n", gc, i);
            return true;
        }
    }
    OX_DebugError("OX_RegisterGamepad(%p) - Unexpected failure", gc);
    return false;
}

bool OX_UnregisterGamepad(SDL_GameController* gc)
{
    if (gc == NULL) { OX_DebugError("OX_UnregisterGamepad(%p) - Null pointer", gc); return false; }
    for (uint8_t i = 0; i < OX_GAMEPAD_MAX; i++)
    {
        if (_gamepads[i] == NULL)
        {
            _gamepads[i] = NULL;
            SDL_GameControllerClose(gc);
            OX_DebugPrint("Unregistered gamepad %p at index %d\n", gc, i);
            return true;
        }
    }
    OX_DebugError("OX_UnregisterGamepad(%p) - Unexpected failure", gc);
    return false;
}

bool OX_GetGamepadButton(OX_GAMEPAD_ID gamepad_id, OX_GAMEPAD_BTN btn)
{
    SDL_GameController* gc = OX_GetGamepad(gamepad_id);
    if (gc == NULL) { return false; }
    return SDL_GameControllerGetButton(gc, (SDL_GameControllerButton)btn);
}

int OX_GetGamepadAxis(OX_GAMEPAD_ID gamepad_id, OX_GAMEPAD_AXIS axis)
{
    SDL_GameController* gc = OX_GetGamepad(gamepad_id);
    if (gc == NULL) { return false; }
    return SDL_GameControllerGetAxis(gc, (SDL_GameControllerAxis)axis);
}

SDL_GameController* OX_GetGamepad(OX_GAMEPAD_ID gamepad_id)
{
    if ((uint32_t)gamepad_id >= OX_GAMEPAD_MAX) { OX_DebugError("OX_GetGamepad(%u) - Invalid gamepad id", (uint32_t)gamepad_id); return NULL; }
    return _gamepads[(uint32_t)gamepad_id];
}