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
	Camera mCamera;
	DirectionalLight mDirectionalLight;
	RenderObject mGround;
	RenderObject mSphere;
	StandardEffect mStandardEffect;
};