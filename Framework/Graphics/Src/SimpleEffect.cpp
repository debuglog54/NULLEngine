#include "Precompiled.h"
#include "SimpleEffect.h"

#include "Camera.h"
#include "MeshBuffer.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace NULLEngine;
using namespace NULLEngine::Graphics;

void SimpleEffect::Initialize()
{
	std::filesystem::path filePath = L"../../Assets/Shaders/DoTransform.fx";
	mVertexShader.Initiliaze<VertexPC>(filePath);
	mPixelShader.Initiliaze(filePath);
	mConstantBuffer.Initialize(sizeof(EMath::Matrix4));
	mBlendState.Initialize(BlendState::Mode::AlphaBlend);
}

void SimpleEffect::Terminate()
{
	mBlendState.Terminate();
	mConstantBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void SimpleEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mConstantBuffer.BindVS(0);
	mBlendState.Set();
}

void SimpleEffect::End()
{
	mBlendState.ClearState();
}

void SimpleEffect::Render(const RenderObject& renderObject)
{
	EMath::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	EMath::Matrix4 matView = mCamera->GetViewMatrix();
	EMath::Matrix4 matProj = mCamera->GetProjectionMatrix();
	EMath::Matrix4 matFinal = matWorld * matView * matProj;
	EMath::Matrix4 wvp = Transpose(matFinal);

	mConstantBuffer.Update(&wvp);
	
	renderObject.meshBuffer.Render();
}

void SimpleEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}
