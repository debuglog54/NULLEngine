#include "Precompiled.h"
#include "FPSCameraControllerComponent.h"
#include "GameObject.h"
#include "CameraComponent.h"

using namespace NULLEngine;
using namespace NULLEngine::Input;

void FPSCameraControllerComponent::Initialize()
{
    mCameraComponent = GetOwner().GetComponent<CameraComponent>();
    ASSERT(mCameraComponent != nullptr, "FPSCameraControllerComponnet: GameObject doesnt have a camera component!");
}

void FPSCameraControllerComponent::Terminate()
{
    mCameraComponent = nullptr;
}

void FPSCameraControllerComponent::Update(float deltaTime)
{
	auto input = InputSystem::Get();

	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? mMoveSpeed : mMoveSpeed;
	const float turnSpeed = 0.1f;

	auto& camera = mCameraComponent->GetCamera();

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