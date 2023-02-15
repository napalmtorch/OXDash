#include "core/common.h"

int main(void)
{
    // initialize core systems
    OX_InitVideo();
    OX_InitFileSystem();
    OX_InitInput();
    OX_InitPages();
    OX_InitDashboard();

    // scan for games
    OX_ScanGames("F:\\Games\\");
    OX_RefreshPageGames(OX_GetPage("games"));

    // finished booting
    OX_DebugPrint("OpenSource-X Dashboard\n");
    OX_DebugPrint("RAM: %u/%u KB\n", OX_GetMemoryUsed() / KILOBYTE, OX_GetMemoryTotal() / KILOBYTE);
    OX_DebugPrint("Entering main loop...\n");
    Sleep(2000);

    // main loop
    SDL_Event e;
    while (true)
    {
        // poll sdl-based events
        while (SDL_PollEvent(&e))
        {
            OX_HandleInputEvents(&e);
        }

        // force quit
        if (OX_GetGamepadButton(OX_GAMEPAD_1, OX_GAMEPAD_BTN_START)) { SDL_Quit(); exit(0); }

        OX_UpdateDashboard();
        OX_DrawDashboard();
    }
    return 0;
}