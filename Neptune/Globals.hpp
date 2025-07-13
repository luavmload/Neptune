#pragma once

#include <Windows.h>
#include <cstdint>

namespace Globals
{
	inline HANDLE processHandle;
	inline uintptr_t moduleBaseAddress;

	inline bool espEnabled = false;

	inline float colorDummy[4] = { 255, 255, 255, 255 };
};