#include "GameState.h"
#include "Input/Inc/InputSystem.h"

using namespace NULLEngine::EMath;
using namespace NULLEngine::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -10.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mDirectionalLight.direction = EMath::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.7f, 0.7f, 0.7f, 1.0f };

	Mesh ground = MeshBuilder::CreatePlane(20, 20, 1.0f);
	mGround.meshBuffer.Initialize(ground);
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture(L"Textures/misc/concrete.jpg");
	mGround.material.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mGround.material.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mGround.material.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	mGround.material.power = 10.0f;

	mGroundCollisionShape.Initialize({ 10.0f, 0.005f, 10.0f });
	mGroundRigidBody.Initialize(mGround.transform, mGroundCollisionShape);

	mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

	mParticleEffect.Initialize();
	mParticleEffect.SetCamera(mCamera);

	DefaultSetup();

	Vector3 dir = Vector3(0.0f, 3.0f, 0.0f);
	Vector3 pos = Vector3(0.0f, 0.5f, 0.0f);
	mParticleSystem.Initialize(info, pos, dir);
}

void GameState::Terminate()
{
	mParticleSystem.Terminate();

	mParticleEffect.Terminate();
	mStandardEffect.Terminate();

	mGroundRigidBody.Terminate();
	mGroundCollisionShape.Terminate();
	mGround.Terminate();

	mBallRigidBody.Terminate();
	mSphereCollisionShape.Terminate();
	mBall.Terminate();

}

void GameState::Update(float deltaTime)
{
	// movement
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

	mParticleSystem.Update(deltaTime);
}

void GameState::Render()
{
	mStandardEffect.Begin();
	mStandardEffect.Render(mGround);
	mStandardEffect.End();

	mParticleEffect.Begin();
	mParticleSystem.Render(mParticleEffect);
	mParticleEffect.End();
}

void GameState::DefaultSetup()
{
	// particle system
	info.lifeTime = 999999999999.0f;
	info.maxParticles = 50;
	info.minParticleSpeed = 2.0f;
	info.maxParticleSpeed = 10.0f;
	info.minParticlesPerEmit = 2;
	info.maxParticlesPerEmit = 3;
	info.minSpawnAngle = -EMath::kPiByTwo * radMin;
	info.maxSpawnAngle = EMath::kPiByTwo * radMax;
	info.minTimeBetweenParticles = 0.01f;
	info.maxTimeBetweenParticles = 0.02f;
	radMax = 0.5f;
	radMin = 0.5f;

	// particle
	info.particleInfo.lifetime = 1.0f;
	info.particleInfo.startScale = { 1.0f, 1.0f, 1.0f };
	info.particleInfo.endScale = { 0.01f, 0.01f, 0.01f };
	info.particleInfo.startColor = Colors::OrangeRed;
	info.particleInfo.startColor.a = 0.8f;

	info.particleInfo.endColor = Colors::LightGoldenrodYellow;
	info.particleInfo.endColor.a = 0.0f;
	info.particleInfo.startColor.a = 0.8f;
	info.particleInfo.endColor.a = 0.0f;
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Separator();

	info.lifeTime = 999999999999.0f;

	// particle
	if (ImGui::CollapsingHeader("Particle System Info", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Start##Color", &info.particleInfo.startColor.r);
		ImGui::ColorEdit4("End##Color", &info.particleInfo.endColor.r);
		ImGui::DragInt("MaxParticles##", &info.maxParticles, 1);

		ImGui::DragFloat("MinParticleSpeed##", &info.minParticleSpeed, 0.25f);
		ImGui::DragFloat("MaxParticleSpeed##", &info.maxParticleSpeed, 0.25f);

		ImGui::DragInt("MinParticlesPerEmit##", &info.minParticlesPerEmit, 1);
		ImGui::DragInt("MaxParticlesPerEmit##", &info.maxParticlesPerEmit, 1);

		ImGui::DragFloat("minSpawnAngle##", &radMin, 0.1f);
		info.minSpawnAngle = EMath::kPiByTwo * radMin;

		ImGui::DragFloat("maxSpawnAngle##", &radMax, 0.1f);
		info.maxSpawnAngle = EMath::kPiByTwo * radMax;

		ImGui::DragFloat("MinTimeBetweenParticles##", &info.minTimeBetweenParticles, 1);
		ImGui::DragFloat("MaxTimeBetweenParticles##", &info.maxTimeBetweenParticles, 1);

		if (ImGui::CollapsingHeader("Particle Info", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat("Lifetime##", &info.particleInfo.lifetime, 0.2f);
			ImGui::DragFloat3("StartScale##", &info.particleInfo.startScale.x, 0.1f);
			ImGui::DragFloat3("EndScale##", &info.particleInfo.endScale.x, 0.1f);
			ImGui::DragFloat("Lifetime##", &info.particleInfo.lifetime, 0.2f);
		}
	}
	if (ImGui::Button("Restart"))
	{
		Vector3 dir = Vector3(0.0f, 3.0f, 0.0f);
		Vector3 pos = Vector3(0.0f, 0.5f, 0.0f);
		mParticleSystem.UpdateParameters(info, pos, dir, 0.0f);
	}
	if (ImGui::Button("Default"))
	{
		DefaultSetup();
		Vector3 dir = Vector3(0.0f, 3.0f, 0.0f);
		Vector3 pos = Vector3(0.0f, 0.5f, 0.0f);
		mParticleSystem.UpdateParameters(info, pos, dir, 0.0f);
	}
	
	PhysicsWorld::Get()->DebugUI();

	ImGui::End();

	SimpleDraw::Render(mCamera);
}