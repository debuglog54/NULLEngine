#pragma once
#include <NULLEngine/Inc/NULLEngine.h>

struct ParticleInfo
{
	float lifeTime;
	float lifeDuration;
	NULLEngine::Color startColor = NULLEngine::Colors::White;
	NULLEngine::Color endColor = NULLEngine::Colors::White;
	NULLEngine::EMath::Vector3 startPostion= NULLEngine::EMath::Vector3::Zero();
	NULLEngine::EMath::Vector3 startVelocity = NULLEngine::EMath::Vector3::Zero();
};

class Particle
{
public:
	Particle() = default;
	void Initialize(const ParticleInfo& info);
	void Terminate();
	void Render();
	void Update(float deltaTime);


private:
	NULLEngine::Graphics::RenderObject  mParticle;
	NULLEngine::Physics::RigidBody  mRigidBody;
	NULLEngine::Physics::CollisionShapeSphere  mSphere;

	float mLifeTime = 0.0f;
	float mLifeDuration = 0.0f;
	NULLEngine::Color mStartColor = NULLEngine::Colors::White;
	NULLEngine::Color mEndColor = NULLEngine::Colors::White;
};