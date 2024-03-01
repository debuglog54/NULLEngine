#include "Precompiled.h"
#include "..\Inc\MeshBuilder.h"

using namespace NULLEngine;
using namespace NULLEngine::Graphics;
using namespace NULLEngine::EMath;

namespace
{
	Color GetNextColor(int& index)
	{
		constexpr Color colorTable[] = {
		Colors::DarkMagenta,
		Colors::LightSkyBlue,
		Colors::Coral,
		Colors::Red,
		Colors::Green,
		Colors::Yellow,
		Colors::DarkTurquoise,
		};

		index = (index + 1) % std::size(colorTable);
		return colorTable[index];
	}

	void CreateCubeIndicies(std::vector<uint32_t>& indicies)
	{
		indicies = {
			// front face
			0,1,3,
			1,2,3,

			// back 
			5,4,7,
			5,7,6,

			// right 
			1,5,2,
			5,6,2,

			// left 
			4,0,3,
			4,3,7,

			// top 
			0,4,1,
			4,5,1,

			// bottom 
			6,7,3,
			6,3,2

		};
	}

	void CreateCapsIndicies(std::vector<uint32_t>& indicies, int slices, int topIndex)
	{
		for (int s = 0; s < slices; ++s)
		{
			uint32_t bottomIndex = topIndex + 1;

			// bottom triangle
			indicies.push_back(s);
			indicies.push_back(s + 1);
			indicies.push_back(bottomIndex);

			// top triangle
			int topRow = topIndex - slices - 1 + s;
			indicies.push_back(topIndex);
			indicies.push_back(topRow + 1);
			indicies.push_back(topRow);
		}
	}

	void CreateCapsPC(MeshPC& mesh, int slices, float halfHeight)
	{
		// Caps
		int index = rand() % 100;
		uint32_t topIndex = mesh.vertices.size();
		uint32_t bottomIndex = topIndex + 1;

		// top center
		mesh.vertices.push_back({ {0.0f, halfHeight, 0.0f}, GetNextColor(index) });
		// bottom center
		mesh.vertices.push_back({ {0.0f, -halfHeight, 0.0f}, GetNextColor(index) });
		CreateCapsIndicies(mesh.indices, slices, topIndex);
	}

	void CreatePlaneIndicies(std::vector<uint32_t>& indicies, int numRows, int numColums)
	{
		for (int r = 0; r < numRows; ++r)
		{
			for (int c = 0; c < numColums; ++c)
			{
				int i = r * (numColums + 1) + c;

				//triangle 1
				indicies.push_back(i);
				indicies.push_back(i + numColums + 1);
				indicies.push_back(i + 1);

				//triangle 2
				indicies.push_back(i + 1);
				indicies.push_back(i + numColums + 1);
				indicies.push_back(i + numColums + 2);
			}
		}
	}
	void CreateRevertPlaneIndicies(std::vector<uint32_t>& indicies, int numRows, int numColums)
	{
		for (int r = 0; r < numRows; ++r)
		{
			for (int c = 0; c < numColums; ++c)
			{
				int i = r * (numColums + 1) + c;

				// Triangle 1 (reversed order)
				indicies.push_back(i);
				indicies.push_back(i + 1);
				indicies.push_back(i + numColums + 2);

				// Triangle 2 (reversed order)
				indicies.push_back(i);
				indicies.push_back(i + numColums + 2);
				indicies.push_back(i + numColums + 1);
			}
		}

	}
}

MeshPC NULLEngine::Graphics::MeshBuilder::CreateCubePC(float size, Color color)
{
	MeshPC mesh;
	int index = rand() % 100;

	float hs = size * 0.5f;

	//front
	mesh.vertices.push_back({ {-hs, hs, -hs },GetNextColor(index) });
	mesh.vertices.push_back({ {hs, hs, -hs },GetNextColor(index) });
	mesh.vertices.push_back({ {hs, -hs, -hs },GetNextColor(index) });
	mesh.vertices.push_back({ {-hs, -hs, -hs },GetNextColor(index) });

	//back
	mesh.vertices.push_back({ {-hs,hs, hs },GetNextColor(index) });
	mesh.vertices.push_back({ {hs,hs, hs },GetNextColor(index) });
	mesh.vertices.push_back({ {hs,-hs, hs },GetNextColor(index) });
	mesh.vertices.push_back({ {-hs,-hs, hs },GetNextColor(index) });

	CreateCubeIndicies(mesh.indices);

	return mesh;
}

