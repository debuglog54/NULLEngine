#include "Precompiled.h"
#include "SimpleDraw.h"

#include"BlendState.h"
#include "Camera.h"
#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "VertexTypes.h"

using namespace NULLEngine;
using namespace NULLEngine::EMath;
using namespace NULLEngine::Graphics;

namespace
{
	class SimpleDrawImpl
	{
	public:
		void Initialize(uint32_t maxVertexCount);
		void Terminate();

		void AddLine(const Vector3& v0, const Vector3& v1, const Color& color);
		void AddFace(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Color& color);

		void Render(const Camera& camera);

	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		ConstantBuffer mConstantBuffer;
		MeshBuffer mMeshBuffer;
		BlendState mAlphaBlendState;

		std::unique_ptr<VertexPC[]> mLineVerticies;
		std::unique_ptr<VertexPC[]> mFaceVerticies;

		uint32_t mLineVertexCount = 0;
		uint32_t mFaceVertexCount = 0;
		uint32_t mMaxVertexCount = 0;

	};

	void SimpleDrawImpl::Initialize(uint32_t maxVertexCount)
	{
		mVertexShader.Initiliaze<VertexPC>(L"../../Assets/Shaders/SimpleDraw.fx");
		mPixelShader.Initiliaze(L"../../Assets/Shaders/SimpleDraw.fx");
		mConstantBuffer.Initialize(sizeof(Matrix4));
		mMeshBuffer.Initialize(nullptr, sizeof(VertexPC), maxVertexCount);
		mAlphaBlendState.Initialize(BlendState::Mode::AlphaBlend);

		mLineVerticies = std::make_unique<VertexPC[]>(maxVertexCount);
		mFaceVerticies = std::make_unique<VertexPC[]>(maxVertexCount);
		mLineVertexCount = 0;
		mFaceVertexCount = 0;
		mMaxVertexCount = maxVertexCount;
	}

	void SimpleDrawImpl::Terminate()
	{
		mAlphaBlendState.Terminate();
		mMeshBuffer.Terminate();
		mConstantBuffer.Terminate();
		mPixelShader.Terminate();
		mVertexShader.Terminate();
	}

	void SimpleDrawImpl::AddLine(const Vector3& v0, const Vector3& v1, const Color& color)
	{
		if (mLineVertexCount + 2 <= mMaxVertexCount)
		{
			mLineVerticies[mLineVertexCount++] = VertexPC{ v0,color };
			mLineVerticies[mLineVertexCount++] = VertexPC{ v1,color };
		}
	}

	void SimpleDrawImpl::AddFace(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Color& color)
	{
		if (mLineVertexCount + 3 <= mMaxVertexCount)
		{
			mLineVerticies[mFaceVertexCount++] = VertexPC{ v0,color };
			mLineVerticies[mFaceVertexCount++] = VertexPC{ v1,color };
			mLineVerticies[mFaceVertexCount++] = VertexPC{ v2,color };
		}
	}

	void SimpleDrawImpl::Render(const Camera& camera)
	{

		auto matView = camera.GetViewMatrix();
		auto matProj = camera.GetProjectionMatrix();
		auto transofrm = Transpose(matView * matProj);
		mConstantBuffer.Update(&transofrm);
		mConstantBuffer.BindVS(0);

		mVertexShader.Bind();
		mPixelShader.Bind();

		mAlphaBlendState.Set();

		mMeshBuffer.Update(mLineVerticies.get(), mLineVertexCount);
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
		mMeshBuffer.Render();

		mMeshBuffer.Update(mFaceVerticies.get(), mFaceVertexCount);
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Triangles);
		mMeshBuffer.Render();

		BlendState::ClearState();

		mLineVertexCount = 0;
		mFaceVertexCount = 0;

	}

	std::unique_ptr < SimpleDrawImpl> sInstance;
}

void SimpleDraw::StaticInitialize(uint32_t maxVertexCount)
{
    sInstance = std::make_unique<SimpleDrawImpl>();
    sInstance->Initialize(maxVertexCount);
}



void SimpleDraw::StaticTerminate()
{
    sInstance->Terminate();
    sInstance.reset();
}



void SimpleDraw::AddLine(const Vector3& v0, const Vector3& v1, const Color& color)
{
    sInstance->AddLine(v0, v1, color);
}



void SimpleDraw::AddFace(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Color& color)
{
    sInstance->AddFace(v0, v1, v2, color);
}



void SimpleDraw::AddAABB(const Vector3& min, const Vector3& max, const Color& color)
{
    AddAABB(min.x, min.y, min.z, max.x, max.y, max.z, color);
}



void SimpleDraw::AddAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color)
{
    const Vector3 topRightF = { maxX, maxY, minZ };
    const Vector3 botRightF = { maxX, minY, minZ };
    const Vector3 topLeftF = { minX, maxY, minZ };
    const Vector3 botLeftF = { minX, minY, minZ };



    const Vector3 topRightB = { maxX, maxY, maxZ };
    const Vector3 botRightB = { maxX, minY, maxZ };
    const Vector3 topLeftB = { minX, maxY, maxZ };
    const Vector3 botLeftB = { minX, minY, maxZ };



    // Front
    SimpleDraw::AddLine(topRightF, botRightF, color);
    SimpleDraw::AddLine(botRightF, botLeftF, color);
    SimpleDraw::AddLine(botLeftF, topLeftF, color);
    SimpleDraw::AddLine(topLeftF, topRightF, color);



    // Back
    SimpleDraw::AddLine(topRightB, botRightB, color);
    SimpleDraw::AddLine(botRightB, botLeftB, color);
    SimpleDraw::AddLine(botLeftB, topLeftB, color);
    SimpleDraw::AddLine(topLeftB, topRightB, color);



    // Top
    SimpleDraw::AddLine(topRightB, topRightF, color);
    SimpleDraw::AddLine(topLeftF, topLeftB, color);



    // Bottom
    SimpleDraw::AddLine(botRightB, botRightF, color);
    SimpleDraw::AddLine(botLeftF, botLeftB, color);
}



