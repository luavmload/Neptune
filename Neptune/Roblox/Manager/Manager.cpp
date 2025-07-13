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

uintptr_t Manager::GetModuleBaseAddress(DWORD processId, const wchar_t* moduleName)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
	if (snapshot == INVALID_HANDLE_VALUE) {
		std::cerr << "Failed to create snapshot of modules." << std::endl;
		return 0;
	}

	MODULEENTRY32W moduleEntry;
	moduleEntry.dwSize = sizeof(MODULEENTRY32W);
	if (Module32FirstW(snapshot, &moduleEntry)) {
		do {
			if (wcscmp(moduleEntry.szModule, moduleName) == 0) {
				CloseHandle(snapshot);
				return reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
			}
		} while (Module32NextW(snapshot, &moduleEntry));
	}

	CloseHandle(snapshot);

	std::wcerr << L"Module not found: " << moduleName << std::endl;
	return 0;
}