MeshPC NULLEngine::Graphics::MeshBuilder::CreateRectanglePC(float width, float height, float depth)
{
	MeshPC mesh;

	const float halfWidth = width * 0.5f;
	const float halfDepth = depth * 0.5f;
	const float halhfHeight = (width - 0.5f) * 0.5f;
	int colorIndex = 0;

	// Face
	mesh.vertices.push_back({ { -halfDepth, halfWidth, -halfDepth }, GetNextColor(colorIndex) });   // FTL  0
	mesh.vertices.push_back({ { halfDepth, halfWidth, -halfDepth }, GetNextColor(colorIndex) });    // FTR  1
	mesh.vertices.push_back({ { halfDepth, -halfWidth, -halfDepth }, GetNextColor(colorIndex) });   // FBR  2
	mesh.vertices.push_back({ { -halfDepth, -halfWidth, -halfDepth }, GetNextColor(colorIndex) });  // FBL  3

	// Back
	mesh.vertices.push_back({ { -halfDepth, halfWidth, halfDepth }, GetNextColor(colorIndex) });    // BTL  4
	mesh.vertices.push_back({ { halfDepth, halfWidth, halfDepth }, GetNextColor(colorIndex) });     // BTR  5
	mesh.vertices.push_back({ { halfDepth, -halfWidth, halfDepth }, GetNextColor(colorIndex) });    // BBR  6
	mesh.vertices.push_back({ { -halfDepth, -halfWidth, halfDepth }, GetNextColor(colorIndex) });   // BBL  7

	mesh.indices = {
		// Front Face
		0, 1, 3,
		1, 2, 3,

		// Back Face
		5, 4, 7,
		5, 7, 6,

		// Right Face
		1, 5, 2,
		5, 6, 2,

		// Left Face
		4, 0, 3,
		4, 3, 7,

		// Top Face
		0, 4, 1,
		4, 5, 1,

		// Bottom Face
		6, 7, 3,
		6, 3, 2
	};

	return mesh;
}

MeshPC NULLEngine::Graphics::MeshBuilder::CreatePlanePC(int numRows, int numColoms, float spacing)
{
	MeshPC mesh;
	int index = rand() % 100;

	const float halfPlaneWidth = static_cast<float>(numColoms) * spacing * 0.5f;
	const float halfPlaneHeight = static_cast<float>(numRows) * spacing * 0.5f;

	float x = -halfPlaneWidth;
	float y = -halfPlaneHeight;

	for (int r = 0; r <= numRows; ++r)
	{
		for (int c = 0; c <= numColoms; ++c)
		{
			mesh.vertices.push_back({ {x,y,0.0f}, GetNextColor(index) });
			x += spacing;
		}
		x = -halfPlaneWidth;
		y += spacing;
	}

	CreatePlaneIndicies(mesh.indices, numColoms, numRows);
	return mesh;
}

MeshPC NULLEngine::Graphics::MeshBuilder::CreateCylinderPC(uint32_t slices, uint32_t rings)
{
	MeshPC mesh;
	int index = rand() % 100;

	const float halfHeight = static_cast<float>(rings) * 0.5f;

	//Plane
	for (int r = 0; r <= rings; ++r)
	{
		float ring = static_cast<float>(r);
		// float phi = r * (EMath::kPi / static_cast<float>(rings));
		for (int s = 0; s < slices + 1; ++s)
		{
			float slice = static_cast<float>(s);
			float rotation = (slice / static_cast<float>(slices)) * EMath::kTwoPi;

			mesh.vertices.push_back({
				{sin(rotation), r - halfHeight, -cos(rotation)},
				GetNextColor(index)
				});
		}
	}

	CreatePlaneIndicies(mesh.indices, rings, slices);
	CreateCapsPC(mesh, slices, halfHeight);
	return mesh;
}

MeshPC NULLEngine::Graphics::MeshBuilder::CreateSpherePC(uint32_t slices, uint32_t rings, float radius)
{
	MeshPC mesh;
	int index = rand() % 100;

	//Plane
	for (int r = 0; r <= rings; ++r)
	{
		float ring = static_cast<float>(r);
		float phi = ring * (EMath::kPi / static_cast<float>(rings - 1));

		for (int s = 0; s < slices + 1; ++s)
		{
			float slice = static_cast<float>(s);
			float rotation = (slice / static_cast<float>(slices)) * EMath::kTwoPi;

			mesh.vertices.push_back({
				{radius * sin(rotation) * sin(phi),
				radius * cos(phi),
				radius * cos(rotation) * sin(phi)},
				GetNextColor(index)
				});
		}
	}

	CreatePlaneIndicies(mesh.indices, rings, slices);

	return mesh;
}

