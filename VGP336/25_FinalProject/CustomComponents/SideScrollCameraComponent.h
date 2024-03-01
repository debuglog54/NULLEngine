#pragma once

#include "NULLEngine/Inc/Component.h"
#include "NULLEngine/Inc/CameraComponent.h"
#include "NULLEngine/Inc/GameObject.h"
#include "NULLEngine/Inc/TransformComponent.h"
#include "TypeIds.h"

class  CameraComponent;

class SideScrollCameraComponent final
    : public Component
{
public:
    SET_TYPE_ID(CustomComponentId::SideScrollCameraComponent);

    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;

    void SetMoveSpeed(float speed) { mMoveSpeed = speed; }
    void SetTurnSpeed(float speed) { mTurnSpeed = speed; }

private:
    NULLEngine::CameraComponent* mCameraComponent = nullptr;
    const NULLEngine::TransformComponent* mPlayerTransform = nullptr;
    float mMoveSpeed = 1.0f;
    float mTurnSpeed = 0.1f;
    Vector3 mOffset = Vector3(0.0f,4.0f, -20.5f);
};
