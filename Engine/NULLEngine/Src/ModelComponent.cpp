#include "Precompiled.h"
#include "ModelComponent.h"
#include "GameWorld.h"
#include "RenderService.h"

using namespace NULLEngine;

void ModelComponent::Initialize()
{
    auto mm = ModelManager::Get();
    mModelId = mm->GetModelId(mFileName);
    if (mm->GetModel(mModelId) == nullptr)
    {
        mm->LoadModel(mFileName);

        for (const auto& animationFileName : mAnimationsFileNames)
        {
            mm->AddAnimation(mModelId, animationFileName);
        }
    }

    auto renderService = GetOwner().GetWorld().GetService<RenderService>();
    renderService->Register(this);
}

void ModelComponent::Terminate()
{
    auto renderService = GetOwner().GetWorld().GetService<RenderService>();
    renderService->Unregister(this);
}