#pragma once

#include <NULLEngine/Inc/NULLEngine.h>

struct ParticleInfo
{
	float lifetime = 0.0f;
	NULLEngine::Color startColor = NULLEngine::Colors::White;
	NULLEngine::Color endColor = NULLEngine::Colors::White;
	NULLEngine::EMath::Vector3 startScale = NULLEngine::EMath::Vector3::One();
	NULLEngine::EMath::Vector3 endScale = NULLEngine::EMath::Vector3::One();
};

class Particle
{
public:
	Particle() = default;

	void Initialize(const ParticleInfo& info);
	void Terminate();

	void Activate(const NULLEngine::EMath::Vector3& pos, const NULLEngine::EMath::Vector3& dir, float speed);
	void Update(float deltaTime);
	void Render(NULLEngine::Graphics::SimpleEffect& effect);
	void UpdateParameters(const ParticleInfo& info);

private:
	NULLEngine::Graphics::MeshPC mMesh;
	NULLEngine::Graphics::RenderObject mParticle;
	NULLEngine::Physics::RigidBody mRigidBody;
	NULLEngine::Physics::CollisionShapeSphere mSphere;

	bool mIsActive = false;
	ParticleInfo mInfo;
	float mLifeTime = 0.0f;
	float mLifeTimeDuration = 0.0f;
	NULLEngine::Color mCurrentColor = NULLEngine::Colors::White;
	NULLEngine::EMath::Vector3 mCurrentScale = NULLEngine::EMath::Vector3::One();
};
