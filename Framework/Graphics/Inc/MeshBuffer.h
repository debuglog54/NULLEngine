#pragma once

namespace NULLEngine::Graphics
{
	class MeshBuffer final  // Final - means no inheritance allowed
	{
	public:
		enum class Topology
		{
			Points,
			Lines,
			Triangles
		};


		template<class VertexType>
		void Initialize(const std::vector<VertexType>& vertices)
		{
			Initialize(vertices.data(), static_cast<uint32_t>(sizeof(VertexType)), static_cast<uint32_t>(vertices.size()));
		}

		template<class MeshType>
		void Initialize(const MeshType& mesh)
		{
			Initialize(mesh.vertices.data(),
				static_cast<uint32_t>(sizeof(MeshType::VertexType)),
				static_cast<uint32_t>(mesh.vertices.size()),
				mesh.indices.data(),
				static_cast<uint32_t>(mesh.indices.size()));
		}

		void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount);
		void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount);
		void Terminate();
		void Render() const;
		
		void Update(const void* vertices, uint32_t vertexCount);
		void SetTopology(Topology topology);
	private:
		void CreateVertexBuffer(const void* vertices, uint32_t vertexSize, uint32_t vertexCount);
		void CreateIndexBuffer(const uint32_t* indices, uint32_t indexCount);

		D3D11_PRIMITIVE_TOPOLOGY mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		ID3D11Buffer* mVertexBuffer = nullptr;
		ID3D11Buffer* mIndexBuffer = nullptr;
		uint32_t mVertexSize = 0;
		uint32_t mVertexCount = 0;
		uint32_t mIndexCount = 0;
	};
}