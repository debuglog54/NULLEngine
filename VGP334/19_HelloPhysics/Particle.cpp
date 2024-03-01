#pragma once
#include "Particle.h"

void Particle::Initialize(const ParticleInfo& info)
{
	mLifeTime = info.lifeTime;
	mLifeDuration = info.lifeTime;
	mStartColor = info.startColor;
	mEndColor = info.endColor;

	MeshPC mesh = MeshBuilder::CreateSpherePC(10.0f, 10.0f, 0.1f);
	for (auto& v : mesh.vertices)
	{
		v.color = mStartColor;
	}

	mParticle.meshBuffer.Initialize(mesh);
	mParticle.transform.position = info.startPostion;

	mSphere.Initialize(0.1f);
	mRigidBody.Initialize(mParticle.transform, mSphere, 0.1f);
}

void Particle::Update(float deltaTime)
{
	//Color particleCOlor = EMath::Lerp(mStartColor, mEndColor, EMath::Clamp(mLifeTime / mLifeDuration);
}

void Particle::Terminate()
{
	mSphere.Terminate();
	mRigidBody.Terminate();
	mParticle.Terminate();
}

void Particle::Render()
{

}
