#pragma once
#include "MeshTypes.h"

namespace NULLEngine::Graphics
{
    class MeshBuilder
    {
    public:
        // Position and Color
        static MeshPC CreateCubePC(float size, Color color);
        static MeshPC CreateRectanglePC(float width, float height, float depth);
        static MeshPC CreatePlanePC(int numRows, int numColoms, float spacing);
        static MeshPC CreateCylinderPC(uint32_t slices, uint32_t rings);
        static MeshPC CreateSpherePC(uint32_t slices, uint32_t rings, float radius);

        // Position and UV
        static MeshPX CreateCubePX(float size);
        static MeshPX CreateSpherePX(uint32_t slices, uint32_t rings, float radius);

        static MeshPX CreateScreenQuad();

        // Postion Normal Tangent UV
        static Mesh CreateSphere(uint32_t slices, uint32_t rings, float radius);
        static Mesh CreatePlane(int numRows, int numColoms, float spacing);
        static Mesh CreateRevertedPlane(int numRows, int numColoms, float spacing);
        static Mesh CreateCube(float size);
    };
}