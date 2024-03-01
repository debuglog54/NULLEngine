#pragma once

#include "Service.h"

namespace NULLEngine
{
	class CameraService;
	class TransformComponent;
	class ModelComponent;
	class MeshComponent;

	class RenderService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Render);

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime) override;
		void Render() override;
		void DebugUI() override;

		void SetDirectionalLight(const EMath::Vector3 direction);
	private:
		friend class ModelComponent;
		void Register(const ModelComponent* modelComponent);
		void Unregister(const ModelComponent* modelComponent);
		friend class MeshComponent;
		void Register(const MeshComponent* meshComponent);
		void Unregister(const MeshComponent* meshComponent);

		struct Entry
		{
			const ModelComponent* modelComponent = nullptr;
			const MeshComponent* meshComponent = nullptr;
			const TransformComponent* transformComponent = nullptr;
			Graphics::RenderGroup renderGroup;
		};

		using RenderEntities = std::vector<Entry>;
		RenderEntities mRenderEntities;

		const CameraService* mCameraService = nullptr;

		Graphics::DirectionalLight mDirectionalLight;
		Graphics::StandardEffect mStandardEffect;
		Graphics::ShadowEffect mShadowEffect;

		float mFPS = 0.0f;
	};
}