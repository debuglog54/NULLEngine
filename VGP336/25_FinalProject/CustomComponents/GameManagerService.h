#pragma once
#include "NULLEngine/Inc/Service.h"
#include "PlayerControllerComponent.h"
#include "CoinComponent.h"
#include "TypeIds.h"

class PlayerControllerComponent;

class GameManagerService final : public Service
{
public:
    SET_TYPE_ID(CustomServiceId::GameManagerService);
    void Initialize() override;
    void Update(float deltaTime) override;

    const PlayerControllerComponent& GetPlayer() const;

private:
    std::vector<CoinComponent*> mCoins;

    friend class PlayerControllerComponent;

    void Register(const PlayerControllerComponent* playerComponent);
    void Unregister(const PlayerControllerComponent* playerComponent);

    const PlayerControllerComponent* mPlayer = nullptr;
    const TransformComponent* mPlayerTransform = nullptr;


    friend class CoinComponent;
    void Register( CoinComponent* coin);
    void Unregister( CoinComponent* coin);
};
