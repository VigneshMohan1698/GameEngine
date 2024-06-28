#pragma once
#include <vector>
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/Vertex_PNCU.hpp"
#include "Engine/Core/Vertex_PNCUTB.hpp"
#include "Engine/Renderer/VertexBuffer.hpp"
#include "Engine/Renderer/IndexBuffer.hpp"
#include <Engine/Core/StringUtils.hpp>
#include "Engine/Math/Mat44.hpp"
#include <string>
struct ID3D11InputLayout;

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
		CPUMesh() {};
		~CPUMesh();

		CPUMesh(std::vector<Vertex_PCU> vertices, std::vector<int> indices);
		CPUMesh(std::vector<Vertex_PNCU>& vertices, std::vector<int>& indices);

	public:
		std::vector<Vertex_PNCU> m_vertices;
		std::vector<unsigned int> m_indices;
		std::vector<Vertex_PNCUTB> m_verticesWithTangent;
};

class GPUMesh
{
	public:
		//---------------------CONSTRUCTOR---------------------------
		GPUMesh(){};
		~GPUMesh();

		GPUMesh(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, ID3D11InputLayout* inputLayoutForVertex_PCU);
		GPUMesh(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, ID3D11InputLayout* inputLayoutForVertex_PCU, bool	useIndices, int	elementCount);

	public:
		VertexBuffer* m_vertexBuffer = nullptr;
		IndexBuffer*  m_indexBuffer = nullptr;
		ID3D11InputLayout* m_inputLayoutForVertex_PCU = nullptr;

		//-------------INFORMATION FOR DRAW CALL
		bool		  m_useIndices = false;
		int			  m_elementCount = 0;
		int			  m_startIndex = 0;
		int			  m_materialCount = 0;
};

class Mesh
{
	//---------------DESTRUCTOR-----------------------
	public:
	~Mesh();
	public:
		bool		ImportFromOBJFile(char const* m_fileName, MeshImportOptions& importOptions);
		std::string MakeValuesStringFromTokens(const Strings& tokens);
		bool		Save();
		bool		Load();
		bool		Load(std::string name);
		void		ApplyMeshOptions();
		void		ApplyInvertVTexture();
		void		ApplyTransform(Mat44 const& transform);
		void		ReverseWindingOrder();
		bool		UpdateFromBuilder(Mesh const& builder);

		void GetTransformedVertices(Vec3& position, std::vector<Vertex_PNCUTB>& vertices);
		std::string GetFilePath();
	public:
	 std::string		m_filePath = "";
	 std::string		m_texturePath = "";
	 std::string		m_modelName = "";
	 MeshImportOptions  m_importOptions;
	 CPUMesh*		    m_cpuMesh    = nullptr;
	 GPUMesh*			m_gpuMesh    = nullptr;

};