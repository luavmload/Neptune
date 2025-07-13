#pragma once

#include "../Overlay.hpp"

#include <imgui.h>

struct ESP
{
public:
	static void DrawEspForPlayer(uintptr_t player, ImColor color);

private:
	static vector3_t vectorSub(vector3_t one, vector3_t two);
};

