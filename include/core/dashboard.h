#pragma once
#include "lib/types.h"
#include "lib/math/rect.h"
#include "ui/label.h"

/// @brief Initialize dashboard, parse configuration, and prepare UI
void OX_InitDashboard();

/// @brief Update dashboard
void OX_UpdateDashboard();

/// @brief Draw dashboard
void OX_DrawDashboard();

/// @brief Set overscan boundaries @param x left, @param y top @param w width, @param h height
void OX_SetOverscan(int x, int y, int w, int h);

/// @brief Get overscan boundaries @return Overscan bounds
OX_Rect OX_GetOverscan();

/// @brief Invalidate dashboard - this will force a redraw on the next frame
void OX_Invalidate();

/// @brief Check if dashboard is invalidated
bool OX_IsInvalidated();

/// @brief Set back(B) button state @param state Button pressed
void OX_SetBackBtn(bool state);

/// @brief Get back(B) button state @return Button pressed
bool OX_IsBackBtnDown();

/// @brief Set OK(A) button state @param state Button pressed
void OX_SetOKBtn(bool state);

/// @brief Get OK(A) button state @return Button pressed
bool OX_IsOKBtnDown();