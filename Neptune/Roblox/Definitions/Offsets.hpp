#pragma once

#include "../Manager/Manager.hpp"
#include "../../Globals.hpp"

#define REBASE(x) x + Globals::moduleBaseAddress;//Globals::moduleBaseAddress; 

namespace Offsets
{
	namespace General
	{
		const uintptr_t Name = 0x78;

		const uintptr_t Children = 0x80;
		const uintptr_t ChildrenEnd = 0x8;

		const uintptr_t Primitive = 0x178;
		const uintptr_t Position = 0x13C;
	}

	namespace Datamodel
	{
		const uintptr_t FakeDatamodel = /*REBASE*/(0x682B928);

		const uintptr_t FakeDatamodelToDatamodel = 0x1B8;

		//const uintptr_t PlaceId = 0x1A0;
	}

	namespace VisualEngine
	{
		const uintptr_t VisualEngine =/*REBASE*/(0x66712A8);

		const uintptr_t Dimensions = 0x720;
		const uintptr_t ViewMatrix = 0x4B0;
		const uintptr_t CameraPosition = 0x124;
	}

	namespace Players
	{
		const uintptr_t LocalPlayer = 0x128;

		//const uintptr_t UserId = 0x278;

		const uintptr_t WalkSpeed = 0x1DC;
		const uintptr_t WalkSpeedCheck = 0x3B8;
	}
}