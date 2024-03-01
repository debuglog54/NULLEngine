#pragma once

namespace NULLEngine::Grpahics
{
	struct Transform;
}

namespace NULLEngine::Physics
{
	class CollisionShape;

	class RigidBody final
	{
	public:
		RigidBody() = default;
		~RigidBody();

		void Initialize(Graphics::Transform& graphicsTransform, const CollisionShape& shape, float mass = 0.0f);
		void Terminate();

		void SetCollisionFilter(int flags);
		void SetPosition(const NULLEngine::EMath::Vector3& position);
		void SetVelocity(const NULLEngine::EMath::Vector3& velocity);
		void ApplyForce(const NULLEngine::EMath::Vector3& force);
		void UpdateCustomRotation();
		const NULLEngine::EMath::Vector3& GetVelocity();

		bool IsDynamic();
		void UpdateTransform();

	private:
		friend class PhysicsWorld;
		btRigidBody* GetRigidBody() { return mRigidBody; }

		btRigidBody* mRigidBody = nullptr;
		btDefaultMotionState* mMotionState = nullptr;
		float mMass = 0.0f;
		bool mIsDynamic = false;
		Graphics::Transform* mGraphicsTransform = nullptr;
	};
}