MeshPX MeshBuilder::CreateCubePX(float size)
{
	MeshPX mesh;
	int index = rand() % 100;

	float hs = size * 0.5f;

	//front
	mesh.vertices.push_back({ {-hs, hs, -hs },{0.25f,0.33f} });
	mesh.vertices.push_back({ {hs, hs, -hs },{0.5f,0.33f} });
	mesh.vertices.push_back({ {hs, -hs, -hs },{0.5f,0.66f} });
	mesh.vertices.push_back({ {-hs, -hs, -hs },{0.25f,0.66f} });

	//back
	mesh.vertices.push_back({ {-hs,hs, hs },{0.0f,0.33f} });
	mesh.vertices.push_back({ {hs,hs, hs },{0.0f,0.5f} });
	mesh.vertices.push_back({ {hs,-hs, hs },{0.5f,1.0f} });
	mesh.vertices.push_back({ {-hs,-hs, hs },{1.0f,1.0f} });

	CreateCubeIndicies(mesh.indices);
	return mesh;
}

MeshPX MeshBuilder::CreateSpherePX(uint32_t slices, uint32_t rings, float radius)
{
	MeshPX mesh;
	int index = rand() % 100;

	float uStep = 1.0f / static_cast<float>(slices);
	float vStep = 1.0f / static_cast<float>(rings);


	//Plane
	for (int r = 0; r <= rings; ++r)
	{
		float ring = static_cast<float>(r);
		float phi = ring * (EMath::kPi / static_cast<float>(rings - 1));

		for (int s = 0; s < slices + 1; ++s)
		{
			float slice = static_cast<float>(s);
			float rotation = (slice / static_cast<float>(slices)) * EMath::kTwoPi;

			float u = 1.0 - (uStep * slice);
			float v = vStep * ring;


			mesh.vertices.push_back({
				{radius * sin(rotation) * sin(phi),
				radius * cos(phi),
				radius * cos(rotation) * sin(phi)},
			   {u,v}
				});
		}
	}

	CreatePlaneIndicies(mesh.indices, rings, slices);

	return mesh;
}

MeshPX MeshBuilder::CreateScreenQuad()
{
	MeshPX mesh;
	mesh.vertices.push_back({ { -1.0f, -1.0f, 0.0f },	{ 0.0f,1.0f } });
	mesh.vertices.push_back({ { -1.0f, 1.0f,  0.0f },	{ 0.0f,0.0f } });
	mesh.vertices.push_back({ { 1.0f,  1.0f,  0.0f },	{ 1.0f,0.0f } });
	mesh.vertices.push_back({ { 1.0f, -1.0f,  0.0f },	{ 1.0f,1.0f } });
	mesh.indices = { 0,1,2,0,2,3 };
	return  mesh;
}

Mesh MeshBuilder::CreateSphere(uint32_t slices, uint32_t rings, float radius)
{
	Mesh mesh;
	int index = rand() % 100;

	float uStep = 1.0f / static_cast<float>(slices);
	float vStep = 1.0f / static_cast<float>(rings);

	//Plane
	for (int r = 0; r <= rings; ++r)
	{
		float ring = static_cast<float>(r);
		float phi = ring * (EMath::kPi / static_cast<float>(rings - 1));

		for (int s = 0; s < slices + 1; ++s)
		{
			float slice = static_cast<float>(s);
			float rotation = (slice / static_cast<float>(slices)) * EMath::kTwoPi;

			float u = 1.0 - (uStep * slice);
			float v = vStep * ring;

			float x = radius * sin(rotation) * sin(phi);
			float y = radius * cos(phi);
			float z = radius * cos(rotation) * sin(phi);

			const Vector3 pos = { x,y,z };
			const Vector3 normal = Normalize(pos);
			const Vector3 tan = Normalize({ -z,0.0f,x });

			mesh.vertices.push_back({ pos, normal, tan, {u,v} });

		}
	}
	CreatePlaneIndicies(mesh.indices, rings, slices);
	return mesh;
}

Mesh MeshBuilder::CreatePlane(int numRows, int numCols, float spacing)
{
	Mesh mesh;

	const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
	const float hph = static_cast<float>(numRows) * spacing * 0.5f;
	const float uInc = 1.0f / (hpw * 2.0f);
	const float vInc = 1.0f / (hph * 2.0f);

	float x = -hpw;
	float z = -hph;
	float u = 0.0f;
	float v = 1.0f;

	for (int r = 0; r <= numRows; ++r)
	{
		for (int c = 0; c <= numCols; ++c)
		{
			mesh.vertices.push_back({
				{x, 0.0f, z},
				{0.0f, 1.0f, 0.0f},
				{0.0f, 0.0f, 1.0f},
				{ u, v } });
			x += spacing;
			u += uInc;
		}
		u = 0.0f;
		v -= vInc;
		x = -hpw;
		z += spacing;
	}

	CreatePlaneIndicies(mesh.indices, numRows, numCols);

	return mesh;
}

