#pragma once
#include "Animator.h"
#include "Material.h"
#include "MeshBuffer.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Transform.h"
#include "Skeleton.h"


namespace NULLEngine::Graphics
{
	struct Model;
	class RenderObject
	{
	public:
		void Terminate();

		Transform transform;

		Material material;

		TextureId diffuseMapId;
		TextureId normalMapId;
		TextureId displacementMapId;
		TextureId specularMapId;

		ModelId modelId = 0;
		MeshBuffer meshBuffer;
		const Skeleton* skeleton = nullptr;
		const Animator* animator = nullptr;
	};

	using RenderGroup = std::vector<RenderObject>;
	[[nodiscard]] RenderGroup CreateRenderGroup(const Model& modelm, const Animator* animator = nullptr);
	[[nodiscard]] RenderGroup CreateRenderGroup(const ModelId& model, const Animator* animator = nullptr);

	void CleanupRenderGroup(RenderGroup& renderGroup);


	template<class Effect>
	void DrawRenderGroup(Effect& effect, const RenderGroup& renderGroup)
	{
		for (auto& renderObject : renderGroup)
		{
			effect.Render(renderObject);
		}
	}

}