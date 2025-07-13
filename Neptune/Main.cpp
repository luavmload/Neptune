#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>

#include "Roblox/Manager/Manager.hpp"
#include "Roblox/Definitions/Definitions.hpp"

#include "Overlay/Overlay.hpp"

#include "Globals.hpp"

char buffer[64];

int main() 
{
	SetConsoleTitleA("Neptune Console");

	DWORD processId = Manager::GetProcessId(L"RobloxPlayerBeta.exe");
	if (processId == 0) 
	{
		std::cerr << "Failed to find Roblox" << std::endl;

		std::cout << std::endl << "Press any key to exit..." << std::endl;
		std::cin.get();

		return 1;
	}

	Globals::processHandle = Manager::AttachToProcess(processId);
	if (Globals::processHandle == 0 || Globals::processHandle == INVALID_HANDLE_VALUE) 
	{
			std::cerr << "Failed to attach to Roblox: " << GetLastErrorAsString() << std::endl;

			std::cout << std::endl << "Press any key to exit..." << std::endl;
			std::cin.get();

			return 1;
	}

	Globals::moduleBaseAddress = Manager::GetModuleBaseAddress(Manager::GetProcessId(L"RobloxPlayerBeta.exe"), L"RobloxPlayerBeta.exe");

	std::cout << "Successfully attached to Roblox process with PID: " << processId << std::endl;

	/*uintptr_t datamodel = RobloxInstance::GetDataModel();
	
	uintptr_t workspace = RobloxInstance::FindFirstChild(RobloxInstance::GetChildren(datamodel), "Workspace");
	uintptr_t players = RobloxInstance::FindFirstChild(RobloxInstance::GetChildren(datamodel), "Players");

	uintptr_t localPlayer = Memory::ReadMemory<uintptr_t>(players + Offsets::Players::LocalPlayer);
	uintptr_t localCharacter = RobloxInstance::FindFirstChild(RobloxInstance::GetChildren(workspace), RobloxInstance::GetName(localPlayer));
	uintptr_t localHumanoid = RobloxInstance::FindFirstChild(RobloxInstance::GetChildren(localCharacter), "Humanoid");

	std::cout << "Players: " << std::endl;

	uintptr_t top = Memory::ReadMemory<uintptr_t>(RobloxInstance::GetChildren(players));
	uintptr_t end = Memory::ReadMemory<uintptr_t>(RobloxInstance::GetChildren(players) + Offsets::General::ChildrenEnd) + 1;
	for (uintptr_t childPtr = top; childPtr < end; childPtr += 0x10) // A child has a size of 0x10 
	{
		uintptr_t childAddress = Memory::ReadMemory<uintptr_t>(childPtr);
		if (childAddress == 0)
			break; // Couldn't find any children

		//if (childAddress == localPlayer) 
		//	continue;

		std::string name = RobloxInstance::GetName(childAddress);

		std::cout << name << std::endl;

		uintptr_t character = RobloxInstance::FindFirstChild(RobloxInstance::GetChildren(workspace), name);

		uintptr_t head = RobloxInstance::FindFirstChild(RobloxInstance::GetChildren(character), "Head");
		vector3_t headPosition = RobloxInstance::GetPartPosition(head);
		
		std::cout << "Pos: ("
			<< headPosition.x << ", "
			<< headPosition.y << ", "
			<< headPosition.z << ")" << std::endl;
	}*/

	//Overlay::InitializeOverlay();

	std::cout << "Base: 0x" << std::hex << Manager::GetModuleBaseAddress(Manager::GetProcessId(L"RobloxPlayerBeta.exe"), L"RobloxPlayerBeta.exe") << std::endl;
	std::cout << "Visual Engine: 0x" << std::hex << RobloxInstance::GetVisualEngine() << std::endl;

	CloseHandle(Globals::processHandle);

	std::cout << std::endl << "Press any key to exit..." << std::endl;
	std::cin.get();

	return 0;
}