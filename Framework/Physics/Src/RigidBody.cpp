#include "Precompiled.h"
#include "RigidBody.h"
#include "Graphics/Inc/Transform.h"
#include "Physics/Inc/PhysicsWorld.h"
#include "Math/Inc/EMath.h"
#include "CollisionShape.h"

using namespace NULLEngine;
using namespace NULLEngine::Physics;

RigidBody::~RigidBody()
{
	ASSERT(mRigidBody == nullptr && mMotionState == nullptr, "RigidBody: terminate must be called first");
}

void RigidBody::Initialize(Graphics::Transform& graphicsTransform, const CollisionShape& shape, float mass)
{
	mGraphicsTransform = &graphicsTransform;
	mMass = mass;
	
	mMotionState = new btDefaultMotionState(ConvertToBtTransform(graphicsTransform));
	mRigidBody = new btRigidBody(mMass, mMotionState, shape.GetCollisionShape());
	mRigidBody->setCollisionFlags(mRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	PhysicsWorld::Get()->Register(this);
}

void RigidBody::Terminate()
{
	PhysicsWorld::Get()->Unregister(this);

	SafeDelete(mMotionState);
	SafeDelete(mRigidBody);
}

bool RigidBody::IsDynamic()
{
	return mMass > 0.0f;
}

void RigidBody::UpdateTransform()
{
	ApplyBtTransformToTransform(mRigidBody->getWorldTransform(), *mGraphicsTransform);
}

void RigidBody::SetCollisionFilter(int flags)
{
	mRigidBody->setCollisionFlags(flags);
}
void RigidBody::SetPosition(const NULLEngine::EMath::Vector3& position)
{
	mGraphicsTransform->position = position;
	mRigidBody->setWorldTransform(ConvertToBtTransform(*mGraphicsTransform));
}

void RigidBody::SetVelocity(const NULLEngine::EMath::Vector3& velocity)
{
	mRigidBody->activate(true);
	mRigidBody->setLinearVelocity(ConvertToBtVector3(velocity));
}

void RigidBody::ApplyForce(const NULLEngine::EMath::Vector3& force)
{
	mRigidBody->activate(true);
	mRigidBody->applyCentralImpulse(ConvertToBtVector3(force));
}

void  RigidBody::UpdateCustomRotation()
{
	mRigidBody->setWorldTransform(ConvertToBtTransform(*mGraphicsTransform));
}

const NULLEngine::EMath::Vector3& RigidBody::GetVelocity()
{
	const NULLEngine::EMath::Vector3 linearVelocity = ConvertToVector3(mRigidBody->getLinearVelocity());
	return linearVelocity;
}

void MyCollisionCallback(btPersistentManifold* manifold)
{
	int numContacts = manifold->getNumContacts();
	for (int i = 0; i < numContacts; i++)
	{
		btManifoldPoint& contactPoint = manifold->getContactPoint(i);
		const btCollisionObject* objectA = manifold->getBody0();
		const btCollisionObject* objectB = manifold->getBody1();
		
			//objectA  setLinearVelocity(ConvertToBtVector3(velocity));
		// Example collision handling based on object types
	/*	if (objectA->getUserIndex() == MY_OBJECT_TYPE && objectB->getUserIndex() == OTHER_OBJECT_TYPE)
		{
			 Collision between MY_OBJECT_TYPE and OTHER_OBJECT_TYPE occurred
			 Perform actions specific to this collision
		}*/
		// Handle other collision scenarios as needed
	}
}