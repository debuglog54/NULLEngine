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
	virtual void OnCreateShape();
	struct Vertex
	{
		NULLEngine::EMath::Vector3 position;
		NULLEngine::Color color;
	};

	std::vector<Vertex> mVertecies;

	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11VertexShader* mVertexShader = nullptr;
	ID3D11InputLayout* mInputLayout = nullptr;
	ID3D11PixelShader* mPixelShader = nullptr;
};

class TriForceSymbol : public GameState
{
public:
	virtual  ~TriForceSymbol() = default;

protected:
	void OnCreateShape() override
	{
		mVertecies.push_back({ EMath::Vector3(-1.0f,-1.0f,0.0f), Colors::Yellow });
		mVertecies.push_back({ EMath::Vector3(-0.5f,0.0f,0.0f), Colors::Red });
		mVertecies.push_back({ EMath::Vector3(0.0f,-1.0f,0.0f), Colors::Green });

		mVertecies.push_back({ EMath::Vector3(0.0f,-1.0f,0.0f), Colors::Yellow });
		mVertecies.push_back({ EMath::Vector3(0.5f,0.0,0.0f), Colors::Red });
		mVertecies.push_back({ EMath::Vector3(1.0f,-1.0f,0.0f), Colors::Green });

		mVertecies.push_back({ EMath::Vector3(-0.5f,0.0f,0.0f), Colors::Yellow });
		mVertecies.push_back({ EMath::Vector3(0.0f,1.0,0.0f), Colors::Red });
		mVertecies.push_back({ EMath::Vector3(0.5f,0.0f,0.0f), Colors::Green });
	}
};

class Diamond : public GameState
{
public:
	virtual  ~Diamond() = default;

protected:
	void OnCreateShape() override
	{
		mVertecies.push_back({ EMath::Vector3(0.0f,-1.0f,0.0f), Colors::Green });
		mVertecies.push_back({ EMath::Vector3(-0.5f,0.0f,0.0f), Colors::Green });
		mVertecies.push_back({ EMath::Vector3(0.0f,0.0f,0.0f), Colors::Yellow });

		mVertecies.push_back({ EMath::Vector3(0.0f,0.0f,0.0f), Colors::Yellow });
		mVertecies.push_back({ EMath::Vector3(0.5f,0.0f,0.0f), Colors::Green });
		mVertecies.push_back({ EMath::Vector3(0.0f,-1.0f,0.0f), Colors::Green });

		mVertecies.push_back({ EMath::Vector3(-0.5f,0.0f,0.0f), Colors::Green });
		mVertecies.push_back({ EMath::Vector3(-0.25f,0.5f,0.0f), Colors::Red });
		mVertecies.push_back({ EMath::Vector3(0.0f,0.0f,0.0f), Colors::Yellow });

		mVertecies.push_back({ EMath::Vector3(+0.0f,0.0f,0.0f), Colors::Yellow });
		mVertecies.push_back({ EMath::Vector3(+0.25f,0.5f,0.0f), Colors::Red });
		mVertecies.push_back({ EMath::Vector3(0.5f,0.0f,0.0f), Colors::Green });

		mVertecies.push_back({ EMath::Vector3(-0.25f,0.5f,0.0f), Colors::Red });
		mVertecies.push_back({ EMath::Vector3(+0.25f,0.5f,0.0f), Colors::Red });
		mVertecies.push_back({ EMath::Vector3(0.0f,0.0f,0.0f), Colors::Yellow });
	}
};

class Heart : public GameState
{
public:
	virtual  ~Heart() = default;

protected:
	void OnCreateShape() override
	{
		mVertecies.push_back({ EMath::Vector3(0.0f,-1.0f,0.0f), Colors::Red });
		mVertecies.push_back({ EMath::Vector3(-0.5f,0.0f,0.0f), Colors::Red });
		mVertecies.push_back({ EMath::Vector3(0.0f,0.0f,0.0f), Colors::Yellow });

		mVertecies.push_back({ EMath::Vector3(0.0f,0.0f,0.0f), Colors::Yellow });
		mVertecies.push_back({ EMath::Vector3(0.5f,0.0f,0.0f), Colors::Red });
		mVertecies.push_back({ EMath::Vector3(0.0f,-1.0f,0.0f), Colors::Red });

		mVertecies.push_back({ EMath::Vector3(-0.5f,0.0f,0.0f), Colors::Red });
		mVertecies.push_back({ EMath::Vector3(-0.25f,0.5f,0.0f), Colors::Red });
		mVertecies.push_back({ EMath::Vector3(0.0f,0.0f,0.0f), Colors::Yellow });

		mVertecies.push_back({ EMath::Vector3(+0.0f,0.0f,0.0f), Colors::Yellow });
		mVertecies.push_back({ EMath::Vector3(+0.25f,0.5f,0.0f), Colors::Red });
		mVertecies.push_back({ EMath::Vector3(0.5f,0.0f,0.0f), Colors::Red });

		/*mVertecies.push_back({ EMath::Vector3(-0.25f,0.5f,0.0f), Colors::Red });
		mVertecies.push_back({ EMath::Vector3(+0.25f,0.5f,0.0f), Colors::Red });
		mVertecies.push_back({ EMath::Vector3(0.0f,0.0f,0.0f), Colors::Yellow });*/
	}
};