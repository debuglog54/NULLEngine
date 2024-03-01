#include "PlayerInputControllerComponent.h"

#include "PlayerControllerComponent.h"
#include "NULLEngine/Inc/GameObject.h"

#include "Input/Inc/InputSystem.h"

using namespace NULLEngine::Input;

void PlayerInputControllerComponent::Initialize()
{
    mPlayerController = GetOwner().GetComponent<PlayerControllerComponent>();
    ASSERT(mPlayerController != nullptr, "PlayerInputControllerComponent: GameObject doesnt have a PlayerControllerComponent component!");
}

void PlayerInputControllerComponent::Terminate()
{
    mPlayerController = nullptr;
}

void PlayerInputControllerComponent::Update(float deltaTime)
{
    auto input = InputSystem::Get();
    bool  isRunninng = input->IsKeyDown(KeyCode::LSHIFT) ? true : false;
    mPlayerController->SetRunFlag(isRunninng);

    if (mPlayerController->IsAttacking())
    {
        return;
    }

    if (input->IsKeyPressed(KeyCode::SPACE))
    {
        mPlayerController->Jump();
    }
    else if (input->IsKeyDown(KeyCode::A))
    {
        mPlayerController->Move(Vector3::XAxis() * -1, deltaTime);
    }
    else if (input->IsKeyDown(KeyCode::D))
    {
        mPlayerController->Move(Vector3::XAxis(), deltaTime);
    }
    else
    {
        mPlayerController->Idle(deltaTime);
    }

    if (input->IsMousePressed(MouseButton::LBUTTON))
    {
        mPlayerController->Attack();
    }
}

void PlayerInputControllerComponent::DebugUI()
{
}
