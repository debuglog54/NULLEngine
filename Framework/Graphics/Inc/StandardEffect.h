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

	class StandardEffect
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
		float mDepthBias = 0.0f;

		struct TransformData
		{
			NULLEngine::EMath::Matrix4 world;
			NULLEngine::EMath::Matrix4 wvp[2];
			NULLEngine::EMath::Vector3 viewPosition;
			float displacementWeight;
		};

		struct BoneTransfomrData
		{
			static constexpr size_t MaxBouneCount = 256; // Can not be higher then in shader
			EMath::Matrix4 boneTransforms[MaxBouneCount];
		};

		struct SettingsData
		{
			int useDiffuseMap = 1;
			int useSpecularMap = 1;
			int useDisplacementMap = 1;
			int useNormalMap = 1;
			int useShadowMap = 1;
			int useSkinning = 1;
			float depthBias = 0.f;
			float padding[1] = { 0.0f };
		};

		using BoneTransformBuffer = ConstantBuffer;
		using LightBuffer = TypedConstantBuffer<DirectionalLight>;
		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using SettingsBuffer = TypedConstantBuffer<SettingsData>;
		using MaterialBuffer = TypedConstantBuffer<Material>;

		TransformBuffer mTransformBuffer;
		BoneTransformBuffer mBoneTransformBuffer;
		LightBuffer mLightBuffer;
		MaterialBuffer mMaterialBuffer;
		SettingsBuffer mSettingsBuffer;
		Sampler mSampler;

		BoneTransfomrData mBoneTransformData;
		SettingsData mSettingsData;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Texture* mShadowMap = nullptr;

	};
}