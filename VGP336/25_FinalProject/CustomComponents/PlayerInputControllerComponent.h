#pragma once

#include <NULLEngine/Inc/Component.h>
#include "TypeIds.h"

class PlayerControllerComponent;

class PlayerInputControllerComponent final : public NULLEngine::Component
{
public:
    SET_TYPE_ID(CustomComponentId::PlayerInputControllerComponent);

    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void DebugUI() override;

private:
    //   NULLEngine::AnimatorComponent* mAnimatorComponent = nullptr;
    PlayerControllerComponent* mPlayerController = nullptr;
    

};
