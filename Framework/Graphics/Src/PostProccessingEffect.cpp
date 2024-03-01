#include "Precompiled.h"
#include "PostProccessingEffect.h"

#include "GraphicsSystem.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"


using namespace NULLEngine;
using namespace NULLEngine::Graphics;

void PostProcessingEffect::Initialize(const std::filesystem::path& filePath)
{
	mVertexShader.Initiliaze<VertexPX>(filePath);
	mPixelShader.Initiliaze(filePath);
	mPostProcessBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Point, Sampler::AdressMode::Wrap);
}

void PostProcessingEffect::Terminate()
{
	mSampler.Terminate();
	mPostProcessBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void PostProcessingEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	PostProcessData data;
	data.mode = mMode;

	switch (mMode)
	{
	case PostProcessingEffect::Mode::None:
	{
		break;
	}
	case PostProcessingEffect::Mode::Monochrome:
	{
		break;
	}
	case PostProcessingEffect::Mode::Ivert:
	{
		break;
	}
	case PostProcessingEffect::Mode::Mirror:
	{
		data.param0 = mMirrorScale;
		break;
	}
	case PostProcessingEffect::Mode::Blur:
	{
		auto gs = GraphicsSystem::Get();
		const auto screenWidth = gs->GetBackBufferWidth();
		const auto screenHeight = gs->GetBackBufferHeight();
		data.param0 = 5.0f / screenWidth;
		data.param1 = 5.0f / screenHeight;
		break;
	}
	case PostProcessingEffect::Mode::Combine2:
	{
		break;
	}
	default:
		break;
	}

	mPostProcessBuffer.Update(data);
	mPostProcessBuffer.BindPS(0);

	mSampler.BindPS(0);

	for (int i = 0; i < mTextures.size(); ++i)
	{
		if (mTextures[i] != nullptr)
		{
			mTextures[i]->BindPS(i);
		}
	}
}

void PostProcessingEffect::End()
{
	for (int i = 0; i < mTextures.size(); ++i)
	{
		Texture::UnbindPS(i);
	}
}

void PostProcessingEffect::Render(const RenderObject& renderObject)
{
	renderObject.meshBuffer.Render();
}

void PostProcessingEffect::SetTexture(const Texture* texture, uint32_t slot)
{
	ASSERT(slot < mTextures.size(), "PostPrecessingEffect -- invalid slot index.");
	mTextures[slot] = texture;
}

void PostProcessingEffect::SetMode(Mode mode)
{
	mMode = mode;
}

void PostProcessingEffect::SetMirrorScale(float mirrorScale)
{
	mMirrorScale = mirrorScale;
}

void PostProcessingEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("PostProcessingEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int currentMode = static_cast<int>(mMode);
		const char* modeNames[] =
		{
			"None",
			"Monochrome",
			"Invert",
			"Mirror",
			"Blur",
			"Combine2"
		};

		if (ImGui::Combo("Mode", &currentMode, modeNames, static_cast<int>(std::size(modeNames))))
		{
			mMode = static_cast<Mode>(currentMode);
		}

		if (mMode == Mode::Mirror)
		{
			ImGui::DragFloat("Scale", &mMirrorScale, 0.01f, -5.0f, 5.0f);
		}
	}
}