#pragma once

// ---- STANDARD LIBRARIES --------------------------------------------------------
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// ---- WINDOWS LIBRARIES ---------------------------------------------------------
#include <windows.h>
#include <SDL.h>

// ---- XNOX LIBRARIES ------------------------------------------------------------
#include <hal/debug.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <hal/fileio.h>
#include <xboxkrnl/xboxkrnl.h>

// ---- NXDK LIBRARIES ------------------------------------------------------------
#include <nxdk/format.h>
#include <nxdk/mount.h>
#include <nxdk/path.h>

// ---- OPENSOURCE-X LIBRARIES ----------------------------------------------------
#include "lib/types.h"
#include "lib/string.h"
#include "lib/parsing/bitmap.h"
#include "lib/parsing/lodepng.h"
#include "lib/parsing/png.h"
#include "lib/parsing/ini.h"
#include "lib/math/point.h"
#include "lib/math/rect.h"

// ---- OPENSOURCE-X --------------------------------------------------------------
#include "core/debug.h"
#include "core/dashboard.h"
#include "system/exec.h"
#include "system/memory.h"
#include "system/filesys.h"
#include "system/input.h"
#include "system/video.h"
#include "system/xbox.h"
#include "gfx/color.h"
#include "gfx/font.h"
#include "gfx/image.h"
#include "gfx/psf.h"
#include "gfx/sprfont.h"
#include "ui/widget.h"
#include "ui/label.h"
#include "ui/menulist.h"
#include "ui/page.h"
#include "ui/pages/page_home.h"
#include "ui/pages/page_settings.h"
#include "ui/pages/page_games.h"

//#define DEBUGGING_MEM