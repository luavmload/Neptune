#pragma once

#include <cstdint>

#include <Windows.h>
#include <iostream>
#include <string>

#include "../../Globals.hpp"

inline std::string GetLastErrorAsString() {
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0)
        return "No error message"; 

    LPSTR messageBuffer = nullptr;

    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&messageBuffer, 0, nullptr);

    std::string message(messageBuffer, size);
    LocalFree(messageBuffer);

    return message;
}

struct Memory
{
public:
    template <typename T>
    static T ReadMemory(uintptr_t address)
    {
        T buffer;
        size_t bytesRead = 0;

        if (ReadProcessMemory(Globals::processHandle, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), &bytesRead) && bytesRead == sizeof(T))
        {
            return buffer;
        }

        std::cerr << "Failed to read memory: " << GetLastErrorAsString() << std::endl;

        return T();
    }

    template <typename T>
    static bool WriteMemory(uintptr_t address, const T value)
    {
        DWORD oldProtect;
        SIZE_T written;

        if (!VirtualProtectEx(Globals::processHandle, (LPVOID)address, sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect))
        {
            std::cerr << "Failed to change memory protection: " << GetLastErrorAsString() << std::endl;
            return false;
        }

        if (!WriteProcessMemory(Globals::processHandle, (LPVOID)address, &value, sizeof(T), &written) || written != sizeof(T))
        {
            std::cerr << "Failed to write memory: " << GetLastErrorAsString() << std::endl;
            VirtualProtectEx(Globals::processHandle, (LPVOID)address, sizeof(T), oldProtect, &oldProtect);
            return false;
        }

        VirtualProtectEx(Globals::processHandle, (LPVOID)address, sizeof(T), oldProtect, &oldProtect);
        return true;
    }
};
