#pragma once

namespace NULLEngine::Physics
{
    class	PhysicsDebugDrawer;
    class RigidBody;

    class PhysicsWorld final
    {
    public:
        struct Settings
        {
            NULLEngine::EMath::Vector3 gravity{ 0.0f,-9.81f,0.0f };
            uint32_t simulationSteps = 1;
            float fixedTimeStep = 1.0f / 60.0f;
        };

        static void StaticInitialize(const Settings& settings);
        static void StaticTerminate();
        static PhysicsWorld* Get();

        PhysicsWorld() = default;
        ~PhysicsWorld();

        void Initialize(const Settings& settings);
        void Terminate();

        void Update(float deltaTime);
        void DebugUI();

        void SetGravity(const NULLEngine::EMath::Vector3& gravity);
    private:
        friend class RigidBody;
        void Register(RigidBody* rigidBody);
        void Unregister(RigidBody* rigidBody);

        btBroadphaseInterface* mInterface = nullptr;
        btCollisionDispatcher* mDispatcher = nullptr;
        btDefaultCollisionConfiguration* mCollisionConfiguration = nullptr;
        btDiscreteDynamicsWorld* mDynamicWorld = nullptr;
        btSequentialImpulseConstraintSolver* mSolver = nullptr;
        PhysicsDebugDrawer* mDebugDrawer = nullptr;
        Settings mSettings;

        using RigidBodies = std::vector<RigidBody*>;
        RigidBodies mRigidBodies;

        bool mRenderDebugUI = false;
    };
}