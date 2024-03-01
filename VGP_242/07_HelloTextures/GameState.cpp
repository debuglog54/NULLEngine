#include "GameState.h"
#include "Input/Inc/InputSystem.h"

using namespace NULLEngine::EMath;

float rotationX = 0.0f;
float rotationY = 0.0f;
float posZ = -1.0f;
float posX = 0.0f;

void GameState::Initialize()
{
    // NDC - Normalized Device Coordinate
    //
    //            +-----------------+
    //           /                 /|
    //     1.0  +-----------------+ |
    //          |        ^        | |
    //          |        |        | |
    //          | <------+------> | |
    //          |        |        | | 1.0
    //          |        v        |/
    //    -1.0  +-----------------+ 0.0
    //      -1.0               1.0
    //

    OnCreateShape();

    mCamera.SetLookAt({ 0.0f,0.0,0.0f });
    posX = 0.0f;
    posZ = -4.0f;

    mConstantBuffer.Initialize(sizeof(Matrix4));
    mMeshBuffer.Initialize(mMesh);
    mVertexShader.Initiliaze(L"../../Assets/Shaders/DoTexturing.fx", VE_Position | VE_TexCoord);
    mPixelShader.Initiliaze(L"../../Assets/Shaders/DoTexturing.fx");

    mSphereTextureId = TextureManager::Get()->LoadTexture(L"Textures/earth.jpg");
}

void GameState::Terminate()
{
    mConstantBuffer.Terminate();
    mMeshBuffer.Terminate();
    mVertexShader.Terminate();
    mPixelShader.Terminate();
}

void GameState::Update(float deltaTime)
{
    if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::UP))
    {
        rotationY += EMath::kPiByTwo * 1 / 60;
    }
    else if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::DOWN))
    {
        rotationY -= EMath::kPiByTwo * 1 / 60;
    }
    if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::RIGHT))
    {
        rotationX += EMath::kPiByTwo * 1 / 60;
    }
    else if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::LEFT))
    {
        rotationX -= EMath::kPiByTwo * 1 / 60;
    }

    if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::W))
    {
        posZ += EMath::kPiByTwo * 1 / 60;
    }
    else if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::S))
    {
        posZ -= EMath::kPiByTwo * 1 / 60;
    }

    if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::A))
    {
        posX -= EMath::kPiByTwo * 1 / 60;
    }
    else if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::D))
    {
        posX += EMath::kPiByTwo * 1 / 60;
    }

    if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::R))
    {
        posX = 0.0f;
        posZ = -2.0f;
        rotationX = 0.0f;
        rotationY = 0.0f;
    }
    mCamera.SetPosition({ posX,0.0f,posZ });
}

void GameState::Render()
{
    mVertexShader.Bind();
    mPixelShader.Bind();

    Matrix4 cubeWorldRotY = Matrix4::RotationY(rotationX);
    Matrix4 cubeWorldRotX = Matrix4::RotationX(rotationY);
    Matrix4 view = mCamera.GetViewMatrix();
    Matrix4 proj = mCamera.GetProjectionMatrix();
    Matrix4 wvp = Transpose(cubeWorldRotX * cubeWorldRotY * view * proj);

    mConstantBuffer.Update(&wvp);
    mConstantBuffer.BindVS(0);
    TextureManager::Get()->BindVS(mSphereTextureId, 0);
    TextureManager::Get()->BindPS(mSphereTextureId, 0);
   
    mMeshBuffer.Render();
}

void GameState::DebugUI()
{
}

void GameState::OnCreateShape()
{
    mMesh = MeshBuilder::CreateSpherePX(36, 36, 3.0f);

    mCamera.SetLookAt({ 0.0f,0.0,0.0f });
    posX = 0.0f;
    posZ = -4.0f;
}