#pragma once
#include <NULLEngine/Inc/NULLEngine.h>
#include "ParticleSystem.h"

using namespace NULLEngine;
using namespace NULLEngine::Graphics;
using namespace NULLEngine::Physics;

class GameState : public NULLEngine::AppState
{
public:
    virtual ~GameState() = default;

    virtual void Initialize() override;
    virtual void Terminate() override;
    
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual void DebugUI() override;

protected:
    Camera mCamera;
    DirectionalLight mDirectionalLight;
       ParticleSystemInfo info;

    RenderObject mBall;
    RigidBody mBallRigidBody;
    CollisionShapeSphere mSphereCollisionShape;

    RenderObject mGround;
    RigidBody mGroundRigidBody;
    CollisionShapeBox mGroundCollisionShape;

    ParticleSystem mParticleSystem;

    StandardEffect mStandardEffect;
    SimpleEffect mParticleEffect;

private:
    void DefaultSetup();
    float radMax = 0.5;
    float radMin = 0.5;
};