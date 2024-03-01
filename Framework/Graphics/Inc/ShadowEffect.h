#pragma once
#include "Camera.h"
#include "ConstantBuffer.h"
#include "LightType.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "RenderTarget.h"

namespace NULLEngine::Graphics
{
	class RenderObject;

	class ShadowEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void SetDirectionalLight(const DirectionalLight& directionalLight);

		void SetFocus(const EMath::Vector3& focusPosition) { mFocusPosition = focusPosition; }
		void SetSize(float size) { mSize = size; }

		const Camera& GetLightCamera() const { return mLightCamera; }
		const Texture& GetDepthMap() const { return mDepthMapRenderTarget; }

		void DebugUI();

	private:
		void UpdateLightCamera();

		struct TransformData
		{
			EMath::Matrix4 wvp;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;

		TransformBuffer mTransformBuffer;
		VertexShader mVertexShader;
		PixelShader mPixelShader;

		const DirectionalLight* mDirectionalLight = nullptr;

		Camera mLightCamera;
		RenderTarget mDepthMapRenderTarget;

		EMath::Vector3 mFocusPosition = EMath::Vector3::Zero();
		float mSize=100.0f;
	};
}