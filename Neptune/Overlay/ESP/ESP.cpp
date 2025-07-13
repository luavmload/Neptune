#include "ESP.hpp"

vector3_t ESP::vectorSub(vector3_t one, vector3_t two)
{
	return { one.x - two.x, one.y - two.y, one.z - two.z };
}

void ESP::DrawEspForPlayer(uintptr_t player, ImColor color)
{
	ImDrawList* drawList = ImGui::GetBackgroundDrawList();

	POINT cursorPosition;
	GetCursorPos(&cursorPosition);
	ScreenToClient(FindWindowA(0, "Roblox"), &cursorPosition);

	uintptr_t datamodel = RobloxInstance::GetDataModel();
	uintptr_t workspace = RobloxInstance::FindFirstChild(RobloxInstance::GetChildren(datamodel), "Workspace");

	uintptr_t visualEngine = RobloxInstance::GetVisualEngine(); 
	vector2_t dimensions = Memory::ReadMemory<vector2_t>(visualEngine + Offsets::VisualEngine::Dimensions);
	vector3_t cameraPosition = Memory::ReadMemory<vector3_t>(visualEngine + Offsets::VisualEngine::CameraPosition);
	matrix4_t viewMatrix = Memory::ReadMemory<matrix4_t>(visualEngine + Offsets::VisualEngine::ViewMatrix);

	ImGui::SetNextWindowSize(ImVec2(600, 400));

	ImGui::Begin("esp-overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
	{
		std::string name = RobloxInstance::GetName(player);
		if (name.empty())
		{
			ImGui::End();
			return;
		}

		uintptr_t character = RobloxInstance::FindFirstChild(RobloxInstance::GetChildren(workspace), name);
		if (!character)
		{
			ImGui::End();
			return;
		}

		uintptr_t humanoid = RobloxInstance::FindFirstChild(RobloxInstance::GetChildren(character), "Humanoid");

		uintptr_t head = RobloxInstance::FindFirstChild(RobloxInstance::GetChildren(character), "Head");
		vector3_t headPosition3d = RobloxInstance::GetPartPosition(head);

		//vector2_t headPosition = RobloxInstance::WorldToScreen(vectorSub(headPosition3d, {0, 2, 0}), dimensions, viewMatrix);

		//ImGui::Text("%s: %f %f %f", name.c_str(), headPosition3d.x, headPosition3d.y, headPosition3d.z);

		//ImVec4 color = ImVec4(Globals::colorDummy[0], Globals::colorDummy[1], Globals::colorDummy[2], Globals::colorDummy[3]);
		//drawList->AddLine(ImVec2(headPosition.x, headPosition.y), ImVec2(static_cast<float>(cursorPosition.x), static_cast<float>(cursorPosition.y)), ImGui::ColorConvertFloat4ToU32(color));
	}
	ImGui::End();
}
