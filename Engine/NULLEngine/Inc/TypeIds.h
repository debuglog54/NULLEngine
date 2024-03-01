#pragma once

#include "Common.h"

namespace NULLEngine
{
    enum class ComponentId
    {
        Transform,
        Camera,
        FPSCameraController,
        ModelComponent,
        MeshComponent,
        ColliderComponent,
        RigidBodyComponent,
        AnimatorComponent,

        Count
    };

    enum class ServiceId
    {
        Camera,
        Render, 

        Count
    };
}

#define SET_TYPE_ID(id)\
    static uint32_t StaticGetTypeId() { return static_cast<uint32_t>(id); }\
    uint32_t GetTypeId() const override { return StaticGetTypeId(); }