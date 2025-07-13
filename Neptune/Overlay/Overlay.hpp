#pragma once

#include <Windows.h>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <d3d11.h>
#include <dwmapi.h>

#include <tchar.h>

#include "../Roblox/Definitions/Definitions.hpp"

#define d_toggle_bind 0x2D /* VK_INSERT */

namespace Overlay
{
	void InitializeOverlay();
	void MoveWindow(HWND hwnd);

	bool IsFullscreen(HWND hwnd);

	void DrawEsp();
	void DrawGui();

	inline bool enabled = true;
};