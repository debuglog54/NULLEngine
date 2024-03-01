#pragma once
#include <NULLEngine/Inc/Component.h>
#include "TypeIds.h"
#include "NULLEngine/Inc/TransformComponent.h"

class CoinComponent final : public NULLEngine::Component
{
public:
    SET_TYPE_ID(CustomComponentId::CoinComponent);

    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void DebugUI() override;
    void PickUp();
    Vector3 GetPosition();
private:
    float mCurrentAngle = 0;

    TransformComponent* mTransformComponent;
};
