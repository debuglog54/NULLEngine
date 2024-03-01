#include "Precompiled.h"
#include "ShadowEffect.h"

#include "RenderObject.h"
#include "VertexTypes.h"

using namespace NULLEngine;
using namespace NULLEngine::Graphics;

void ShadowEffect::Initialize()
{
	mLightCamera.SetMode(Camera::ProjectionMode::Orthographic);

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Shadow.fx";
	mVertexShader.Initiliaze<Vertex>(shaderFile);
	mPixelShader.Initiliaze(shaderFile);

	mTransformBuffer.Initialize();

	constexpr uint32_t depthMapResolution = 4096;
	mDepthMapRenderTarget.Initialize(depthMapResolution, depthMapResolution, Texture::Format::RGBA_U32);
}

void ShadowEffect::Terminate()
{
	mDepthMapRenderTarget.Terminate();

	mTransformBuffer.Terminate();
	mVertexShader.Terminate();
	mPixelShader.Terminate();
}

void ShadowEffect::Begin()
{
	UpdateLightCamera();

	mVertexShader.Bind();
	mPixelShader.Bind();
	mTransformBuffer.BindVS(0);

	mDepthMapRenderTarget.BeginRender();
}

void ShadowEffect::End()
{
	mDepthMapRenderTarget.EndRender();
}

void ShadowEffect::Render(const RenderObject& renderObject)
{
	const auto& matWorld = renderObject.transform.GetMatrix4();
	const auto& matView = mLightCamera.GetViewMatrix();
	const auto& matProj = mLightCamera.GetProjectionMatrix();

	TransformData transformData;
	transformData.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(transformData);

	renderObject.meshBuffer.Render();
}

void ShadowEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;
}

void ShadowEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("ShadowEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Depth Map");
		ImGui::Image(
			mDepthMapRenderTarget.GetRawData(),
			{ 144,144 },
			{ 0,0 },
			{ 1,1 },
			{ 1,1,1,1 },
			{ 1,1,1,1 }
		);
		ImGui::DragFloat("Size", &mSize, 1.0f, 1.0f, 1000.0f);
	}
}

void ShadowEffect::UpdateLightCamera()
{
	ASSERT((mDirectionalLight != nullptr), "ShadowEffect -- no light set!");

	const EMath::Vector3& direction = mDirectionalLight->direction;
	mLightCamera.SetDirection(direction);
	mLightCamera.SetPosition(mFocusPosition - (direction * 1000.0f));
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(1500.0f);
	mLightCamera.SetSize(mSize, mSize);
}


