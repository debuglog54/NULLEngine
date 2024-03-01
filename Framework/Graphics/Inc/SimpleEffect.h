#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "BlendState.h"

namespace NULLEngine::Graphics
{
	class Camera;
	class RenderObject;

	class SimpleEffect
	{
	public:
		void Initialize();
		void Terminate();
		void Begin();
		void End();
		void Render(const RenderObject& renderObject);
		void SetCamera(const Camera& camera);

	private:
		const Camera* mCamera = nullptr;

		ConstantBuffer mConstantBuffer;
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		BlendState mBlendState;
	};
}