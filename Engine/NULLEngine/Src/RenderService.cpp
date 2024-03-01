#include "Precompiled.h"
#include "RenderService.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "MeshComponent.h"
#include "AnimatorComponent.h"
#include"CameraService.h"
#include "GameWorld.h"

using namespace NULLEngine;
using namespace NULLEngine::Graphics;
using namespace NULLEngine::EMath;

void RenderService::Initialize()
{
    mCameraService = GetWorld().GetService<CameraService>();

    mDirectionalLight.ambient = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
    mDirectionalLight.specular = { 0.7f, 0.7f, 0.7f, 1.0f };

    mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    mShadowEffect.Initialize();
    mShadowEffect.SetDirectionalLight(mDirectionalLight);
}

void RenderService::Terminate()
{
    mShadowEffect.Terminate();
    mStandardEffect.Terminate();
}

void RenderService::Update(float deltaTime)
{
    mFPS = 1.0f / deltaTime;
}

void RenderService::Render()
{
    const auto& camera = mCameraService->GetMain();
    mStandardEffect.SetCamera(camera);

    for (auto& entry : mRenderEntities)
    {
        for (auto& renderObject : entry.renderGroup)
        {
            renderObject.transform = *entry.transformComponent;
        }
    }

    mShadowEffect.Begin();
    for (auto& entry : mRenderEntities)
    {
        DrawRenderGroup(mShadowEffect, entry.renderGroup);
    }
    mShadowEffect.End();

    mStandardEffect.Begin();
    for (auto& entry : mRenderEntities)
    {
        DrawRenderGroup(mStandardEffect, entry.renderGroup);
    }
    mStandardEffect.End();
}

void RenderService::DebugUI()
{
    ImGui::Text("FPS: %f ", mFPS);
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("Direction##Light", &mDirectionalLight.direction.x, 0.01f, -1.0f, 1.0f))
        {
            mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
        }
        ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
        ImGui::ColorEdit4("Difuse##Light", &mDirectionalLight.diffuse.r);
        ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
    }

    mStandardEffect.DebugUI();
    mShadowEffect.DebugUI();
    SimpleDraw::Render(mCameraService->GetMain());

    ImGui::Separator();
}

void RenderService::Register(const ModelComponent* modelComponent)
{
    auto& entry = mRenderEntities.emplace_back();
    auto& gameObject = modelComponent->GetOwner();
    entry.modelComponent = modelComponent;
    entry.transformComponent = gameObject.GetComponent<TransformComponent>();

    const Animator* animator = nullptr;
    auto animatorComponent = gameObject.GetComponent<AnimatorComponent>();
    if (animatorComponent != nullptr)
    {
        animator = &animatorComponent->GetAnimator();
    }

    entry.renderGroup = CreateRenderGroup(modelComponent->GetModelId(), animator);
}

void RenderService::Unregister(const ModelComponent* modelComponent)
{
    auto iter = std::find_if(
        mRenderEntities.begin(),
        mRenderEntities.end(),
        [&](const Entry& entry)
        {
            return entry.modelComponent == modelComponent;
        });

    if (iter != mRenderEntities.end())
    {
        auto& entry = *iter;
        CleanupRenderGroup(entry.renderGroup);
        mRenderEntities.erase(iter);
    }
}

void RenderService::Register(const MeshComponent* meshComponent)
{
    auto& entry = mRenderEntities.emplace_back();
    auto& gameObject = meshComponent->GetOwner();
    entry.meshComponent = meshComponent;
    entry.transformComponent = gameObject.GetComponent<TransformComponent>();
    entry.renderGroup.push_back(meshComponent->GetRenderObject());
}

void RenderService::Unregister(const MeshComponent* meshComponent)
{
    auto iter = std::find_if(
        mRenderEntities.begin(),
        mRenderEntities.end(),
        [&](const Entry& entry)
        {
            return entry.meshComponent == meshComponent;
        });

    if (iter != mRenderEntities.end())
    {
        auto& entry = *iter;
        CleanupRenderGroup(entry.renderGroup);
        mRenderEntities.erase(iter);
    }
}

void  RenderService::SetDirectionalLight(const EMath::Vector3 direction)
{
    mDirectionalLight.direction = direction;
}