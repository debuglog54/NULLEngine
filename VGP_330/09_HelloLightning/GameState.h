#pragma once
#include <NULLEngine/Inc/NULLEngine.h>

using namespace NULLEngine;
using namespace NULLEngine::Graphics;

class GameState : public AppState
{
public:
	virtual ~GameState() = default;
	virtual void Initialize() override;
	virtual void Terminate()override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void DebugUI() override;

protected:
	void ShowNormals();

	struct Vertex
	{
		NULLEngine::EMath::Vector3 position;
		NULLEngine::Color color;
	};

	Camera mCamera;
	DirectionalLight mDirectionalLight;

	RenderObject mEarthObject;
	Mesh mEarth;
	StandardEffect mStandardEffect;

	bool mShowNormals = false;
};