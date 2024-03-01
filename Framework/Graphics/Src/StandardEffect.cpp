#include "Precompiled.h"
#include "StandardEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"
#include "AnimationUtil.h"

using namespace NULLEngine;
using namespace NULLEngine::Graphics;

static constexpr size_t MaxBoneCount = 256;

void StandardEffect::Initialize(const std::filesystem::path& filePath)
{
	mTransformBuffer.Initialize();
	mBoneTransformBuffer.Initialize(MaxBoneCount * sizeof(EMath::Matrix4));
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();

	mVertexShader.Initiliaze<Vertex>(filePath);
	mPixelShader.Initiliaze(filePath);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AdressMode::Wrap);
}

void StandardEffect::Terminate()
{
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mBoneTransformBuffer.Terminate();
	mTransformBuffer.Terminate();
}


void StandardEffect::Begin()
{
	ASSERT((mCamera != nullptr), "StandardEffect - no camera set!");
	ASSERT((mDirectionalLight != nullptr), "StandardEffect - no light set!");
	mVertexShader.Bind();
	mPixelShader.Bind();

	//Begin Render (Bind all variables)
	mTransformBuffer.BindVS(0);

	mBoneTransformBuffer.BindVS(1);

	mLightBuffer.BindVS(2);
	mLightBuffer.BindPS(2);

	mMaterialBuffer.BindVS(3);
	mMaterialBuffer.BindPS(3);

	mSettingsBuffer.BindVS(4);
	mSettingsBuffer.BindPS(4);

	mSampler.BindVS(0);
	mSampler.BindPS(0);
}

void StandardEffect::End()
{
	//if need clean up everything afterwards
	if (mShadowMap != nullptr)
	{
		Texture::UnbindPS(4);
	}
}

void StandardEffect::Render(const RenderObject& renderObject)
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
	settingsData.useSkinning = mSettingsData.useSkinning > 0 && renderObject.skeleton != nullptr;
	settingsData.depthBias = mDepthBias;

	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp[0] = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = mCamera->GetPosition();
	transformData.displacementWeight = mDisplacementWeight;

	if (settingsData.useShadowMap)
	{
		const auto& matLightView = mLightCamera->GetViewMatrix();
		const auto& matLightProj = mLightCamera->GetProjectionMatrix();
		transformData.wvp[1] = Transpose(matWorld * matLightView * matLightProj);
		mShadowMap->BindPS(4);
	}

	if (settingsData.useSkinning)
	{
		AnimationUtil::BoneTransforms boneTransforms;
		AnimationUtil::ComputeBoneTransform(renderObject.modelId, boneTransforms, renderObject.animator);
		AnimationUtil::ApplyBoneOffsets(renderObject.modelId, boneTransforms);

		for (auto& transform : boneTransforms)
		{
			transform = EMath::Transpose(transform);
		}

		boneTransforms.resize(BoneTransfomrData::MaxBouneCount);
		mBoneTransformBuffer.Update(boneTransforms.data());
	}

	mTransformBuffer.Update(transformData);
	mLightBuffer.Update(*mDirectionalLight);
	mMaterialBuffer.Update(renderObject.material);
	mSettingsBuffer.Update(settingsData);

	auto tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	tm->BindPS(renderObject.normalMapId, 1);
	tm->BindVS(renderObject.displacementMapId, 2);
	tm->BindPS(renderObject.specularMapId, 3);
	renderObject.meshBuffer.Render();
}

void StandardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void StandardEffect::SetDirectionalLight(const DirectionalLight& dirLight)
{
	mDirectionalLight = &dirLight;
}

void StandardEffect::SetLightCamera(const Camera& camera)
{
	mLightCamera = &camera;
}

void StandardEffect::SetShadowMap(const Texture* shadowMap)
{
	mShadowMap = shadowMap;
}

void StandardEffect::DebugUI()
{
	//ImGui::Begin("Debug Contorls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
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

		ImGui::DragFloat("Depth Bias", &mDepthBias, 0.0000001, 0.0f, 1.0f, "%.6f");
	}

	//ImGui::End();
}