#pragma once

#include <Windows.h>
#include <TlHelp32.h>

#include <cstdint>

#include <iostream>

struct Manager
{
public:
	static HANDLE AttachToProcess(DWORD processId);
	static bool DetachFromProcess(HANDLE processHandle);

	static DWORD GetProcessId(const wchar_t* processName);

	static uintptr_t GetModuleBaseAddress(DWORD processId, const wchar_t* moduleName);
};

