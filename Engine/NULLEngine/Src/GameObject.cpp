#include "Precompiled.h"
#include "GameObject.h"

using namespace NULLEngine;

static uint32_t gUinqueId = 0;

void GameObject::Initialize()
{
    ASSERT(!mInitialized, "GameObject: is already initialized!");
    mInitialized = true;

    for (auto& component : mComponents)
    {
        component->Initialize();
    }

    mUniqueId = ++gUinqueId;
}

void GameObject::Terminate()
{
    for (auto& component : mComponents)
    {
        component->Terminate();
    }
}

void GameObject::Update(float deltaTime)
{
    for (auto& component : mComponents)
    {
        component->Update(deltaTime);
    }
}

void GameObject::DebugUI()
{
    for (auto& component : mComponents)
    {
        component->DebugUI();
    }
}

void GameObject::EditorUI()
{
    std::string gameObjectName = std::string("GameObject##" + std::to_string(mUniqueId));
    if (ImGui::CollapsingHeader(gameObjectName.c_str(), ImGuiTreeNodeFlags_None))
    {
        char buffer[256];
        strcpy_s(buffer, mName.c_str());
        std::string objName = std::string("Name##" + std::to_string(mUniqueId));
        if (ImGui::InputText(objName.c_str(), buffer, sizeof(buffer)))
        {
            mName = std::move(buffer);
        }

        for (auto& component : mComponents)
        {
            component->EditorUI();
        }
    }
}

void GameObject::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
    rapidjson::Value gameObjectValue(rapidjson::kObjectType);
    for (auto& component : mComponents)
    {
        component->Serialize(doc, gameObjectValue);
    }

    rapidjson::Value name(mName.c_str(), mName.size(), doc.GetAllocator());
    value.AddMember(name, gameObjectValue, doc.GetAllocator());
}