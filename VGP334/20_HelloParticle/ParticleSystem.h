#pragma once
#include "Particle.h"

struct ParticleSystemInfo
{
    ParticleInfo particleInfo;
    int minParticlesPerEmit = 0;
    int maxParticlesPerEmit = 0;
    float minTimeBetweenParticles = 0.0f;
    float maxTimeBetweenParticles = 0.0f;
    float minSpawnAngle = 0.0f;
    float maxSpawnAngle = 0.0f;
    float minParticleSpeed = 0.0f;
    float maxParticleSpeed = 0.0f;
    float lifeTime = 0.0f;
    int maxParticles = 100;
};

class ParticleSystem
{
public:
    ParticleSystem();
    ~ParticleSystem();

    void Initialize(const ParticleSystemInfo& info, const NULLEngine::EMath::Vector3& spawnPos, const NULLEngine::EMath::Vector3& spawnDir, float delay = 0.0f);
    void Terminate();
    void Update(float deltaTime);
    void Render(NULLEngine::Graphics::SimpleEffect& effect);
    void UpdateParameters(const ParticleSystemInfo& info, const Vector3& spawnPos, const Vector3& spawnDir, float delay);

private:
    void SpawnParticles();
    void SpawnParticle();

    using Particles = std::vector<Particle>;
    Particles mParticles;
    int mNextAvailableParticleIndex;
    float mLifeTime;
    float mNextSpawn;

    ParticleSystemInfo mInfo;
    NULLEngine::EMath::Vector3 mSpawnPosition;
    NULLEngine::EMath::Vector3 mSpawnDirection;
};