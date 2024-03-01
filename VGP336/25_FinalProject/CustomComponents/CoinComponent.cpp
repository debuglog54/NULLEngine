#include"CoinComponent.h"
#include"GameManagerService.h"
#include "NULLEngine/Inc/GameObject.h"
#include "NULLEngine/Inc/GameWorld.h"

void CoinComponent::Initialize()
{
    mTransformComponent = GetOwner().GetComponent<TransformComponent>();

    auto& world = GetOwner().GetWorld();
    auto gameManager = world.GetService<GameManagerService>();
    gameManager->Register(this);
}

void CoinComponent::Terminate()
{

}

void CoinComponent::Update(float deltaTime)
{
    mCurrentAngle += deltaTime * 2.0f;
    if (mCurrentAngle >= 360)
    {
        mCurrentAngle = 0.0;
    }
    mTransformComponent->rotation = Quaternion::CreateFromAxisAngle(Vector3::YAxis(), mCurrentAngle);
}

void CoinComponent::DebugUI()
{

}

void CoinComponent::PickUp()
{
    auto& world = GetOwner().GetWorld();
    world.DestroyGameObject(GetOwner().GetHandle());
}

Vector3 CoinComponent::GetPosition()
{
    return mTransformComponent->position;
}