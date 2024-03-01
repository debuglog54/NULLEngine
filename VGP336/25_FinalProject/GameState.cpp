#include "GameState.h"
#include "Input/Inc/InputSystem.h"
#include "CustomComponents/PlayerControllerComponent.h"
#include "CustomComponents/PlayerInputControllerComponent.h"
#include "CustomComponents/SideScrollCameraComponent.h"
#include "CustomComponents/CoinComponent.h"
#include "CustomComponents/GameManagerService.h"

using namespace NULLEngine::EMath;
using namespace NULLEngine::Input;
using namespace NULLEngine::Physics;

namespace
{
    bool CustomGameMake(const char* componentName, const rapidjson::Value& value, GameObject& gameObject)
    {
        if (strcmp(componentName, "PlayerControllerComponent") == 0)
        {
            PlayerControllerComponent* playerControllerComponent = gameObject.AddComponent<PlayerControllerComponent>();
            return true;
        }
        else if (strcmp(componentName, "PlayerInputControllerComponent") == 0)
        {
            PlayerInputControllerComponent* playerInputControllerComponent = gameObject.AddComponent<PlayerInputControllerComponent>();
            return true;
        }
        else if (strcmp(componentName, "SideScrollCameraComponent") == 0)
        {
            SideScrollCameraComponent* slideScrollCamera = gameObject.AddComponent<SideScrollCameraComponent>();
            if (value.HasMember("MoveSpeed"))
            {
                const float moveSpeed = value["MoveSpeed"].GetFloat();
                slideScrollCamera->SetMoveSpeed(moveSpeed);
            }

            if (value.HasMember("TurnSpeed"))
            {
                const float turnSpeed = value["TurnSpeed"].GetFloat();
                slideScrollCamera->SetTurnSpeed(turnSpeed);
            }
            return true;
        }
        else if (strcmp(componentName, "CoinComponent") == 0)
        {
            CoinComponent* cointComponnent = gameObject.AddComponent<CoinComponent>();
            return true;
        }
        return false;
    }
}

bool GameState::CustomLocalMakeService(const char* service)
{
    if (strcmp(service, "GameManagerService") == 0)
    {
        auto gameManagerService = mGameWorld.AddService<GameManagerService>();
        return false;
    }

    return true;
}

Vector3 camrePos = Vector3::Zero();
float mYaw, mPitch, mRoll;

void GameState::Initialize()
{
    CustomLocalMakeService("GameManagerService");
    GameObjectFactory::SetCustomMake(CustomGameMake);
    mGameWorld.LoadLevel("../../Assets/Templates/Levels/FinalLevel.json");
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