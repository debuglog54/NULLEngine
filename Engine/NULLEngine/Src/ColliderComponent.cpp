#include "Precompiled.h"
#include "ColliderComponent.h"

using namespace NULLEngine;
using namespace NULLEngine::Physics;

void ColliderComponent::Initialize()
{
    ASSERT(mCollisionShape != nullptr, "CollsionShape: was not set.");
}

void ColliderComponent::Terminate()
{
    mCollisionShape->Terminate();
    delete mCollisionShape;
}

void ColliderComponent::SetSphereCollider(float radius)
{
    ASSERT(mCollisionShape == nullptr, "ColliderComponent: is already set!!!");
    CollisionShapeSphere* sphere = new CollisionShapeSphere();
    sphere->Initialize(radius);
    mCollisionShape = sphere;
}

void ColliderComponent::SetBoxCollider(const EMath::Vector3& halfExtents)
{
    ASSERT(mCollisionShape == nullptr, "ColliderComponent: is already set!!!");
    CollisionShapeBox* box = new CollisionShapeBox();
    box->Initialize(halfExtents);
    mCollisionShape = box;
}

void ColliderComponent::SetHullCollider(const NULLEngine::EMath::Vector3& halfExtents, const NULLEngine::EMath::Vector3 origin)
{
    ASSERT(mCollisionShape == nullptr, "ColliderComponent: is already set!!!");

    CollisionShapeHull* hull = new CollisionShapeHull();
    hull->Initialize(halfExtents, origin);
    mCollisionShape = hull;
}