#pragma once

#include "TypeIds.h"

namespace NULLEngine
{
    class GameObject;

    class Component
    {

    public:
        Component() = default;
        virtual ~Component() = default;

        Component(const Component&) = delete;
        Component(const Component&&) = delete;
        Component& operator=(const Component&) = delete;
        Component& operator=(const Component&&) = delete;

        virtual void Initialize() {}
        virtual void Terminate() {}
        virtual void Update(float deltaTime) {}
        virtual void DebugUI() {}
        virtual void EditorUI() {}
        virtual void Serialize(rapidjson::Document& doc, rapidjson::Value& value){}

        virtual uint32_t GetTypeId() const = 0;

        GameObject& GetOwner() { return *mOwner; }
        const GameObject& GetOwner() const { return *mOwner; }

    private:
        friend class GameObject;
        GameObject* mOwner = nullptr;
    };
}