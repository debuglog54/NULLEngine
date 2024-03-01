#include "GameManagerService.h"

void GameManagerService::Initialize()
{
    mCoins.clear();
}

void GameManagerService::Update(float deltaTime)
{
    std::vector<CoinComponent*> temp;

    for (auto& coin : mCoins)
    {
        float distance = EMath::Distance(coin->GetPosition(), mPlayerTransform->position);
        if (distance < 2)
        {
            temp.push_back(coin);
        }
    }

    for (auto& coin : temp)
    {
        Unregister(coin);
        coin->PickUp();
    }
}

const PlayerControllerComponent& GameManagerService::GetPlayer() const
{
    return *mPlayer;
}

void GameManagerService::Register(const PlayerControllerComponent* playerComponent)
{
    mPlayer = playerComponent;
    mPlayerTransform = mPlayer->GetOwner().GetComponent<TransformComponent>();
}

void GameManagerService::Unregister(const PlayerControllerComponent* playerComponent)
{
    if (mPlayer == playerComponent)
    {
        mPlayer == nullptr;
    }
}

void GameManagerService::Register( CoinComponent* coin)
{
    mCoins.push_back(coin);
}

void GameManagerService::Unregister( CoinComponent* coin)
{
	auto iter = std::find(mCoins.begin(), mCoins.end(), coin);
	if (iter != mCoins.end())
	{
		mCoins.erase(iter);
	}
}