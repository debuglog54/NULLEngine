#include "SideScrollCameraComponent.h"
#include "NULLEngine/Inc/GameWorld.h"
#include "GameManagerService.h"

using namespace NULLEngine;
using namespace NULLEngine::Input;


void SideScrollCameraComponent::Initialize()
{
    mCameraComponent = GetOwner().GetComponent<NULLEngine::CameraComponent>();
    ASSERT(mCameraComponent != nullptr, "FPSCameraControllerComponnet: GameObject doesnt have a camera component!");
    auto& world = GetOwner().GetWorld();
    auto gameManager = world.GetService<GameManagerService>();
    const auto& player = gameManager->GetPlayer();
    mPlayerTransform = player.GetOwner().GetComponent<NULLEngine::TransformComponent>();
}

void SideScrollCameraComponent::Terminate()
{
    mCameraComponent = nullptr;
}

void SideScrollCameraComponent::Update(float deltaTime)
{

    auto input = InputSystem::Get();
    auto& camera = mCameraComponent->GetCamera();

    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? mMoveSpeed : mMoveSpeed;
    const float turnSpeed = 0.1f;

    if (input->IsKeyDown(KeyCode::CAPSLOCK))
    {
        if (input->IsKeyDown(KeyCode::W))
        {
            camera.Walk(moveSpeed * deltaTime);
        }
        else if (input->IsKeyDown(KeyCode::S))
        {
            camera.Walk(-moveSpeed * deltaTime);
        }

        if (input->IsKeyDown(KeyCode::D))
        {
            camera.Strafe(moveSpeed * deltaTime);
        }
        else if (input->IsKeyDown(KeyCode::A))
        {
            camera.Strafe(-moveSpeed * deltaTime);
        }

        if (input->IsKeyDown(KeyCode::E))
        {
            camera.Rise(moveSpeed * deltaTime);
        }
        else if (input->IsKeyDown(KeyCode::Q))
        {
            camera.Rise(-moveSpeed * deltaTime);
        }

        if (input->IsMouseDown(MouseButton::RBUTTON))
        {
            camera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
            camera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
        }
    }
    else
    {
        camera.SetPosition(mPlayerTransform->position + mOffset);
        camera.SetLookAt(mPlayerTransform->position + Vector3::YAxis());
    }
}