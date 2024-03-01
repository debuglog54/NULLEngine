#include "GameState.h"
#include "Input/Inc/InputSystem.h"

using namespace NULLEngine::EMath;
using namespace NULLEngine::Input;

Vector3 camrePos = Vector3::Zero();
float mYaw, mPitch, mRoll;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f,1.0f,-3.0f });
	mCamera.SetLookAt({ 0.0f,0.0,0.0f });

	mDirectionalLight.direction = EMath::Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionalLight.ambient = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.7f, 0.7f, 0.7f, 1.0f };

	Mesh ground = MeshBuilder::CreatePlane(20, 20, 1.0f);
	mGround.meshBuffer.Initialize(ground);
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture(L"Textures/misc/concrete.jpg");
	mGround.material.ambient = { 0.5f,0.5f,0.5f,1.0f };
	mGround.material.diffuse = { 0.8f,0.5f,0.5f,1.0f };
	mGround.material.specular = { 0.5f,0.5f,0.5f,1.0f };
	mGround.material.power = 10.0f;
	mGround.transform.position = { Vector3(0.0, -2.0, 0.0) };

	Mesh sphere = MeshBuilder::CreateSphere(60.0f, 60.0f, 1.0f);
	mSphere.meshBuffer.Initialize(sphere);
	mSphere.diffuseMapId = TextureManager::Get()->LoadTexture(L"Textures/misc/basketball.jpg");
	mSphere.material.ambient = { 0.5f,0.5f,0.5f,1.0f };
	mSphere.material.diffuse = { 0.8f,0.5f,0.5f,1.0f };
	mSphere.material.specular = { 0.5f,0.5f,0.5f,1.0f };
	mSphere.material.power = 10.0f;
	mSphere.transform.position.y = 1.0f;

	mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

	mAnimationTime = 0.0f;
	mSphereAnimation = AnimationBuilder()
		.AddPositionKey(Vector3(0.0f, 0.0f, 0.0f), 0.0f, EaseType::EaseIn)
		.AddRotationKey(Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, 0.0f), 0.0f, EaseType::EaseIn)
		.AddScaleKey(Vector3(1.0f, 1.0f, 1.0f), 0.0f, EaseType::EaseIn)

		.AddPositionKey(Vector3(1.0f, 1.0f, 0.0f), 1.0f, EaseType::EaseIn)
		.AddRotationKey(Quaternion::CreateFromYawPitchRoll(45.0f, 0.0f, 0.0f), 1.0f, EaseType::EaseIn)
		.AddScaleKey(Vector3(1.0f, 1.0f, 1.0f), 1.0f, EaseType::EaseIn)

		.AddPositionKey(Vector3(2.0f, 2.0f, 0.0f), 2.0f, EaseType::EaseIn)
		.AddRotationKey(Quaternion::CreateFromYawPitchRoll(90.0f, 45.0f, 0.0f), 2.0f, EaseType::EaseIn)
		.AddScaleKey(Vector3(1.0f, 1.0f, 1.0f), 2.0f, EaseType::EaseIn)

		.AddPositionKey(Vector3(3.0f, 3.0f, 0.0f), 3.0f, EaseType::EaseIn)
		.AddRotationKey(Quaternion::CreateFromYawPitchRoll(180.0f, 65.0f, 0.0f), 3.0f, EaseType::EaseIn)
		.AddScaleKey(Vector3(2.0f, 2.0f, 2.0f), 3.0f, EaseType::EaseIn)

		.AddPositionKey(Vector3(4.0f, 4.0f, 4.0f), 4.0f, EaseType::EaseIn)
		.AddRotationKey(Quaternion::CreateFromYawPitchRoll(180.0f, 90.0f, 0.0f), 4.0f, EaseType::EaseIn)
		.AddScaleKey(Vector3(2.0f, 2.0f, 2.0f), 4.0f, EaseType::EaseIn)

		.AddPositionKey(Vector3(4.0f, 7.0f, 4.0f), 5.0f, EaseType::EaseIn)
		.AddRotationKey(Quaternion::CreateFromYawPitchRoll(90.0f, 90.0f, 0.0f), 5.0f, EaseType::EaseIn)
		.AddScaleKey(Vector3(2.0f, 2.0f, 2.0f), 5.0f, EaseType::EaseIn)

		.AddPositionKey(Vector3(4.0f, 3.0f, 4.0f), 6.0f, EaseType::EaseIn)
		.AddRotationKey(Quaternion::CreateFromYawPitchRoll(90.0f, 90.0f, 90.0f), 6.0f, EaseType::EaseIn)
		.AddScaleKey(Vector3(3.0f, 3.0f, 3.0f), 6.0f, EaseType::EaseIn)

		.AddPositionKey(Vector3(5.0f, 5.0f, 5.0f), 7.0f, EaseType::EaseIn)
		.AddRotationKey(Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, 0.0f), 7.0f, EaseType::EaseIn)
		.AddScaleKey(Vector3(4.0f, 4.0f, 4.0f), 7.0f, EaseType::EaseIn)

		.AddPositionKey(Vector3(5.0f, 8.0f, 5.0f), 8.0f, EaseType::EaseIn)
		.AddRotationKey(Quaternion::CreateFromYawPitchRoll(0.0f, 90.0f, 180.0f), 8.0f, EaseType::EaseIn)
		.AddScaleKey(Vector3(3.0f, 3.0f, 3.0f), 8.0f, EaseType::EaseIn)

		.AddPositionKey(Vector3(5.0f, 4.0f, 5.0f), 9.0f, EaseType::EaseIn)
		.AddRotationKey(Quaternion::CreateFromYawPitchRoll(0.0f, 90.0f, 270.0f), 9.0f, EaseType::EaseIn)
		.AddScaleKey(Vector3(2.0f, 2.0f, 2.0f), 9.0f, EaseType::EaseIn)

		.AddPositionKey(Vector3(5.0f, 12.0f, 5.0f), 10.0f, EaseType::EaseIn)
		.AddRotationKey(Quaternion::CreateFromYawPitchRoll(0.0f, 90.0f, 180.0f), 10.0f, EaseType::EaseIn)
		.AddScaleKey(Vector3(1.5f, 1.5f, 1.5f), 10.0f, EaseType::EaseIn)

		.AddPositionKey(Vector3(5.0f, 7.0f, 5.0f), 11.0f, EaseType::EaseIn)
		.AddRotationKey(Quaternion::CreateFromYawPitchRoll(0.0f, 90.0f, 90.0f), 11.0f, EaseType::EaseIn)
		.AddScaleKey(Vector3(1.0f, 1.0f, 1.0f), 11.0f, EaseType::EaseIn)

		.AddPositionKey(Vector3(3.0f, 8.0f, 3.0f), 12.0f, EaseType::EaseIn)
		.AddRotationKey(Quaternion::CreateFromYawPitchRoll(0.0f, 90.0f, 45.0f), 12.0f, EaseType::EaseIn)
		.AddScaleKey(Vector3(1.0f, 1.0f, 1.0f), 12.0f, EaseType::EaseIn)

		.AddPositionKey(Vector3(2.0f, 3.0f, 2.0f), 13.0f, EaseType::EaseIn)
		.AddRotationKey(Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, 0.0f), 13.0f, EaseType::EaseIn)
		.AddScaleKey(Vector3(1.0f, 1.0f, 1.0f), 13.0f, EaseType::EaseIn)

		.AddPositionKey(Vector3(0.0f, 1.0f, 0.0f), 14.0f, EaseType::EaseIn)
		.AddRotationKey(Quaternion::CreateFromYawPitchRoll(0.0f, 45.0f, 0.0f), 14.0f, EaseType::EaseIn)
		.AddScaleKey(Vector3(1.0f, 1.0f, 1.0f), 14.0f, EaseType::EaseIn)

		.AddPositionKey(Vector3(0.0f, 0.0f, 0.0f), 15.0f, EaseType::EaseIn)
		.AddRotationKey(Quaternion::CreateFromYawPitchRoll(0.0f, 90.0f, 0.0f), 15.0f, EaseType::EaseIn)
		.AddScaleKey(Vector3(1.0f, 1.0f, 1.0f), 15.0f, EaseType::EaseIn)

		.Build();
}

void GameState::Terminate()
{
	mStandardEffect.Terminate();
	mGround.Terminate();
	mSphere.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto input = InputSystem::Get();

	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.1f;

	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed * deltaTime);
	}

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}

	mAnimationTime += deltaTime;
	const float animDuration = mSphereAnimation.GetDuration();
	while (mAnimationTime > animDuration)
	{
		mAnimationTime -= animDuration;
	}
	mSphere.transform = mSphereAnimation.GetTransform(mAnimationTime);
}

void GameState::Render()
{
	mStandardEffect.Begin();
	mStandardEffect.Render(mGround);
	mStandardEffect.Render(mSphere);
	mStandardEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Contorls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction##Light", &mDirectionalLight.direction.x, 0.01f, -1.0f, 1.0f))
		{
			mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Difuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}
	ImGui::Separator();

	ImGui::DragFloat("Yaw##", &mYaw, 0.1f);
	ImGui::DragFloat("Pitch##", &mPitch, 0.1f);
	ImGui::DragFloat("Roll##", &mRoll, 0.1f);

	mStandardEffect.DebugUI();
	ImGui::End();
}