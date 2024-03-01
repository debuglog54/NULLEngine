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

	Mesh ball = MeshBuilder::CreateSphere(30, 30, 1.0f);
	mBall.meshBuffer.Initialize(ball);
	mBall.diffuseMapId = TextureManager::Get()->LoadTexture(L"Textures/misc/basketball.jpg");
	mBall.material.ambient = { 0.5f,0.5f,0.5f,1.0f };
	mBall.material.diffuse = { 0.8f,0.5f,0.5f,1.0f };
	mBall.material.specular = { 0.5f,0.5f,0.5f,1.0f };
	mBall.material.power = 10.0f;
	mBall.transform.position = { Vector3(0.0f, 5.0f, 0.0f) };
	mSphereCollisionShape.Initialize(1.0f);
	mBallRigidBody.Initialize(mBall.transform, mSphereCollisionShape, 5.0f);


	Mesh floor = MeshBuilder::CreatePlane(30, 30, 1.0f);
	mFloor.meshBuffer.Initialize(floor);
	mFloor.diffuseMapId = TextureManager::Get()->LoadTexture(L"Textures/misc/concrete.jpg");
	mFloor.material.ambient = { 0.5f,0.5f,0.5f,1.0f };
	mFloor.material.diffuse = { 0.8f,0.5f,0.5f,1.0f };
	mFloor.material.specular = { 0.5f,0.5f,0.5f,1.0f };
	mFloor.material.power = 10.0f;
	mFloor.transform.position = { Vector3(0.0f, 0.0f, 0.0f) };
	mBoxCollisionShape.Initialize(Vector3(15,0.1f,15));
	mFloorlRigidBody.Initialize(mFloor.transform, mBoxCollisionShape, 0.0f);

	mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	NULLEngine::Graphics::GraphicsSystem::Get()->SetClearColor(Colors::DarkOliveGreen);
}

void GameState::Terminate()
{
	mStandardEffect.Terminate();
	mBall.Terminate();
	mBallRigidBody.Terminate();
	mSphereCollisionShape.Terminate();

	mFloor.Terminate();
	mFloorlRigidBody.Terminate();
	mBoxCollisionShape.Terminate();
}

int gAnimationIndex = 0;
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
}

void GameState::Render()
{
	mStandardEffect.Begin();
	mStandardEffect.Render(mBall);
	mStandardEffect.Render(mFloor);
	mStandardEffect.End();

	SimpleDraw::AddGroundPlane(20.0f, Colors::White);
	SimpleDraw::Render(mCamera);
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
	PhysicsWorld::Get()->DebugUI();
	ImGui::End();
}

