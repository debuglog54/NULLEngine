#pragma once

#include "Component.h"

namespace NULLEngine
{
    class ColliderComponent final : public Component
    {
    public:
        SET_TYPE_ID(ComponentId::ColliderComponent);

        void Initialize() override;
        void Terminate() override;
    
        void SetSphereCollider(float radius);
        void SetBoxCollider(const EMath::Vector3& halfExtents);
        void SetHullCollider(const NULLEngine::EMath::Vector3& halfExtents, const NULLEngine::EMath::Vector3 origin);

    private:
        friend class RigidBodyComponent;
        Physics::CollisionShape* mCollisionShape = nullptr;
    };
}