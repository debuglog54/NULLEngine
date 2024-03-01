#pragma once
#include <NULLEngine/Inc/NULLEngine.h>

using namespace NULLEngine;
using namespace NULLEngine::Graphics;

class GameState : public AppState
{
public:
    virtual ~GameState() = default;
    virtual void Initialize() override;
    virtual void Terminate()override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual void DebugUI() override;

protected:
    virtual void OnCreateShape();

    struct Vertex
    {
        NULLEngine::EMath::Vector3 position;
        NULLEngine::Color color;
    };

    Camera mCamera;
    ConstantBuffer mConstantBuffer;
    MeshPC mCube;
    VertexShader mVertexShader{};
    MeshBuffer mMeshBuffer{};
    PixelShader mPixelShader{};
};

class RectangleDemo : public GameState
{
public:
    virtual  ~RectangleDemo() = default;

protected:
    void OnCreateShape() override
    {
        mCube = MeshBuilder::CreateRectanglePC(5, 9,2);
    }
};

class PlaneDemo : public GameState
{
public:
    virtual  ~PlaneDemo() = default;

protected:
    void OnCreateShape() override
    {
        mCube = MeshBuilder::CreatePlanePC(4, 4, 1);
    }
};

class SphereDemo : public GameState
{
public:
    virtual  ~SphereDemo() = default;

protected:
    void OnCreateShape() override
    {
        mCube = MeshBuilder::CreateSpherePC(36, 36, 3.0f);
    }
};

class CylinderDemo : public GameState
{
public:
    virtual  ~CylinderDemo() = default;

protected:
    void OnCreateShape() override
    {
        mCube = MeshBuilder::CreateCylinderPC(8, 2);
    }
};
