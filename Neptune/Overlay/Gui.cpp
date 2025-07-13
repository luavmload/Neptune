#include "Overlay.hpp"

#include "ESP/ESP.hpp"

#include <imgui.h>
#include <imgui_toggle.h>

void Overlay::DrawGui()
{
	ImGui::SetNextWindowSize(ImVec2(600, 400));

	ImGui::Begin("Neptune External", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
	{
		static bool showConsole = true;

		ImGui::Toggle("Localplayer Check", &Globals::localplayerCheck);

		ImGui::Separator();

		ImGui::Toggle("ESP", &Globals::espEnabled);
		ImGui::ColorEdit4("Box Color", Globals::colorDummy, ImGuiColorEditFlags_NoInputs);
		ImGui::ColorEdit4("Fill Color", Globals::fillColor, ImGuiColorEditFlags_NoInputs);
		
		ImGui::Separator();

		ImGui::Toggle("Tracers", &Globals::tracersEnabled);
		ImGui::ColorEdit4("Tracers Color", Globals::tracersColor, ImGuiColorEditFlags_NoInputs);

		ImGui::Separator();

		if (ImGui::Toggle("Console Visible", &showConsole))
		{
			if (showConsole)
				::ShowWindow(::GetConsoleWindow(), SW_SHOW);
			else 
				::ShowWindow(::GetConsoleWindow(), SW_HIDE);
		}
	}
	ImGui::End();
}

void Overlay::DrawEsp()
{
	uintptr_t datamodel = RobloxInstance::GetDataModel();

	uintptr_t players = RobloxInstance::FindFirstChild(RobloxInstance::GetChildren(datamodel), "Players");
	uintptr_t localPlayer = Memory::ReadMemory<uintptr_t>(players + Offsets::Players::LocalPlayer);

	uintptr_t top = Memory::ReadMemory<uintptr_t>(RobloxInstance::GetChildren(players));
	uintptr_t end = Memory::ReadMemory<uintptr_t>(RobloxInstance::GetChildren(players) + Offsets::General::ChildrenEnd) + 1;
	for (uintptr_t childPtr = top; childPtr < end; childPtr += 0x10) // A child has a size of 0x10 
	{
		uintptr_t childAddress = Memory::ReadMemory<uintptr_t>(childPtr);
		if (childAddress == 0)
			break; // Couldn't find any children

		if (childAddress == localPlayer && Globals::localplayerCheck) 
			continue;

		ESP::DrawEspForPlayer(childAddress, ImColor(0, 0, 0));
	}
}