#include "Precompiled.h"
#include "AnimatorComponent.h"

#include "ModelComponent.h"
#include "GameObject.h"

void AnimatorComponent::Initialize()
{
    auto modelComponent = GetOwner().GetComponent<ModelComponent>();
    mAnimator.Initialize(modelComponent->GetModelId());
    mAnimator.PlayAnimation(0, true);
}

void AnimatorComponent::Update(float deltaTime)
{
    mAnimator.Update(deltaTime);
}

void AnimatorComponent::DebugUI()
{
}

bool AnimatorComponent::PlayAnimation(int index, bool looping)
{
    if (mAnimator.GetCurrentAnimationIndex() != index)
    {
        mAnimator.PlayAnimation(index, looping);
    }
    return true;
}

float AnimatorComponent::GetCurrentAnimationTime()
{
    return mAnimator.CurrentAnimationTime();
}