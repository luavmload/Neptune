#pragma once

#include <Windows.h>
#include <cstdint>

namespace Globals
{
	inline HANDLE processHandle;
	inline uintptr_t moduleBaseAddress;

	inline bool localplayerCheck = true;

	inline bool espEnabled = false;
	inline bool tracersEnabled = false;

	inline float fillColor[4] = { 255, 255, 255, 125 };
	inline float tracersColor[4] = { 255, 255, 255, 255 };
	inline float colorDummy[4] = { 255, 255, 255, 255 };
};