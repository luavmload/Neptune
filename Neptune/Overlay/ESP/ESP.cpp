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

		vector2_t headPosition = RobloxInstance::WorldToScreen(vectorSub(headPosition3d, {0, 2, 0}), dimensions, viewMatrix);
		vector2_t legPosition = RobloxInstance::WorldToScreen(vectorSub(headPosition3d, { 0, 5, 0 }), dimensions, viewMatrix);

		float height = headPosition.y - legPosition.y;
		float width = height / 1.6f;

		if (Globals::espEnabled)
		{
			float outlineOffset = 0.1f;

			ImVec4 fillColor = ImVec4(Globals::fillColor[0], Globals::fillColor[1], Globals::fillColor[2], Globals::fillColor[3]);
			
			drawList->AddRectFilled(ImVec2(legPosition.x - (width / 1.3f) - outlineOffset, legPosition.y - outlineOffset), ImVec2(headPosition.x + width + outlineOffset, headPosition.y + height + outlineOffset), ImGui::ColorConvertFloat4ToU32(fillColor));
			//ImGui::Text("%s: %f %f %f", name.c_str(), headPosition3d.x, headPosition3d.y, headPosition3d.z);
		}

		if (Globals::tracersEnabled)
		{ 
			ImVec4 color = ImVec4(Globals::tracersColor[0], Globals::tracersColor[1], Globals::tracersColor[2], Globals::tracersColor[3]);
			drawList->AddLine(ImVec2(headPosition.x, headPosition.y), ImVec2(static_cast<float>(cursorPosition.x), static_cast<float>(cursorPosition.y)), ImGui::ColorConvertFloat4ToU32(color));
		}
	}
	ImGui::End();
}
