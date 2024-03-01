#include "GameState.h"
#include "Input/Inc/InputSystem.h"
#include "AnimationChangeComponent.h"

using namespace NULLEngine::EMath;
using namespace NULLEngine::Input;
using namespace NULLEngine::Physics;

namespace
{
	bool CustomGameMake(const char* componentName, const rapidjson::Value& value, GameObject& gameObject)
	{
		if (strcmp(componentName, "AnimationChangeComponent") == 0)
		{
			AnimationChangeComponent* animationChangeComponent = gameObject.AddComponent<AnimationChangeComponent>();
			return true;
		}
		return false;
	}
}

Vector3 camrePos = Vector3::Zero();
float mYaw, mPitch, mRoll;

void GameState::Initialize()
{
	GameObjectFactory::SetCustomMake(CustomGameMake);
	mGameWorld.LoadLevel("../../Assets/Templates/Levels/TestLevel.json");
}

void GameState::Terminate()
{
	mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
	mGameWorld.Render();
}

void GameState::DebugUI()
{
	mGameWorld.DebugUI();
}

void GameState::EditorUI()
{
	mGameWorld.EditorUI();
}