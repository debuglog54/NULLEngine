#include "GameState.h"
#include "Input/Inc/InputSystem.h"

void GameState::Initialize()
{
	NULLEngine::Graphics::GraphicsSystem::Get()->SetClearColor(Colors::Black);
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = Input::InputSystem::Get();

	if (inputSystem->IsKeyPressed(Input::KeyCode::F1))
	{
		NULLEngine::Graphics::GraphicsSystem::Get()->SetClearColor(Colors::Red);
	}
	else if (inputSystem->IsKeyPressed(Input::KeyCode::F2))
	{
		NULLEngine::Graphics::GraphicsSystem::Get()->SetClearColor(Colors::Green);
	}
}
