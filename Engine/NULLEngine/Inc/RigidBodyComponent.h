#pragma once
#include "Component.h"

namespace NULLEngine
{
    class RigidBodyComponent final : public Component
    {
    public:
        SET_TYPE_ID(ComponentId::RigidBodyComponent);

        void Initialize() override;
        void Terminate() override;

        void SetMass(float mass);
        void SetPosition(const NULLEngine::EMath::Vector3& position);
        void ApplyForce(Vector3& force);
        void SetVelocity(Vector3& velocity);
        const Vector3& GetVelocity();
        void UpdateCustomRotation();

    private:
        Physics::RigidBody mRigidBody;
        float mMass = 0.0f;
    };
}