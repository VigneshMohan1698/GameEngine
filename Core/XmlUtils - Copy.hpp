#pragma once
#include <vector>
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Renderer/VertexBuffer.hpp"
#include "Engine/Renderer/IndexBuffer.hpp"
#include "Engine/Math/Mat44.hpp"

class ID3D11InputLayout;

struct MeshImportOptions
{
	Mat44 m_transform;
	float m_scale = 1.0f;
	bool  m_reverseWindingOrder = false;
	bool  m_invertV = false;
};

class CPUMesh
{
	public:
	//---------------------CONSTRUCTOR---------------------------
		CPUMesh(){};
		~CPUMesh();

		CPUMesh(std::vector<Vertex_PCU> vertices, std::vector<int> indices);

	public:
		std::vector<Vertex_PCU> m_vertices;
		std::vector<int> m_indices;
};

class GPUMesh
{
	public:
		//---------------------CONSTRUCTOR---------------------------
		GPUMesh(){};
		~GPUMesh();

		GPUMesh(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, ID3D11InputLayout* inputLayoutForVertex_PCU);
		GPUMesh(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, ID3D11InputLayout* inputLayoutForVertex_PCU, bool	useIndices, int	elementCount);

		VertexBuffer* m_vertexBuffer = nullptr;
		IndexBuffer*  m_indexBuffer = nullptr;
		ID3D11InputLayout* m_inputLayoutForVertex_PCU = nullptr;

		//-------------INFORMATION FOR DRAW CALL
		bool		  m_useIndices = false;
		int			  m_elementCount = 0;
};

class MeshBuilder
{
	public:
		bool	 ImportFromOBJFile(char const* m_fileName, MeshImportOptions& importOptions);
		bool     Save(char const* m_fileName);
		bool     Load(char const* m_fileName);

		void	 ApplyTransform(Mat44 const& transform);
		void	 ReverseWindingOrder();

		bool	 UpdateFromBuilder(MeshBuilder const& builder);
	public:
	 char const* m_fileName = "";

};