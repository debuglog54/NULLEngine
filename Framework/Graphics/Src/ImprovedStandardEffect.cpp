#include "Precompiled.h"
#include "ImprovedStandardEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace NULLEngine;
using namespace NULLEngine::Graphics;

void ImprovedStandardEffect::ResetExtraSettings()
{
	mExtraSettings.useExtraSettings = 1;
	mExtraSettings.brightness = 1.0f;
	mExtraSettings.saturation = 0.0f;
	mExtraSettings.contrast = 1.0f;
}

void ImprovedStandardEffect::Initialize(const std::filesystem::path& filePath)
{
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mExtraSettingsBuffer.Initialize();

	mVertexShader.Initiliaze<Vertex>(filePath);
	mPixelShader.Initiliaze(filePath);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AdressMode::Wrap);
}

void ImprovedStandardEffect::Terminate()
{
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mExtraSettingsBuffer.Terminate();
}


void ImprovedStandardEffect::Begin()
{
	ASSERT((mCamera != nullptr), "ImprovedStandard.fx - no camera set!");
	ASSERT((mDirectionalLight != nullptr), "ImprovedStandard.fx - no light set!");
	mVertexShader.Bind();
	mPixelShader.Bind();

	//Begin Render (Bind all variables)
	mTransformBuffer.BindVS(0);

	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mExtraSettingsBuffer.BindVS(4);
	mExtraSettingsBuffer.BindPS(4);

	mSampler.BindVS(0);
	mSampler.BindPS(0);
}

void ImprovedStandardEffect::End()
{
	//if need clean up everything afterwards
	if (mShadowMap != nullptr)
	{
		Texture::UnbindPS(4);
	}
}

void ImprovedStandardEffect::Render(const RenderObject& renderObject)
{
	//Update variables
	const auto& matWorld = renderObject.transform.GetMatrix4();
	const auto& matView = mCamera->GetViewMatrix();
	const auto& matProj = mCamera->GetProjectionMatrix();

	SettingsData settingsData;
	settingsData.useDiffuseMap = mSettingsData.useDiffuseMap > 0 && renderObject.diffuseMapId > 0;
	settingsData.useNormalMap = mSettingsData.useNormalMap > 0 && renderObject.normalMapId > 0;
	settingsData.useSpecularMap = mSettingsData.useSpecularMap > 0 && renderObject.specularMapId > 0;
	settingsData.useDisplacementMap = mSettingsData.useDisplacementMap > 0 && renderObject.displacementMapId > 0;
	settingsData.useShadowMap = mSettingsData.useShadowMap > 0 && mShadowMap != nullptr;
	settingsData.depthBias = mDepthBias;

	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp[0] = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = mCamera->GetPosition();
	transformData.displacementWeight = mDisplacementWeight;
	
	ExtraSettingsData extraSettingsData;
	extraSettingsData.brightness = mExtraSettings.brightness;
	extraSettingsData.contrast = mExtraSettings.contrast;
	extraSettingsData.saturation = mExtraSettings.saturation;
	extraSettingsData.useExtraSettings = mExtraSettings.useExtraSettings > 0;

	if (settingsData.useShadowMap)
	{
		const auto& matLightView = mLightCamera->GetViewMatrix();
		const auto& matLightProj = mLightCamera->GetProjectionMatrix();
		transformData.wvp[1] = Transpose(matWorld * matLightView * matLightProj);
		mShadowMap->BindPS(4);
	}

	mTransformBuffer.Update(transformData);
	mLightBuffer.Update(*mDirectionalLight);
	mMaterialBuffer.Update(renderObject.material);
	mSettingsBuffer.Update(settingsData);
	mExtraSettingsBuffer.Update(extraSettingsData);

	auto tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	tm->BindPS(renderObject.normalMapId, 1);
	tm->BindVS(renderObject.displacementMapId, 2);
	tm->BindPS(renderObject.specularMapId, 3);
	renderObject.meshBuffer.Render();
}

void ImprovedStandardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void ImprovedStandardEffect::SetDirectionalLight(const DirectionalLight& dirLight)
{
	mDirectionalLight = &dirLight;
}

void ImprovedStandardEffect::SetLightCamera(const Camera& camera)
{
	mLightCamera = &camera;
}

void ImprovedStandardEffect::SetShadowMap(const Texture* shadowMap)
{
	mShadowMap = shadowMap;
}

void ImprovedStandardEffect::DebugUI()
{
	//ImGui::Begin("Debug Contorls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("ImprovedStandardEffect - Basic", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Separator();
		ImGui::DragFloat("Displacement Weight", &mDisplacementWeight, 0.01f);

		bool useDiffuseMap = mSettingsData.useDiffuseMap > 0;
		ImGui::Checkbox("Use Diffuese Map", &useDiffuseMap);
		mSettingsData.useDiffuseMap = (useDiffuseMap) ? 1 : 0;

		bool useNormalMap = mSettingsData.useNormalMap > 0;
		ImGui::Checkbox("Use Normal Map", &useNormalMap);
		mSettingsData.useNormalMap = (useNormalMap) ? 1 : 0;

		bool useSpecularMap = mSettingsData.useSpecularMap > 0;
		ImGui::Checkbox("Use Specular Map", &useSpecularMap);
		mSettingsData.useSpecularMap = (useSpecularMap) ? 1 : 0;

		bool useDisplacementMap = mSettingsData.useDisplacementMap > 0;
		ImGui::Checkbox("Use Displacement Map", &useDisplacementMap);
		mSettingsData.useDisplacementMap = (useDisplacementMap) ? 1 : 0;


		bool useShadowCamera = mSettingsData.useShadowMap > 0;
		ImGui::Checkbox("Use Shadow Map", &useShadowCamera);
		mSettingsData.useShadowMap = (useShadowCamera) ? 1 : 0;

		ImGui::DragFloat("Depth Bias", &mDepthBias, 0.001, 0.0f, 1.0f, "%.3f");
	}

	
	if (ImGui::CollapsingHeader("ImprovedStandardEffect - Extra", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Separator();
		bool useExtraSettings = mExtraSettings.useExtraSettings > 0;
		ImGui::Checkbox("Use Color Enhancement", &useExtraSettings);
		mExtraSettings.useExtraSettings = (useExtraSettings) ? 1 : 0;

		ImGui::Separator();
		ImGui::DragFloat("Brightnes:", &mExtraSettings.brightness, 0.005, -10.0f, 10.0f, "%.3f");
		ImGui::Separator();
		ImGui::DragFloat("Saturation:", &mExtraSettings.saturation, 0.005, -10.0f, 10.0f, "%.3f");
		ImGui::Separator();
		ImGui::DragFloat("Contrast:", &mExtraSettings.contrast, 0.005, -10.0f, 10.0f, "%.3f");

		if (ImGui::Button("Reset", ImVec2(100, 25)))
		{
			ResetExtraSettings();
		}
	}
	ImGui::Separator();
}
