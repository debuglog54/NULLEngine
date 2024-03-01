#include "Precompiled.h"
#include "PhysicsWorld.h"
#include "PhysicsDebugDrawer.h"
#include "Rigidbody.h"

using namespace NULLEngine;
using namespace NULLEngine::Physics;

namespace
{
    std::unique_ptr<PhysicsWorld> sPhysicsWorld;
}

void PhysicsWorld::StaticInitialize(const Settings& settings)
{
    ASSERT(sPhysicsWorld == nullptr, "PhysicsWorld: is already initialized");
    sPhysicsWorld = std::make_unique<PhysicsWorld>();
    sPhysicsWorld->Initialize(settings);
}

PhysicsWorld* PhysicsWorld::Get()
{
    ASSERT(sPhysicsWorld != nullptr, "ERROR: PhysicsWorld: is NOT already initialized");
    return sPhysicsWorld.get();
}

void PhysicsWorld::StaticTerminate()
{
    if (sPhysicsWorld != nullptr)
    {
        sPhysicsWorld->Terminate();
        sPhysicsWorld.reset();
    }
}

PhysicsWorld::~PhysicsWorld()
{
    ASSERT(sPhysicsWorld == nullptr, "ERROR: Terminate must be called before deletion");
}

void PhysicsWorld::Initialize(const Settings& settings)
{
    mSettings = settings;
    mCollisionConfiguration = new btDefaultCollisionConfiguration();
    mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
    mInterface = new btDbvtBroadphase();
    mSolver = new btSequentialImpulseConstraintSolver();
    mDynamicWorld = new btDiscreteDynamicsWorld(mDispatcher, mInterface, mSolver, mCollisionConfiguration);
    mDynamicWorld->setGravity(ConvertToBtVector3(settings.gravity));

    mDebugDrawer = new PhysicsDebugDrawer();
    mDynamicWorld->setDebugDrawer(mDebugDrawer);
}

void PhysicsWorld::Terminate()
{
    SafeDelete(mDebugDrawer);
    SafeDelete(mDynamicWorld);
    SafeDelete(mSolver);
    SafeDelete(mInterface);
    SafeDelete(mDispatcher);
    SafeDelete(mCollisionConfiguration);
}

void PhysicsWorld::Update(float deltaTime)
{
    mDynamicWorld->stepSimulation(deltaTime, mSettings.simulationSteps, mSettings.fixedTimeStep);
    for (auto& rb : mRigidBodies)
    {
        rb->UpdateTransform();
    }
}

void PhysicsWorld::Register(RigidBody* rigidBody)
{
    if (rigidBody != nullptr)
    {
        mRigidBodies.push_back(rigidBody);
        if (rigidBody->GetRigidBody() != nullptr)
        {
            mDynamicWorld->addRigidBody(rigidBody->GetRigidBody());
        }
    }
}

void PhysicsWorld::Unregister(RigidBody* rigidBody)
{
    if (rigidBody != nullptr)
    {
        auto iter = std::find(mRigidBodies.begin(), mRigidBodies.end(), rigidBody);
        if (iter != mRigidBodies.end())
        {
            if (rigidBody->GetRigidBody() != nullptr)
            {
                mDynamicWorld->removeRigidBody(rigidBody->GetRigidBody());
            }
            mRigidBodies.erase(iter);
        }
    }
}

void PhysicsWorld::DebugUI()
{
    ImGui::Checkbox("RenderPhysics##", &mRenderDebugUI);
    if (mRenderDebugUI)
    {
        int debugMode = mDebugDrawer->getDebugMode();
        bool isEnabled = (debugMode & btIDebugDraw::DBG_DrawWireframe) > 0;
        if (ImGui::Checkbox("[DBG] DrawWireframe##", &isEnabled))
        {
            debugMode = (isEnabled) ? debugMode | btIDebugDraw::DBG_DrawWireframe : debugMode & ~btIDebugDraw::DBG_DrawWireframe;
        }

        isEnabled = (debugMode & btIDebugDraw::DBG_DrawAabb) > 0;
        if (ImGui::Checkbox("[DBG] DrawAabbe##", &isEnabled))
        {
            debugMode = (isEnabled) ? debugMode | btIDebugDraw::DBG_DrawAabb : debugMode & ~btIDebugDraw::DBG_DrawAabb;
        }

        mDebugDrawer->setDebugMode(debugMode);
        mDynamicWorld->debugDrawWorld();

        mDynamicWorld->debugDrawWorld();
    }
}

void PhysicsWorld::SetGravity(const NULLEngine::EMath::Vector3& gravity)
{
    mDynamicWorld->setGravity(ConvertToBtVector3(gravity));
}