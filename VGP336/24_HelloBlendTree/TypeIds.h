#pragma once
#include<NULLEngine/Inc/TypeIds.h>

enum class CustomComponentId
{
    AnimationChangeComponent= static_cast<uint32_t>(NULLEngine::ComponentId::Count),
    AnotherNewComponent,
};

enum class CustomServiceId
{
    AnimationChangeInputService = static_cast<uint32_t>(NULLEngine::ServiceId::Count),
    MoreServices,
};