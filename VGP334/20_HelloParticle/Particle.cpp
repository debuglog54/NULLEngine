#include "Particle.h"

using namespace NULLEngine;
using namespace NULLEngine::Graphics;

void Particle::Initialize(const ParticleInfo& info)
{
	mIsActive = false;
	mInfo = info;
	mLifeTime = 0.0f;
	mLifeTimeDuration = 0.0f;

	mMesh = MeshBuilder::CreateSpherePC(8, 6, 0.5f);

	mParticle.meshBuffer.Initialize(nullptr, sizeof(VertexPC), mMesh.vertices.size(), mMesh.indices.data(), mMesh.indices.size());

	mSphere.Initialize(0.1f);
	mRigidBody.Initialize(mParticle.transform, mSphere, 0.1f);
	mRigidBody.SetCollisionFilter(0);
}

void Particle::Terminate()
{
	mRigidBody.Terminate();
	mSphere.Terminate();
	mParticle.Terminate();
}

void Particle::Activate(const NULLEngine::EMath::Vector3& pos, const NULLEngine::EMath::Vector3& dir, float speed)
{
	mLifeTime = 0.0f;
	mLifeTimeDuration = mInfo.lifetime;
	mIsActive = true;
	mRigidBody.SetPosition(pos);
	mRigidBody.SetVelocity(dir * speed);
}

void Particle::Update(float deltaTime)
{
	if (mIsActive)
	{
		mLifeTime += deltaTime;
		float t = EMath::Clamp(mLifeTime / mLifeTimeDuration, 0.0f, 1.0f);
		mCurrentColor = EMath::Lerp(mInfo.startColor, mInfo.endColor, t);
		mCurrentScale = EMath::Lerp(mInfo.startScale, mInfo.endScale, t);
		mIsActive = mLifeTime < mLifeTimeDuration;
	}
	else
	{
		mRigidBody.SetVelocity(Vector3::Zero());
	}
}

void Particle::Render(NULLEngine::Graphics::SimpleEffect& effect)
{
	if (mIsActive)
	{
		for (int i = 0; i < mMesh.vertices.size(); ++i)
		{
			mMesh.vertices[i].color = mCurrentColor;
		}
		mParticle.transform.scale = mCurrentScale;
		mParticle.meshBuffer.Update(mMesh.vertices.data(), mMesh.vertices.size());

		effect.Render(mParticle);
	}
}

void Particle::UpdateParameters(const ParticleInfo& info)
{
	mIsActive = false;
	mInfo = info;
	mLifeTime = 0.0f;
	mLifeTimeDuration = 0.0f;
}