void SimpleDraw::AddFilledAABB(const Vector3& min, const Vector3& max, const Color& color)
{
    AddFilledAABB(min.x, min.y, min.z, max.x, max.y, max.z, color);
}



void SimpleDraw::AddFilledAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color)
{
    const Vector3 topRightF = { maxX, maxY, minZ };
    const Vector3 botRightF = { maxX, minY, minZ };
    const Vector3 topLeftF = { minX, maxY, minZ };
    const Vector3 botLeftF = { minX, minY, minZ };



    const Vector3 topRightB = { maxX, maxY, maxZ };
    const Vector3 botRightB = { maxX, minY, maxZ };
    const Vector3 topLeftB = { minX, maxY, maxZ };
    const Vector3 botLeftB = { minX, minY, maxZ };



    // Front
    SimpleDraw::AddFace(topRightF, botRightF, botLeftF, color);
    SimpleDraw::AddFace(botLeftF, topLeftF, topRightF, color);



    // Back
    SimpleDraw::AddFace(topRightB, botLeftB, botRightB, color);
    SimpleDraw::AddFace(botLeftB, topRightB, topLeftB, color);



    // Top
    SimpleDraw::AddFace(topRightB, topRightF, topLeftF, color);
    SimpleDraw::AddFace(topLeftF, topLeftB, topRightB, color);



    // Bot
    SimpleDraw::AddFace(botRightB, botLeftF, botRightF, color);
    SimpleDraw::AddFace(botLeftF, botRightB, botLeftB, color);



    // Right
    SimpleDraw::AddFace(topRightB, botRightB, botRightF, color);
    SimpleDraw::AddFace(botRightF, topRightF, topRightB, color);



    // Left
    SimpleDraw::AddFace(topLeftF, botLeftF, botLeftB, color);
    SimpleDraw::AddFace(botLeftB, topLeftB, topLeftF, color);
}



void SimpleDraw::AddSphere(int slices, int rings, float radius, const Color& color)
{
    Vector3 v0 = Vector3::Zero();
    Vector3 v1 = Vector3::Zero();

    // Add Side Vertices
    for (int ring = 0; ring < rings; ++ring)
    {
        const float r = static_cast<float>(ring);
        const float phi = r * (NULLEngine::EMath::kPi / (rings - 1));



        for (int slice = 0; slice < slices + 1; ++slice)
        {
            float s0 = static_cast<float>(slice);
            float rotation0 = (s0 / (slices)) * NULLEngine::EMath::kTwoPi;
            float s1 = static_cast<float>(slice + 1);
            float rotation1 = (s1 / (slices)) * NULLEngine::EMath::kTwoPi;



            v0 =
            {
                radius * sin(rotation0) * sin(phi),
                radius * cos(phi) + radius,
                radius * cos(rotation0) * sin(phi),
            };



            v1 =
            {
                radius * sin(rotation1) * sin(phi),
                radius * cos(phi) + radius,
                radius * cos(rotation1) * sin(phi),
            };



            SimpleDraw::AddLine(v0, v1, color);



            v0 =
            {
                radius * cos(phi),
                radius * cos(rotation0) * sin(phi) + radius,
                radius * sin(rotation0) * sin(phi),
            };



            v1 =
            {
                radius * cos(phi),
                radius * cos(rotation1) * sin(phi) + radius,
                radius * sin(rotation1) * sin(phi),
            };



            SimpleDraw::AddLine(v0, v1, color);
        }
    }
}



void SimpleDraw::AddGroundPlane(float size, const Color& color)
{
    const float halfSize = size * 0.5f;
    for (int i = 0; i <= size; ++i)
    {
        SimpleDraw::AddLine({ i - halfSize, 0.0f, -halfSize }, { i - halfSize, 0.0f, halfSize }, color);
        SimpleDraw::AddLine({ -halfSize, 0.0f, i - halfSize }, { halfSize, 0.0f, i - halfSize }, color);
    }
}



void SimpleDraw::AddTransform(const Matrix4& matrix)
{
    const Vector3 Side = { matrix._11, matrix._12, matrix._13 };
    const Vector3 Up = { matrix._21, matrix._22, matrix._23 };
    const Vector3 Look = { matrix._31, matrix._32, matrix._33 };
    const Vector3 Transform = { matrix._41, matrix._42, matrix._43 };



    SimpleDraw::AddLine(Transform, Transform + Side, Colors::Red);
    SimpleDraw::AddLine(Transform, Transform + Up, Colors::Green);
    SimpleDraw::AddLine(Transform, Transform + Look, Colors::Blue);
}



void SimpleDraw::Render(const Camera& camera)
{
    sInstance->Render(camera);
}