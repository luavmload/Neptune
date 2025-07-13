#include "Definitions.hpp"

uintptr_t RobloxInstance::GetDataModel()
{
    uintptr_t fakeDatamodel = Memory::ReadMemory<uintptr_t>(Globals::moduleBaseAddress + Offsets::Datamodel::FakeDatamodel);
    uintptr_t datamodel = Memory::ReadMemory<uintptr_t>(fakeDatamodel + Offsets::Datamodel::FakeDatamodelToDatamodel);

    return datamodel;
}

uintptr_t RobloxInstance::GetVisualEngine()
{
    uintptr_t visualEngine = Memory::ReadMemory<uintptr_t>(Globals::moduleBaseAddress + Offsets::VisualEngine::VisualEngine);
    return visualEngine;
}

uintptr_t RobloxInstance::GetChildren(uintptr_t address)
{
    return Memory::ReadMemory<uintptr_t>(address + Offsets::General::Children);
}

uintptr_t RobloxInstance::FindFirstChild(uintptr_t children, std::string name)
{
    uintptr_t top = Memory::ReadMemory<uintptr_t>(children);
    uintptr_t end = Memory::ReadMemory<uintptr_t>(children + Offsets::General::ChildrenEnd) + 1;
    for (uintptr_t childPtr = top; childPtr < end; childPtr += 0x10) // A child has a size of 0x10 
    {
        uintptr_t childAddress = Memory::ReadMemory<uintptr_t>(childPtr);
        if (childAddress == 0)
            break; // Couldn't find any children

        if (GetName(childAddress) == name)
            return childAddress;
    }

    std::cerr << "Failed to find child: " << name << std::endl;

    return 0;
}

std::string RobloxInstance::GetName(uintptr_t address)
{
    uintptr_t namePtr = Memory::ReadMemory<uintptr_t>(address + Offsets::General::Name);
    return Roblox::RobloxString(namePtr);
}

vector3_t RobloxInstance::GetPartPosition(uintptr_t address)
{
    vector3_t result{0};

    uintptr_t primitive = Memory::ReadMemory<uintptr_t>(address + Offsets::General::Primitive);
    if (!primitive)
        return result;
    
    result = Memory::ReadMemory<vector3_t>(primitive + Offsets::General::Position);
    return result;
}

vector2_t RobloxInstance::WorldToScreen(vector3_t world, vector2_t dimensions, matrix4_t viewmatrix)
{
    quaternion quaternion;

    quaternion.x = (world.x * viewmatrix.data[0]) + (world.y * viewmatrix.data[1]) + (world.z * viewmatrix.data[2]) + viewmatrix.data[3];
    quaternion.y = (world.x * viewmatrix.data[4]) + (world.y * viewmatrix.data[5]) + (world.z * viewmatrix.data[6]) + viewmatrix.data[7];
    quaternion.z = (world.x * viewmatrix.data[8]) + (world.y * viewmatrix.data[9]) + (world.z * viewmatrix.data[10]) + viewmatrix.data[11];
    quaternion.w = (world.x * viewmatrix.data[12]) + (world.y * viewmatrix.data[13]) + (world.z * viewmatrix.data[14]) + viewmatrix.data[15];

    if (quaternion.w < 0.1f)
        return{ -1, -1 };

    float inv_w = 1.0f / quaternion.w;
    vector3_t ndc;
    ndc.x = quaternion.x * inv_w;
    ndc.y = quaternion.y * inv_w;
    ndc.z = quaternion.z * inv_w;

    return
    {
        (dimensions.x / 2 * ndc.x) + (ndc.x + dimensions.x / 2),
        -(dimensions.y / 2 * ndc.y) + (ndc.y + dimensions.y / 2)
    };
}

std::string Roblox::RobloxString(uintptr_t address)
{
    int32_t check = Memory::ReadMemory<int32_t>(address + 0x18);
    if (check > 15)
        address = Memory::ReadMemory<uintptr_t>(address);

    std::string robloxString;
    for (int offset = 0; offset < 640; offset++)
    {
        char character = Memory::ReadMemory<char>(address + offset);
        if (character == '\0') break;

        robloxString.push_back(character);
    }

    return robloxString;
}

void Roblox::SetWalkspeed(uintptr_t localHumanoid, float walkspeed)
{
    Memory::WriteMemory<float>(localHumanoid + Offsets::Players::WalkSpeed, walkspeed);
    Memory::WriteMemory<float>(localHumanoid + Offsets::Players::WalkSpeedCheck, walkspeed);
}
