#pragma once
#include <NULLEngine/Inc/NULLEngine.h>

using namespace NULLEngine;
using namespace NULLEngine::Graphics;

class GameState : public NULLEngine::AppState
{
public:
    virtual ~GameState() = default;

    virtual void Initialize() override;

    virtual void Update(float deltaTime) override;
};