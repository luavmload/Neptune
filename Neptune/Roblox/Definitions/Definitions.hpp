#pragma once

#include <cstdint>
#include <string>

#include "./Offsets.hpp"

#include "../Memory/Memory.hpp"

#include "../../Globals.hpp"

struct vector3_t final { float x, y, z; };
struct vector2_t final { float x, y; };
struct quaternion final { float x, y, z, w; };
struct matrix4_t final { float data[16]; };

namespace RobloxInstance
{
	uintptr_t GetDataModel();
	uintptr_t GetVisualEngine();

	uintptr_t GetChildren(uintptr_t address);
	uintptr_t FindFirstChild(uintptr_t children, std::string name);
	
	std::string GetName(uintptr_t address);

	vector3_t GetPartPosition(uintptr_t address);
	vector2_t WorldToScreen(vector3_t world, vector2_t dimensions, matrix4_t viewmatrix);
}

namespace Roblox
{
	std::string RobloxString(uintptr_t address);

	void SetWalkspeed(uintptr_t localHumanoid, float walkspeed);
}