Mesh MeshBuilder::CreateRevertedPlane(int numRows, int numCols, float spacing)
{
	Mesh mesh;

	const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
	const float hph = static_cast<float>(numRows) * spacing * 0.5f;
	const float uInc = 1.0f / (hpw * 2.0f);
	const float vInc = 1.0f / (hph * 2.0f);

	float x = -hpw;
	float z = -hph;
	float u = 0.0f;
	float v = 1.0f;

	for (int r = 0; r <= numRows; ++r)
	{
		for (int c = 0; c <= numCols; ++c)
		{
			mesh.vertices.push_back({
				{x, 0.0f, z},
				{0.0f, 1.0f, 0.0f},
				{0.0f, 0.0f, 1.0f},
				{ u, v } });
			x += spacing;
			u += uInc;
		}
		u = 0.0f;
		v -= vInc;
		x = -hpw;
		z += spacing;
	}

	CreateRevertPlaneIndicies(mesh.indices, numRows, numCols);

	return mesh;
}

Mesh MeshBuilder::CreateCube(float size)
{
	Mesh mesh;

	float hs = size * 0.5f;

	// FRONT
	mesh.vertices.push_back({ Vector3{  hs,  hs, -hs }, Vector3::ZAxis(), Vector3::XAxis(), Vector2(0.5f, 0.33f) }); // Top Right Close
	mesh.vertices.push_back({ Vector3{ -hs, -hs, -hs }, Vector3::ZAxis(), Vector3::XAxis(), Vector2(0.25f, 0.66f) }); // Bottom Left Close
	mesh.vertices.push_back({ Vector3{  hs, -hs, -hs }, Vector3::ZAxis(), Vector3::XAxis(), Vector2(0.5f, 0.66f) }); // Bottom Right Close

	mesh.vertices.push_back({ Vector3{  hs,  hs, -hs }, Vector3::ZAxis(), Vector3::XAxis(), Vector2(0.5f, 0.33f) }); // Top Right Close
	mesh.vertices.push_back({ Vector3{ -hs,  hs, -hs }, Vector3::ZAxis(), Vector3::XAxis(), Vector2(0.25f, 0.33f) }); // Top Left Close
	mesh.vertices.push_back({ Vector3{ -hs, -hs, -hs }, Vector3::ZAxis(), Vector3::XAxis(), Vector2(0.25f, 0.66f) }); // Bottom Left Close

	// BACK
	mesh.vertices.push_back({ Vector3{ -hs,  hs,  hs }, -Vector3::ZAxis(), -Vector3::XAxis(), Vector2(1.0f, 0.33f) }); // Top Left Far
	mesh.vertices.push_back({ Vector3{  hs, -hs,  hs }, -Vector3::ZAxis(), -Vector3::XAxis(), Vector2(0.75f, 0.66f) }); // Bottom Right Far
	mesh.vertices.push_back({ Vector3{ -hs, -hs,  hs }, -Vector3::ZAxis(), -Vector3::XAxis(), Vector2(1.0f, 0.66f) }); // Bottom Left Far

	mesh.vertices.push_back({ Vector3{ -hs,  hs,  hs }, -Vector3::ZAxis(), -Vector3::XAxis(), Vector2(1.0f, 0.33f) }); // Top Left Far
	mesh.vertices.push_back({ Vector3{  hs,  hs,  hs }, -Vector3::ZAxis(), -Vector3::XAxis(), Vector2(0.75f, 0.33f) }); // Top Right Far
	mesh.vertices.push_back({ Vector3{  hs, -hs,  hs }, -Vector3::ZAxis(), -Vector3::XAxis(), Vector2(0.75f, 0.66f) }); // Bottom Right Far

	// RIGHT
	mesh.vertices.push_back({ Vector3{  hs,  hs,  hs }, Vector3::XAxis(), Vector3::ZAxis(), Vector2(0.75f, 0.33f) }); // Top Right Far
	mesh.vertices.push_back({ Vector3{  hs, -hs, -hs }, Vector3::XAxis(), Vector3::ZAxis(), Vector2(0.5f, 0.66f) }); // Bottom Right Close
	mesh.vertices.push_back({ Vector3{  hs, -hs,  hs }, Vector3::XAxis(), Vector3::ZAxis(), Vector2(0.75f, 0.66f) }); // Bottom Right Far

	mesh.vertices.push_back({ Vector3{  hs,  hs,  hs }, Vector3::XAxis(), Vector3::ZAxis(), Vector2(0.75f, 0.33f) }); // Top Right Far
	mesh.vertices.push_back({ Vector3{  hs,  hs, -hs }, Vector3::XAxis(), Vector3::ZAxis(), Vector2(0.5f, 0.33f) }); // Top Right Close
	mesh.vertices.push_back({ Vector3{  hs, -hs, -hs }, Vector3::XAxis(), Vector3::ZAxis(), Vector2(0.5f, 0.66f) }); // Bottom Right Close

	// Left
	mesh.vertices.push_back({ Vector3{ -hs,  hs, -hs }, -Vector3::XAxis(), -Vector3::ZAxis(), Vector2(0.25f, 0.33f) }); // Top Left Close
	mesh.vertices.push_back({ Vector3{ -hs, -hs,  hs }, -Vector3::XAxis(), -Vector3::ZAxis(), Vector2(0.0f, 0.66f) }); // Bottom Left Far
	mesh.vertices.push_back({ Vector3{ -hs, -hs, -hs }, -Vector3::XAxis(), -Vector3::ZAxis(), Vector2(0.25f, 0.66f) }); // Bottom Left Close

	mesh.vertices.push_back({ Vector3{ -hs,  hs, -hs }, -Vector3::XAxis(), -Vector3::ZAxis(), Vector2(0.25f, 0.33f) }); // Top Left Close
	mesh.vertices.push_back({ Vector3{ -hs,  hs,  hs }, -Vector3::XAxis(), -Vector3::ZAxis(), Vector2(0.0f, 0.33f) }); // Top Left Far
	mesh.vertices.push_back({ Vector3{ -hs, -hs,  hs }, -Vector3::XAxis(), -Vector3::ZAxis(), Vector2(0.0f, 0.66f) }); // Bottom Left Far

	// Top
	mesh.vertices.push_back({ Vector3{  hs,  hs,  hs }, Vector3::YAxis(), Vector3::XAxis(), Vector2(0.5f,  0.0f) }); // Top Right Far
	mesh.vertices.push_back({ Vector3{ -hs,  hs, -hs }, Vector3::YAxis(), Vector3::XAxis(), Vector2(0.25f, 0.33f) }); // Top Left Close
	mesh.vertices.push_back({ Vector3{  hs,  hs, -hs }, Vector3::YAxis(), Vector3::XAxis(), Vector2(0.5f, 0.33f) }); // Top Right Close

	mesh.vertices.push_back({ Vector3{  hs,  hs,  hs }, Vector3::YAxis(), Vector3::XAxis(), Vector2(0.5f,  0.0f) }); // Top Right Far
	mesh.vertices.push_back({ Vector3{ -hs,  hs,  hs }, Vector3::YAxis(), Vector3::XAxis(), Vector2(0.25f,  0.0f) }); // Top Left Far
	mesh.vertices.push_back({ Vector3{ -hs,  hs, -hs }, Vector3::YAxis(), Vector3::XAxis(), Vector2(0.25f, 0.33f) }); // Top Left Close

	// Bottom
	mesh.vertices.push_back({ Vector3{  hs, -hs, -hs }, -Vector3::YAxis(), -Vector3::XAxis(), Vector2(0.5f, 0.66f) }); // Bottom Right Close
	mesh.vertices.push_back({ Vector3{ -hs, -hs,  hs }, -Vector3::YAxis(), -Vector3::XAxis(), Vector2(0.25f,  1.0f) }); // Bottom Left Far
	mesh.vertices.push_back({ Vector3{  hs, -hs,  hs }, -Vector3::YAxis(), -Vector3::XAxis(), Vector2(0.5f,  1.0f) }); // Bottom Right Far

	mesh.vertices.push_back({ Vector3{  hs, -hs, -hs }, -Vector3::YAxis(), -Vector3::XAxis(), Vector2(0.5f, 0.66f) }); // Bottom Right Close
	mesh.vertices.push_back({ Vector3{ -hs, -hs, -hs }, -Vector3::YAxis(), -Vector3::XAxis(), Vector2(0.25f, 0.66f) }); // Bottom Left Close
	mesh.vertices.push_back({ Vector3{ -hs, -hs,  hs }, -Vector3::YAxis(), -Vector3::XAxis(), Vector2(0.25f,  1.0f) }); // Bottom Left Far

	int i = 0;
	for (const auto& vertex : mesh.vertices)
	{
		mesh.indices.push_back(i);
		++i;
	}

	return mesh;
}