#pragma once

#include "ConstantBuffer.h"
#include "LightType.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"
#include "Material.h"

namespace NULLEngine::Graphics
{
    class Camera;
    class RenderObject;
    class Texture;

    class ImprovedStandardEffect
    {
    public:
        void Initialize(const std::filesystem::path& filePath);
        void Terminate();

        void Begin();
        void End();

        void Render(const RenderObject& renderObject);

        void SetCamera(const Camera& camera);
        void SetDirectionalLight(const DirectionalLight& directionalLight);
        void SetLightCamera(const Camera& camera);
        void SetShadowMap(const Texture* shadowMap);


        void DebugUI();

    private:
        float mDisplacementWeight = 0.0f;
        float mDepthBias = 1.0f;

        struct TransformData
        {
            NULLEngine::EMath::Matrix4 world;
            NULLEngine::EMath::Matrix4 wvp[2];
            NULLEngine::EMath::Vector3 viewPosition;
            float displacementWeight;
        };

        struct SettingsData
        {
            int useDiffuseMap = 1;
            int useSpecularMap = 1;
            int useDisplacementMap = 1;
            int useNormalMap = 1;
            int useShadowMap = 1;
            float depthBias = 0.0f;
            float padding[2] = { 0.0f };
        };

        struct ExtraSettingsData
        {
            float brightness = 1.0f;
            float saturation = 0.0f;
            float contrast = 1;
            int useExtraSettings = 1;
        };

        using LightBuffer = TypedConstantBuffer<DirectionalLight>;
        using TransformBuffer = TypedConstantBuffer<TransformData>;
        using SettingsBuffer = TypedConstantBuffer<SettingsData>;
        using MaterialBuffer = TypedConstantBuffer<Material>;
        using ExtraSettingsBuffer = TypedConstantBuffer<ExtraSettingsData>;


        TransformBuffer mTransformBuffer;
        LightBuffer mLightBuffer;
        MaterialBuffer mMaterialBuffer;
        SettingsBuffer mSettingsBuffer;
        ExtraSettingsBuffer mExtraSettingsBuffer;
        Sampler mSampler;

        SettingsData mSettingsData;
        ExtraSettingsData mExtraSettings;

        VertexShader mVertexShader;
        PixelShader mPixelShader;

        const Camera* mCamera = nullptr;
        const Camera* mLightCamera = nullptr;
        const DirectionalLight* mDirectionalLight = nullptr;
        const Texture* mShadowMap = nullptr;

        void ResetExtraSettings();

    };
}