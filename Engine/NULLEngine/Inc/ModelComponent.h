
#pragma once

#include "Component.h"

namespace NULLEngine
{
    class ModelComponent final : public Component
    {
    public:
        SET_TYPE_ID(ComponentId::ModelComponent);

        void Initialize() override;
        void Terminate() override;

        void SetFileName(const char* fileName) { mFileName = fileName; }
        void AddAnimation(const char* animationFileName) 
        { 
            mAnimationsFileNames.push_back(animationFileName); 
        }

        Graphics::ModelId GetModelId() const { return mModelId; }
        const Graphics::Model& GetModel() const
        {
            return *Graphics::ModelManager::Get()->GetModel(mModelId);
        }

    private:
        std::string mFileName;
        Graphics::ModelId mModelId = 0;
        using Animations = std::vector<std::string>;
        Animations mAnimationsFileNames;

    };
}