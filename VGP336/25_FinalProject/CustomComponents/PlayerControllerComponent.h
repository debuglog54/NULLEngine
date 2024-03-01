#pragma once

#include <NULLEngine/Inc/Component.h>
#include "NULLEngine/Inc/RigidBodyComponent.h"
#include "NULLEngine/Inc/TransformComponent.h"
#include "NULLEngine/Inc/AnimatorComponent.h"
#include "NULLEngine/Inc/GameWorld.h"
#include "GameManagerService.h"

#include "TypeIds.h"

enum class PlayerAnimations
{
    DefaultModel = 0,
    Idle1 = 1,
    Idle2,
    Walk,
    Run1,
    Run2,
    Jump,
    Attack1,
    Attack2,
    Attack3,
};

class PlayerControllerComponent final : public NULLEngine::Component
{
public:
    SET_TYPE_ID(CustomComponentId::PlayerController);

    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void DebugUI() override;

    void Move(Vector3 inputDirection, float deltaTime);
    void LookAtRotate(Vector3 lookAt, float deltaTime);
    void Jump();
    void Idle(float deltaTime);
    void SetRunFlag(bool isRunning);
    void Attack();
    bool IsAttacking() { return mIsAttacking; }

    Transform* GetTransform() { return mTransformComponent; }
private:
    RigidBodyComponent* mRigidBodyComponent = nullptr;
    TransformComponent* mTransformComponent = nullptr;
    AnimatorComponent* mAnimatorComponent = nullptr;

    float mJumpHeight = 7.0f;
    float mMoveSpeed = 5.0f;
    bool mIsRunning = false;
    int mAttackStyleCounter = 1;

    bool mIsAttacking = false;
    float mCurrentAttackTimer = 0.0f;
    float mAttackTime = 3.0f;

    void UpdateTimers(float deltaTime);
    void PlayAnimation(PlayerAnimations animationIndex, bool isLooping = true);
};
