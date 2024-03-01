#pragma once

namespace NULLEngine
{
    class GameObject;

    using CustomMake = std::function<bool(const char*, const rapidjson::Value&, GameObject&)>;

    namespace GameObjectFactory
    {
        void Make(const std::filesystem::path& templateFile, GameObject& gameObject);
        void SetCustomMake(CustomMake customMake);
    }
}