#pragma once
#include <NULLEngine/Inc/Component.h>
#include "TypeIds.h"

namespace
{
    class AnimatorComponent;
}

class AnimationChangeComponent final : public NULLEngine::Component
{
public:
    SET_TYPE_ID(CustomComponentId::AnimationChangeComponentId);

    void Initialize() override;
    void Terminate() override;

    void Update(float deltaTime) override;

    void SetAnimation(int index);

private:
    AnimatorComponent* mAnimatorComponent;
};
