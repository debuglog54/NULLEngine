#include "PlayerControllerComponent.h"
#include "NULLEngine/Inc/GameObject.h"


using namespace NULLEngine::EMath;

void PlayerControllerComponent::Initialize()
{
    mRigidBodyComponent = GetOwner().GetComponent<RigidBodyComponent>();
    mTransformComponent = GetOwner().GetComponent<TransformComponent>();
    mAnimatorComponent = GetOwner().GetComponent<AnimatorComponent>();
    int x = mAnimatorComponent->GetAnimator().GetAnimationCount();
    PlayAnimation(PlayerAnimations::Idle2);
    ASSERT(mRigidBodyComponent != nullptr, "PlayerInputControllerComponent: GameObject doesnt have a PlayerControllerComponent component!");

    auto gameMangerService = GetOwner().GetWorld().GetService<GameManagerService>();
    ASSERT(gameMangerService != nullptr, "PlayerComponent: GameManager Service is not available!");
    gameMangerService->Register(this);
}

void PlayerControllerComponent::Terminate()
{
}

void PlayerControllerComponent::Update(float deltaTime)
{
    UpdateTimers(deltaTime);
}

void PlayerControllerComponent::DebugUI()
{
}

void PlayerControllerComponent::Move(Vector3 inputDirection, float deltaTime)
{
    if (mIsAttacking)
        return;

    float speedMultiplier = mIsRunning ? 3.0f : 1;

    Vector3 moveForce = inputDirection * mMoveSpeed * speedMultiplier * deltaTime * 25.0f;
    mRigidBodyComponent->SetVelocity(moveForce);
    Vector3 dir = (mTransformComponent->position + moveForce) - mTransformComponent->position;
    LookAtRotate(dir, deltaTime);

    if (mIsRunning)
    {
        PlayAnimation(PlayerAnimations::Run2);
    }
    else
    {
        PlayAnimation(PlayerAnimations::Walk);
    }
}

void PlayerControllerComponent::LookAtRotate(Vector3 lookAt, float deltaTime)
{
    //Quaternion::Slerp(mTransformComponent->rotation, rotation, deltaTime * 50.0f);
    mTransformComponent->rotation = Quaternion::LookRotation(lookAt, Vector3::YAxis());
    mRigidBodyComponent->UpdateCustomRotation();
}

void PlayerControllerComponent::Jump()
{
    Vector3 up = Vector3::YAxis() * mJumpHeight;
    mRigidBodyComponent->ApplyForce(up);
    PlayAnimation(PlayerAnimations::Jump, false);
}

void PlayerControllerComponent::Idle(float deltaTime)
{
    if (Magnitude(mRigidBodyComponent->GetVelocity()) <= 0.1f && !mIsAttacking)
    {
        LookAtRotate(Vector3::ZAxis(), deltaTime);
        PlayAnimation(PlayerAnimations::Idle2);
    }
}

void PlayerControllerComponent::SetRunFlag(bool isRunning)
{
    mIsRunning = isRunning;
}

void PlayerControllerComponent::PlayAnimation(PlayerAnimations animation, bool isLooping)
{
    mAnimatorComponent->PlayAnimation(static_cast<int>(animation), isLooping);
}

void PlayerControllerComponent::Attack()
{
    if (mIsAttacking == false)
    {
        mIsAttacking = true;

        switch (mAttackStyleCounter)
        {
        case 1:
            PlayAnimation(PlayerAnimations::Attack1, false);
            mAttackTime = mAnimatorComponent->GetCurrentAnimationTime();
            break;
        case 2:
            PlayAnimation(PlayerAnimations::Attack2, false);
            mAttackTime = mAnimatorComponent->GetCurrentAnimationTime();
            break;
        case 3:
            PlayAnimation(PlayerAnimations::Attack3, false);
            mAttackTime = mAnimatorComponent->GetCurrentAnimationTime();
            break;
        }

        mAttackStyleCounter++;
        if (mAttackStyleCounter == 4)
        {
            mAttackStyleCounter = 1;
        }
    }
}

void PlayerControllerComponent::UpdateTimers(float deltaTime)
{
    if (mIsAttacking)
    {
        mCurrentAttackTimer += deltaTime;
        if (mCurrentAttackTimer >= mAttackTime)
        {
            mIsAttacking = false;
            mCurrentAttackTimer = 0.0f;
        }
    }
}
