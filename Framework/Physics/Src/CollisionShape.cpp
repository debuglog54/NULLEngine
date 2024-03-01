#include "Precompiled.h"
#include "CollisionShape.h"

using namespace NULLEngine;
using namespace NULLEngine::Physics;

CollisionShape::~CollisionShape()
{
    ASSERT(mCollisionShape == nullptr, "CollisionShape: terminate must be called first");
}

void CollisionShape::Terminate()
{
    SafeDelete(mCollisionShape);
}

void CollisionShapeSphere::Initialize(float radius)
{
    ASSERT(mCollisionShape == nullptr, "CollisionShapeSphere: collision shape has already been created");
    mCollisionShape = new btSphereShape(radius);
}

void CollisionShapeBox::Initialize(const NULLEngine::EMath::Vector3& halfExtents)
{
    ASSERT(mCollisionShape == nullptr, "CollisionShapeBox: collision shape has already been created");
    mCollisionShape = new btBoxShape(ConvertToBtVector3(halfExtents));
}

void CollisionShapeHull::Initialize(const NULLEngine::EMath::Vector3& halfExtents, const NULLEngine::EMath::Vector3 origin)
{
    ASSERT(mCollisionShape == nullptr, "CollisionShapeHull: collision shape has already been created");
    btConvexHullShape* hullShape = new btConvexHullShape();
    
    std::vector< NULLEngine::EMath::Vector3> points = {
        { -halfExtents.x, -halfExtents.y, -halfExtents.z },
        { -halfExtents.x,  halfExtents.y, -halfExtents.z },
        {  halfExtents.x,  halfExtents.y, -halfExtents.z },
        {  halfExtents.x, -halfExtents.y, -halfExtents.z },
        {  halfExtents.x, -halfExtents.y,  halfExtents.z },
        {  halfExtents.x, halfExtents.y,   halfExtents.z },
        { -halfExtents.x,  halfExtents.y,  halfExtents.z },
        { -halfExtents.x, -halfExtents.y,  halfExtents.z }
    };

    for (auto& point : points)
    {
        hullShape->addPoint(ConvertToBtVector3(point - origin), false);
    }

    hullShape->recalcLocalAabb();
    mCollisionShape = hullShape;
}
