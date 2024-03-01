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

	class ToonEffect
	{
	public:
		void Initialize(const std::filesystem::path& filePath);
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void DebugUI();

		void SetCamera(const Camera& camera);
		void SetDirectionalLight(const DirectionalLight& directionalLight);
		void SetLightCamera(const Camera& camera);
		void SetShadowMap(const Texture* shadowMap);

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

		struct SettingsData
		{
			Color color1 = { 1.0, 0.31f, 0.31f, 1.0f };
			Color color2 = {0.324f, 0.051f, 0.075f, 1.0f};
			Color borderColor = { 0.7f, 0.7f, 0.7f, 1.0f };
			float blendingCoeficient = 0.35f;
			int useIntensity = 1;
			int useDiffuseAsSecondColor = 0;
			float padding[1] = { 0.0f };
		};

		using LightBuffer = TypedConstantBuffer<DirectionalLight>;
		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using SettingsBuffer = TypedConstantBuffer<SettingsData>;
		using MaterialBuffer = TypedConstantBuffer<Material>;

		TransformBuffer mTransformBuffer;
		LightBuffer mLightBuffer;
		//MaterialBuffer mMaterialBuffer;
		SettingsBuffer mSettingsBuffer;
		Sampler mSampler;

		SettingsData mSettingsData;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		
		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Texture* mShadowMap = nullptr;
	};
}