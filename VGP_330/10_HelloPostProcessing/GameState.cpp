#include "GameState.h"
#include "Input/Inc/InputSystem.h"

using namespace NULLEngine::EMath;
using namespace NULLEngine::Input;

Vector3 camrePos = Vector3::Zero();

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f,1.0f,-3.0f });
	mCamera.SetLookAt({ 0.0f,0.0,0.0f });

	mDirectionalLight.direction = EMath::Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionalLight.ambient = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.7f, 0.7f, 0.7f, 1.0f };

	mEarth = MeshBuilder::CreateSphere(60, 60, 1.0f);

	mEarthObject.meshBuffer.Initialize(mEarth);
	mEarthObject.diffuseMapId = TextureManager::Get()->LoadTexture(L"Textures/earth.jpg");
	mEarthObject.normalMapId = TextureManager::Get()->LoadTexture(L"Textures/earth_normal.jpg");
	mEarthObject.displacementMapId = TextureManager::Get()->LoadTexture(L"Textures/earth_bump.jpg");
	mEarthObject.specularMapId = TextureManager::Get()->LoadTexture(L"Textures/earth_spec.jpg");

	mScreenQuad.meshBuffer.Initialize(MeshBuilder::CreateScreenQuad());

	mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

	mPostProcessingEffect.Initialize(L"../../Assets/Shaders/PostProcessing.fx");
	mPostProcessingEffect.SetTexture(&mRenderTarget);
	mPostProcessingEffect.SetMode(PostProcessingEffect::Mode::None);

	auto gs = GraphicsSystem::Get();
	const auto screenWidth = gs->GetBackBufferWidth();
	const auto screenHeight = gs->GetBackBufferHeight();
	mRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
}

void GameState::Terminate()
{
	mRenderTarget.Terminate();
	mScreenQuad.Terminate();
	mPostProcessingEffect.Terminate();
	mEarthObject.Terminate();
	mStandardEffect.Terminate();
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
}

void GameState::Render()
{
	mRenderTarget.BeginRender();
	{
		mStandardEffect.Begin();
		mStandardEffect.Render(mEarthObject);
		mStandardEffect.End();

		SimpleDraw::AddGroundPlane(10.0f, Colors::White);
		SimpleDraw::AddTransform(Matrix4::Identity);
		SimpleDraw::Render(mCamera);
	}
	mRenderTarget.EndRender();

	mPostProcessingEffect.Begin();
	mPostProcessingEffect.Render(mScreenQuad);

	ShowNormals();
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

	ImGui::Checkbox("Draw Normls", &mShowNormals);
	mStandardEffect.DebugUI();
	mPostProcessingEffect.DebugUI();

	ImGui::End();
}

void GameState::ShowNormals()
{
	if (mShowNormals)
	{
		for (const auto& v : mEarth.vertices)
		{
			SimpleDraw::AddLine(v.position, v.position + v.normal * 0.25f, Colors::Cyan);
		}

		/*	for (int i = 0; i < mMesh.vertices.size(); i++)
			{
				if (i + 2 < mMesh.vertices.size())
				{
					SimpleDraw::AddLine(mMesh.vertices[i].position, mMesh.vertices[i + 1].position, Colors::Cyan);
					SimpleDraw::AddLine(mMesh.vertices[i + 1].position, mMesh.vertices[i + 2].position, Colors::Cyan);
					SimpleDraw::AddLine(mMesh.vertices[i].position, mMesh.vertices[i + 2].position, Colors::Cyan);

				}

			}*/

		SimpleDraw::Render(mCamera);
	}
}