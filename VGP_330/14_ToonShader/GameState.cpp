#include "GameState.h"
#include "Input/Inc/InputSystem.h"

using namespace NULLEngine::EMath;
using namespace NULLEngine::Input;

Vector3 camrePos = Vector3::Zero();

void GameState::Initialize()
{
	NULLEngine::Graphics::GraphicsSystem::Get()->SetClearColor(Colors::LightGray);
	mCamera.SetPosition({ 0.0f,1.0f,-3.0f });
	mCamera.SetLookAt({ 0.0f,0.0,0.0f });

	mDirectionalLight.direction = EMath::Normalize({ 0.0f,0.3,-0.95f });
	mDirectionalLight.ambient = Colors::White;
	mDirectionalLight.diffuse = Colors::White;
	mDirectionalLight.specular = Colors::White;

	Mesh ground = MeshBuilder::CreatePlane(20, 20, 1.0f);
	mGround.meshBuffer.Initialize(ground);
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture(L"Textures/earth.jpg");
	mGround.material.ambient = { 0.5f,0.5f,0.5f,1.0f };
	mGround.material.diffuse = { 0.8f,0.5f,0.5f,1.0f };
	mGround.material.specular = { 0.5f,0.5f,0.5f,1.0f };
	mGround.material.power = 10.0f;

	Mesh wall = MeshBuilder::CreatePlane(20, 20, 1.0f);
	mWall.meshBuffer.Initialize(wall);
	mWall.diffuseMapId = TextureManager::Get()->LoadTexture(L"Textures/wall.png");
	mWall.normalMapId = TextureManager::Get()->LoadTexture(L"Textures/wall_normal.png");
	mWall.material.ambient = { 0.5f,0.5f,0.5f,1.0f };
	mWall.material.diffuse = { 0.8f,0.5f,0.5f,1.0f };
	mWall.material.specular = { 0.5f,0.5f,0.5f,1.0f };
	mWall.material.power = 10.0f;
	mWall.transform.position.z +=10.0f;
	mWall.transform.position.y+= 10.0f;
	mWall.transform.rotation = Quaternion::CreateFromAxisAngle(Vector3::XAxis(), 30.0f);

	Model character;
	ModelIO::LoadModel("../../Assets/Models/mutant/Mutant.model", character);
	ModelIO::LoadMaterial("../../Assets/Models/mutant/Mutant.model", character);
	mCharacter = CreateRenderGroup(character);

	mToonEffect.Initialize(L"../../Assets/Shaders/Toon.fx");
	mToonEffect.SetCamera(mCamera);
	mToonEffect.SetDirectionalLight(mDirectionalLight);
	mToonEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mToonEffect.SetShadowMap(&mShadowEffect.GetDepthMap());

	mImrovedStandardEffect.Initialize(L"../../Assets/Shaders/ImprovedStandard.fx");
	mImrovedStandardEffect.SetCamera(mCamera);
	mImrovedStandardEffect.SetDirectionalLight(mDirectionalLight);
	mImrovedStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mImrovedStandardEffect.SetShadowMap(&mShadowEffect.GetDepthMap());

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);
}

void GameState::Terminate()
{
	mImrovedStandardEffect.Terminate();
	mShadowEffect.Terminate();
	mToonEffect.Terminate();
	mGround.Terminate();
	CleanupRenderGroup(mCharacter);
}

float rotation = 0.0f;
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
	rotation += turnSpeed * deltaTime;
	mCharacter[0].transform.rotation = Quaternion::CreateFromAxisAngle(Vector3::YAxis(), rotation);
}

void GameState::Render()
{
	mShadowEffect.Begin();
	DrawRenderGroup(mShadowEffect, mCharacter);
	mShadowEffect.End();

	mToonEffect.Begin();
	DrawRenderGroup(mToonEffect, mCharacter);
	mToonEffect.Render(mGround);
	mToonEffect.End();

	mImrovedStandardEffect.Begin();
	mImrovedStandardEffect.Render(mWall);
	mImrovedStandardEffect.End();
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

	mToonEffect.DebugUI();
	mShadowEffect.DebugUI();
	mImrovedStandardEffect.DebugUI();
	ImGui::End();
}