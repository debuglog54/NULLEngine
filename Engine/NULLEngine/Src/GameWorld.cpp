#include "Precompiled.h"
#include  "GameWorld.h"
#include "GameObjectFactory.h"
#include "CameraService.h"
#include "RenderService.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"

using namespace NULLEngine::Physics;

void GameWorld::Initialize(uint32_t capacity)
{
    ASSERT(!mInitialized, "GameWorld: is already initialized");
    mInitialized = true;

    for (auto& service : mServices)
    {
        service->Initialize();
    }

    mGameObjectSlots.resize(capacity);
    mFreeSlots.resize(capacity);
    std::iota(mFreeSlots.begin(), mFreeSlots.end(), 0);
}

void GameWorld::Terminate()
{
    ASSERT(!mUpdating, "GameWorld: cant terminate while updating!");

    if (!mInitialized)
    {
        return;
    }

    for (auto& gameObject : mUpdateList)
    {
        DestroyGameObject(gameObject->GetHandle());
    }
    ProcessDestroyList();

    for (auto& service : mServices)
    {
        service->Terminate();
    }

    mInitialized = false;
}

void GameWorld::Update(float deltaTime)
{
    mUpdating = true;

    for (auto& gameObject : mUpdateList)
    {
        gameObject->Update(deltaTime);
    }

    for (auto& service : mServices)
    {
        service->Update(deltaTime);
    }

    mUpdating = false;

    ProcessDestroyList();
}

void GameWorld::Render()
{
    for (auto& service : mServices)
    {
        service->Render();
    }
}

