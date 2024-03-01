#include "Precompiled.h"
#include "RigidBodyComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

using namespace NULLEngine;

void RigidBodyComponent::Initialize()
{
    TransformComponent* transform = GetOwner().GetComponent<TransformComponent>();
    ColliderComponent* collider = GetOwner().GetComponent<ColliderComponent>();
    mRigidBody.Initialize(*transform, *collider->mCollisionShape, mMass);
}

void RigidBodyComponent::Terminate()
{
    mRigidBody.Terminate();
}

void RigidBodyComponent::SetPosition(const NULLEngine::EMath::Vector3& position)
{
    mRigidBody.SetPosition(position);
}


void RigidBodyComponent::SetMass(float mass)
{
    mMass = mass;
}

void RigidBodyComponent::ApplyForce(Vector3& force)
{
    mRigidBody.ApplyForce(force);
}

void RigidBodyComponent::SetVelocity(Vector3& velocity)
{
    mRigidBody.SetVelocity(velocity);
}

void RigidBodyComponent::UpdateCustomRotation()
{
    mRigidBody.UpdateCustomRotation();
}
const Vector3& RigidBodyComponent::GetVelocity()
{
   return  mRigidBody.GetVelocity();
}