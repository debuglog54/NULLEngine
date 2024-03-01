#pragma once
#include "Colors.h"

using namespace NULLEngine::EMath;

namespace NULLEngine::Graphics
{
	class Camera;

	namespace SimpleDraw
	{
		void StaticInitialize(uint32_t maxVertexCount);
		void StaticTerminate();

		void AddLine(const Vector3& v0, const Vector3& v1, const Color& color);
		void AddFace(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Color& color);

		void AddAABB(const Vector3& min, const Vector3& max, const Color& color);
		void AddAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color);

		void AddFilledAABB(const Vector3& min, const Vector3& max, const Color& color);
		void AddFilledAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color);

		void AddSphere(int slices, int ring, float radius, const Color& color);

		void AddGroundPlane(float size, const Color& color);

		void AddTransform(const Matrix4& m);

		void Render(const Camera& camera);
	}
}