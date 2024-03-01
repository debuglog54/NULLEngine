#pragma once

#include<NULLEngine/Inc/TypeIds.h>

enum class CustomComponentId
{
    AnimationChangeComponent= static_cast<uint32_t>(NULLEngine::ComponentId::Count),
    PlayerInputControllerComponent,
    PlayerController,
    SideScrollCameraComponent,
    CoinComponent,
    AnotherNewComponent,

};

enum class CustomServiceId
{
    AnimationChangeInputService = static_cast<uint32_t>(NULLEngine::ServiceId::Count),
    GameManagerService,
};