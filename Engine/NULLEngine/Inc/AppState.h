#pragma once

namespace NULLEngine
{
    class AppState
    {
    public:
        virtual ~AppState() = default;
        virtual void Initialize(){}
        virtual void Terminate(){}

        virtual void Update(float deltaTime){}
        virtual void Render(){}
        virtual void DebugUI(){}
        virtual void EditorUI() {}

    };
}