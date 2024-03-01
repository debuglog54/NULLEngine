#pragma once

namespace NULLEngine::Physics
{
    class CollisionShape
    {
    public:
        CollisionShape() = default;
        virtual ~CollisionShape();

        virtual void Terminate();

    protected:
        btCollisionShape* mCollisionShape = nullptr;

    private:
        friend class RigidBody;
        btCollisionShape* GetCollisionShape() const { return mCollisionShape; }
    };


    class CollisionShapeSphere :public CollisionShape
    {
    public:
        CollisionShapeSphere() = default;
        void Initialize(float radius);
    };


    class CollisionShapeBox :public CollisionShape
    {
    public:
        CollisionShapeBox() = default;
        void Initialize(const NULLEngine::EMath::Vector3& halfExtents);
    };

    class CollisionShapeHull : public CollisionShape
    {
    public:
        CollisionShapeHull() = default;
        void Initialize(const NULLEngine::EMath::Vector3& halfExtents, const NULLEngine::EMath::Vector3 origin);
    };
}