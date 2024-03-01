#include "Precompiled.h"
#include "ToonEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace NULLEngine;
using namespace NULLEngine::Graphics;

void ToonEffect::Initialize(const std::filesystem::path& filePath)
{
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	//mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();

	mVertexShader.Initiliaze<Vertex>(filePath);
	mPixelShader.Initiliaze(filePath);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AdressMode::Wrap);
}

void ToonEffect::Terminate()
{
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mSettingsBuffer.Terminate();
	//mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
}


void ToonEffect::Begin()
{
	ASSERT((mCamera != nullptr), "ToonEffect - no camera set!");
	ASSERT((mDirectionalLight != nullptr), "ToonEffect - no light set!");
	mVertexShader.Bind();
	mPixelShader.Bind();

	//Begin Render (Bind all variables)
	mTransformBuffer.BindVS(0);

	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	//mMaterialBuffer.BindVS(2);
	//mMaterialBuffer.BindPS(2);

	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mSampler.BindVS(0);
	mSampler.BindPS(0);
}

void ToonEffect::End()
{
	//if need clean up everything afterwards
	if (mShadowMap != nullptr)
	{
		Texture::UnbindPS(4);
	}
}

void ToonEffect::Render(const RenderObject& renderObject)
{
	//Update variables
	const auto& matWorld = renderObject.transform.GetMatrix4();
	const auto& matView = mCamera->GetViewMatrix();
	const auto& matProj = mCamera->GetProjectionMatrix();

	SettingsData settingsData;
	settingsData.blendingCoeficient = mSettingsData.blendingCoeficient;
	settingsData.color1 = mSettingsData.color1;
	settingsData.color2 = mSettingsData.color2;
	settingsData.borderColor = mSettingsData.borderColor;
	settingsData.useIntensity = mSettingsData.useIntensity > 0;
	settingsData.useDiffuseAsSecondColor = mSettingsData.useDiffuseAsSecondColor > 0;


	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp[0] = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = mCamera->GetPosition();
	transformData.displacementWeight = mDisplacementWeight;

	//if (settingsData.useShadowMap)
	//{
	//	const auto& matLightView = mLightCamera->GetViewMatrix();
	//	const auto& matLightProj = mLightCamera->GetProjectionMatrix();
	//	transformData.wvp[1] = Transpose(matWorld * matLightView * matLightProj);
	//	mShadowMap->BindPS(4);
	//}

	mTransformBuffer.Update(transformData);
	mLightBuffer.Update(*mDirectionalLight);
	//mMaterialBuffer.Update(renderObject.material);
	mSettingsBuffer.Update(settingsData);

	auto tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	tm->BindPS(renderObject.normalMapId, 1);
	tm->BindVS(renderObject.displacementMapId, 2);
	tm->BindPS(renderObject.specularMapId, 3);
	renderObject.meshBuffer.Render();
}

void ToonEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void ToonEffect::SetDirectionalLight(const DirectionalLight& dirLight)
{
	mDirectionalLight = &dirLight;
}

void ToonEffect::SetLightCamera(const Camera& camera)
{
	mLightCamera = &camera;
}

void ToonEffect::SetShadowMap(const Texture* shadowMap)
{
	mShadowMap = shadowMap;
}

void ToonEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("ToonEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Separator();
		ImGui::ColorEdit4("Color1", &mSettingsData.color1.r);
		ImGui::ColorEdit4("Color2", &mSettingsData.color2.r);
		ImGui::ColorEdit4("Border Collor", &mSettingsData.borderColor.r);
		ImGui::DragFloat("Blending Coeficient", &mSettingsData.blendingCoeficient, 0.001f, 0.0f, 1.0f);
	
		bool useIntensity = mSettingsData.useIntensity > 0;
		ImGui::Checkbox("Use Itensity", &useIntensity);
		mSettingsData.useIntensity = (useIntensity) ? 1 : 0;


		bool useDiffuse = mSettingsData.useDiffuseAsSecondColor > 0;
		ImGui::Checkbox("Use Diffuse map as Second Color", &useDiffuse);
		mSettingsData.useDiffuseAsSecondColor = (useDiffuse) ? 1 : 0;
	}
}