void GameWorld::DebugUI()
{
    ImGui::Begin("Debug Contorls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    for (auto& service : mServices)
    {
        service->DebugUI();
    }

    PhysicsWorld::Get()->DebugUI();

    ImGui::End();
}
void GameWorld::EditorUI()
{
    ImGui::Begin("Editor ", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    for (auto& gameObject : mUpdateList)
    {
        gameObject->EditorUI();
    }

    if (ImGui::Button("Save"))
    {
        SaveLevel("../../Assets/Templates/Levels/TestLevelSave.json");
    }

    ImGui::End();
}

GameObject* GameWorld::CreateGameObject(const std::filesystem::path& templateFile)
{
    ASSERT(mInitialized, "GameWorld: is not initiliazed!");

    if (mFreeSlots.empty())
    {
        ASSERT(false, "GameWorld: no free slots available");
        return nullptr;
    }

    const uint32_t freeSlot = mFreeSlots.back();
    mFreeSlots.pop_back();

    auto& slot = mGameObjectSlots[freeSlot];
    auto& newObject = slot.gameObject;
    newObject = std::make_unique<GameObject>();

    GameObjectFactory::Make(templateFile, *newObject);
    newObject->mWorld = this;
    newObject->mHandle.mIndex = freeSlot;
    newObject->mHandle.mGeneration = slot.generation;
    newObject->Initialize();
    mUpdateList.push_back(newObject.get());
    return newObject.get();
}

GameObject* GameWorld::GetGameObject(const GameObjectHandle& handle)
{
    if (!isValid(handle))
    {
        return nullptr;
    }

    return mGameObjectSlots[handle.mIndex].gameObject.get();
}

void GameWorld::DestroyGameObject(const GameObjectHandle& handle)
{
    if (!isValid(handle))
    {
        return;
    }

    auto& slot = mGameObjectSlots[handle.mIndex];
    slot.generation++;
    mToBeDestroyed.push_back(handle.mIndex);
}

bool GameWorld::isValid(const GameObjectHandle& handle)
{
    if (handle.mIndex < 0 || handle.mIndex >= mGameObjectSlots.size())
    {
        return false;
    }

    if (mGameObjectSlots[handle.mIndex].generation != handle.mGeneration)
    {
        return false;
    }

    return true;
}

void GameWorld::ProcessDestroyList()
{
    ASSERT(!mUpdating, "GameWorld: cnat destroy when updating objects.");

    for (auto& index : mToBeDestroyed)
    {
        auto& slot = mGameObjectSlots[index];
        GameObject* gameObject = slot.gameObject.get();

        ASSERT(!isValid(gameObject->GetHandle()), "GameWorld: object is still alive");
        mUpdateList.erase(std::remove(mUpdateList.begin(), mUpdateList.end(), gameObject));
        gameObject->Terminate();
        slot.gameObject.reset();
        mFreeSlots.push_back(index);
    }

    mToBeDestroyed.clear();
}

void GameWorld::LoadLevel(const std::filesystem::path& levelFile)
{
    FILE* file = nullptr;
    auto err = fopen_s(&file, levelFile.u8string().c_str(), "r");
    ASSERT(err == 0 && file != nullptr, "GameWorld: failed to open level file %s", levelFile.u8string());

    char readBuffer[65536];
    rapidjson::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
    fclose(file);

    rapidjson::Document doc;
    doc.ParseStream(readStream);

    auto services = doc["Services"].GetObj();
    for (auto& service : services)
    {
        const char* serviceName = service.name.GetString();

        if (strcmp(serviceName, "CameraService") == 0)
        {
            auto cameraService = AddService<CameraService>();
        }
        else if (strcmp(serviceName, "RenderService") == 0)
        {
            auto renderService = AddService<RenderService>();

            if (service.value.HasMember("DirectionalLight"))
            {
                const auto& direction = service.value["DirectionalLight"].GetArray();
                const float x = direction[0].GetFloat();
                const float y = direction[1].GetFloat();
                const float z = direction[2].GetFloat();
                renderService->SetDirectionalLight({ x,y,z });
            }
        }
        else if (strcmp(serviceName, "GameManager") == 0)
        {
            auto renderService = AddService<RenderService>();

            if (service.value.HasMember("DirectionalLight"))
            {
                const auto& direction = service.value["DirectionalLight"].GetArray();
                const float x = direction[0].GetFloat();
                const float y = direction[1].GetFloat();
                const float z = direction[2].GetFloat();
                renderService->SetDirectionalLight({ x,y,z });
            }
        }
        else
        {
            ASSERT(false, "GameWorld: service %s is not defined", serviceName);
        }
    }

    uint32_t capacity = static_cast<uint32_t>(doc["Capacity"].GetInt());
    Initialize(capacity);

    if (doc.HasMember("Physics"))
    {
        const auto& physicsData = doc["Physics"].GetObj();
        if (physicsData.HasMember("Gravity"))
        {
            const auto& gravity = physicsData["Gravity"].GetArray();
            const auto& x = gravity[0].GetFloat();
            const auto& y = gravity[1].GetFloat();
            const auto& z = gravity[2].GetFloat();
            PhysicsWorld::Get()->SetGravity({ x,y,z });
        }
    }

    auto gameObjects = doc["GameObjects"].GetObj();
    for (auto& gameObject : gameObjects)
    {
        const char* templateFile = gameObject.value["Template"].GetString();
        auto obj = CreateGameObject(templateFile);
        if (obj != nullptr)
        {
            const char* name = gameObject.name.GetString();
            obj->SetName(name);
            if (gameObject.value.HasMember("Position"))
            {
                const auto& pos = gameObject.value["Position"].GetArray();

                const float x = pos[0].GetFloat();
                const float y = pos[1].GetFloat();
                const float z = pos[2].GetFloat();

                auto transform = obj->GetComponent<TransformComponent>();
                transform->position = { x,y,z };

                auto rigidBodyComponent = obj->GetComponent<RigidBodyComponent>();
                if (rigidBodyComponent != nullptr)
                {
                    rigidBodyComponent->SetPosition(transform->position);
                }
                // will need to update if rigid body exists
            }

            if (gameObject.value.HasMember("Rotation"))
            {
                const auto& rotation = gameObject.value["Rotation"].GetArray();
                const float rot_x = rotation[0].GetFloat();
                const float rot_y = rotation[1].GetFloat();
                const float rot_z = rotation[2].GetFloat();
                const float rot_w = rotation[3].GetFloat();

                auto transform = obj->GetComponent<TransformComponent>();

                auto rigidBodyComponent = obj->GetComponent<RigidBodyComponent>();
                transform->rotation = { rot_x,rot_y,rot_z,rot_w };
                if (rigidBodyComponent != nullptr)
                {
                    rigidBodyComponent->UpdateCustomRotation();
                }
            }

            if (gameObject.value.HasMember("Scale"))
            {
                const auto& scale = gameObject.value["Scale"].GetArray();
                const float scale_x = scale[0].GetFloat();
                const float scale_y = scale[1].GetFloat();
                const float scale_z = scale[2].GetFloat();

                auto transform = obj->GetComponent<TransformComponent>();
                transform->scale = { scale_x,scale_y,scale_z };
            }
        }
    }
}
void GameWorld::SaveLevel(const std::filesystem::path& levelFile)
{
    rapidjson::Document doc;
    rapidjson::Value gameObjects(rapidjson::kObjectType);
    for (auto& gameObject : mUpdateList)
    {
        gameObject->Serialize(doc, gameObjects);
    }

    doc.SetObject();
    doc.AddMember("GameObjects", gameObjects, doc.GetAllocator());

    FILE* file = nullptr;
    auto err = fopen_s(&file, levelFile.u8string().c_str(), "w");
    ASSERT(err == 0 && file != nullptr, "GameWorld: failed to open level file %s", levelFile.u8string());

    char writeBuffer[65536];
    rapidjson::FileWriteStream writeStream(file, writeBuffer, sizeof(writeBuffer));

    rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(writeStream);
    doc.Accept(writer);

    fclose(file);
}