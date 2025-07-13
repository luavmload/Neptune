#include "Manager.hpp"

HANDLE Manager::AttachToProcess(DWORD processId)
{
	HANDLE processHandle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, 0, processId);
	if (!processHandle) {
		std::cerr << "Failed to open process with ID: " << processId << std::endl;
		return 0;
	}

	return processHandle;
}
bool Manager::DetachFromProcess(HANDLE processHandle)
{
	bool status = CloseHandle(processHandle);
	return status;
}

DWORD Manager::GetProcessId(const wchar_t* processName) 
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) {
		std::cerr << "Failed to create snapshot of processes." << std::endl;
		return 0;
	}

	PROCESSENTRY32W processEntry;
	processEntry.dwSize = sizeof(PROCESSENTRY32W);
	while (Process32NextW(snapshot, &processEntry)) {
		if (wcscmp(processEntry.szExeFile, processName) == 0) {
			CloseHandle(snapshot);
			return processEntry.th32ProcessID;
		}
	}
	
	CloseHandle(snapshot);

	std::wcerr << L"Process not found: " << processName << std::endl;
	return 0;
}

uintptr_t Manager::GetModuleBaseAddress(DWORD processId, std::string moduleName) // i made this cuz im pro
{
	MODULEENTRY32 modEntry = { 0 };
	modEntry.dwSize = sizeof(MODULEENTRY32);

	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		return {};
	}

	if (Module32First(hModuleSnap, &modEntry))
	{
		do
		{
#ifdef UNICODE
			if (_wcsicmp(modEntry.szModule, std::wstring(moduleName.begin(), moduleName.end()).c_str()) == 0)
#else
			if (_stricmp(modEntry.szModule, moduleName.c_str()) == 0)
#endif
			{
				CloseHandle(hModuleSnap);
				return (uintptr_t)modEntry.modBaseAddr;
			}
		} while (Module32Next(hModuleSnap, &modEntry));
	}

	CloseHandle(hModuleSnap);
	return {};
}