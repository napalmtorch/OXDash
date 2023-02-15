#pragma once
#include "lib/types.h"

/// @brief Print a formated debug message to the screen @param str Formatted string @param ... Variadic arguments
void OX_DebugPrint(const char* str, ...);

/// @brief Print a formated error message to the screen and terminate program @param str Formatted string @param ... Variadic arguments
void OX_DebugError(const char* str, ...);