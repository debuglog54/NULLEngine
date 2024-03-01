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

   // mCamera.SetPosition({ 0.0f,1.0f,-1.0f });
    mCamera.SetLookAt({ 0.0f,0.0,0.0f });

    OnCreateShape();

    mConstantBuffer.Initialize(sizeof(Matrix4));
    mMeshBuffer.Initialize(mVertecies.data(), sizeof(Vertex), mVertecies.size());
    mVertexShader.Initiliaze(L"../../Assets/Shaders/DoTransform.fx", VE_Position | VE_Color );
    mPixelShader.Initiliaze(L"../../Assets/Shaders/DoTransform.fx");
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
        rotationX -= EMath::kPiByTwo * 1 / 60;
    }
    else if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::LEFT))
    {
        rotationX += EMath::kPiByTwo * 1 / 60;
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

    mMeshBuffer.Render();
}

void GameState::DebugUI()
{

}

void GameState::OnCreateShape()
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

    mVertecies.push_back({ EMath::Vector3(-0.5f,-0.5f,-0.5f), Colors::Yellow });
    mVertecies.push_back({ EMath::Vector3(-0.5f,-0.5f, 0.5f), Colors::Red });
    mVertecies.push_back({ EMath::Vector3(-0.5f, 0.5f, 0.5f), Colors::Green });
    mVertecies.push_back({ EMath::Vector3(0.5f, 0.5f, -0.5f), Colors::Yellow });
    mVertecies.push_back({ EMath::Vector3(-0.5f, -0.5f, -0.5f), Colors::Red });
    mVertecies.push_back({ EMath::Vector3(-0.5f, 0.5f, -0.5f), Colors::Green });
    mVertecies.push_back({ EMath::Vector3(0.5f, -0.5f, 0.5f), Colors::Yellow });
    mVertecies.push_back({ EMath::Vector3(-0.5f, -0.5f, -0.5f), Colors::Red });
    mVertecies.push_back({ EMath::Vector3(0.5f, -0.5f, -0.5f), Colors::Green });
    mVertecies.push_back({ EMath::Vector3(0.5f, 0.5f, -0.5f), Colors::Yellow });
    mVertecies.push_back({ EMath::Vector3(0.5f, -0.5f, -0.5f), Colors::Red });
    mVertecies.push_back({ EMath::Vector3(-0.5f, -0.5f, -0.5f), Colors::Green });
    mVertecies.push_back({ EMath::Vector3(-0.5f, -0.5f, -0.5f), Colors::Yellow });
    mVertecies.push_back({ EMath::Vector3(-0.5f, 0.5f, 0.5f), Colors::Red });
    mVertecies.push_back({ EMath::Vector3(-0.5f, 0.5f, -0.5f), Colors::Green });
    mVertecies.push_back({ EMath::Vector3(0.5f, -0.5f, 0.5f), Colors::Yellow });
    mVertecies.push_back({ EMath::Vector3(-0.5f, -0.5f, 0.5f), Colors::Red });
    mVertecies.push_back({ EMath::Vector3(-0.5f, -0.5f, -0.5f), Colors::Green });
    mVertecies.push_back({ EMath::Vector3(-0.5f, 0.5f, 0.5f), Colors::Yellow });
    mVertecies.push_back({ EMath::Vector3(-0.5f, -0.5f, 0.5f), Colors::Red });
    mVertecies.push_back({ EMath::Vector3(0.5f, -0.5f, 0.5f), Colors::Green });
    mVertecies.push_back({ EMath::Vector3(0.5f, 0.5f, 0.5f), Colors::Yellow });
    mVertecies.push_back({ EMath::Vector3(0.5f, -0.5f, -0.5f), Colors::Red });
    mVertecies.push_back({ EMath::Vector3(0.5f, 0.5f, -0.5f), Colors::Green });
    mVertecies.push_back({ EMath::Vector3(0.5f, -0.5f, -0.5f), Colors::Yellow });
    mVertecies.push_back({ EMath::Vector3(0.5f, 0.5f, 0.5f), Colors::Red });
    mVertecies.push_back({ EMath::Vector3(0.5f, -0.5f, 0.5f), Colors::Green });
    mVertecies.push_back({ EMath::Vector3(0.5f, 0.5f, 0.5f), Colors::Yellow });
    mVertecies.push_back({ EMath::Vector3(0.5f, 0.5f, -0.5f), Colors::Red });
    mVertecies.push_back({ EMath::Vector3(-0.5f, 0.5f, -0.5f), Colors::Green });
    mVertecies.push_back({ EMath::Vector3(0.5f, 0.5f, 0.5f), Colors::Green });
    mVertecies.push_back({ EMath::Vector3(-0.5f, 0.5f, -0.5f), Colors::Yellow });
    mVertecies.push_back({ EMath::Vector3(-0.5f, 0.5f, 0.5f), Colors::Red });
    mVertecies.push_back({ EMath::Vector3(0.5f, 0.5f, 0.5f), Colors::Green });
    mVertecies.push_back({ EMath::Vector3(-0.5f, 0.5f, 0.5f), Colors::Yellow });
    mVertecies.push_back({ EMath::Vector3(0.5f, -0.5f, 0.5f), Colors::Red });
}