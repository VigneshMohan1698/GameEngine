#include "Model.hpp"
#include <string>
#include <Engine/Core/FileUtils.hpp>
#include <Engine/Core/Vertex_PNCU.hpp>
#include <Engine/Math/IntVec3.hpp>
#include <Engine/Core/StringUtils.hpp>
#include <Engine/Renderer/Renderer.hpp>
#include <d3d11.h>
#include <Engine/Math/VertexUtils.hpp>

MeshBuilder::~MeshBuilder()
{	
	m_cpuMesh->m_vertices.clear();
	m_cpuMesh->m_indices.clear();
	if (m_gpuMesh)
	{
		DX_SAFE_RELEASE(m_gpuMesh->m_vertexBuffer->m_buffer);
	}
	if (m_gpuMesh)
	{
		DX_SAFE_RELEASE(m_gpuMesh->m_indexBuffer->m_buffer);
	}
}

bool MeshBuilder::ImportFromOBJFile(char const* m_fileName, MeshImportOptions& importOptions)
{
	m_filePath = m_fileName;
	std::string modelSubstring = m_filePath.substr(12);
	m_modelName = modelSubstring.substr(0,4);
	std::string outObjFileValues;
	Vec3 vec3Value;
	IntVec3 intVec3Value;
	IntVec2 intVec2Value;
	Vertex_PNCU v;
	std::vector<Vec3> vertexes;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvs;
	std::string vertString;
	std::vector<Vertex_PNCU>			m_meshVerts;
	FileReadToString(outObjFileValues, m_fileName);

//-----------------GET INDIVIDIUAL LINES--------------------
std::vector<int> indices;
Strings objLines = SplitStringOnDelimiter(outObjFileValues, '\n');
int readStartPoint = 0;
int m,i;
//----------------ADD VERTS NORMALS AND UVS----------------
for (i = 0; i < objLines.size(); i++)
{
	if (objLines[i][0] == 'v' && objLines[i][1] == ' ')
	{
		readStartPoint = 1;
		for (m = 1; m < objLines[i].size(); m++)
		{
			if (objLines[i][m] == ' ')
			{
				readStartPoint++;
			}
			else
			{
				break;
			}
		}

		for (int j = readStartPoint; j < objLines[i].size(); j++)
		{
			vertString.push_back(objLines[i][j]);
		}
		vec3Value.SetFromTextWithSpace(vertString.c_str());
		vertexes.push_back(vec3Value);
		vertString.clear();
	}

	if (objLines[i][0] == 'v' && objLines[i][1] == 'n')
	{
		readStartPoint = 2;
		for (m = 2; m < objLines[i].size(); m++)
		{
			if (objLines[i][m] == ' ')
			{
				readStartPoint++;
			}
			else
			{
				break;
			}
		}
		for (int j = readStartPoint ; j < objLines[i].size(); j++)
		{
			vertString.push_back(objLines[i][j]);
		}
		vec3Value.SetFromTextWithSpace(vertString.c_str());
		normals.push_back(vec3Value);
		vertString.clear();
	}

	if (objLines[i][0] == 'v' && objLines[i][1] == 't')
	{
		readStartPoint = 2;
		for (m = 2; m < objLines[i].size(); m++)
		{
			if (objLines[i][m] == ' ')
			{
				readStartPoint++;
			}
			else
			{
				break;
			}
		}

		for (int j = readStartPoint; j < objLines[i].size(); j++)
		{
			vertString.push_back(objLines[i][j]);
		}
		if (SplitStringOnDelimiter(vertString, ' ').size() == 3)
		{
			vec3Value.SetFromTextWithSpace(vertString.c_str());
			uvs.push_back(Vec2(vec3Value));
		}
		else
		{	
			Vec2 uvVec2;
			uvVec2.SetFromText(vertString.c_str(),' ');
			uvs.push_back(uvVec2);
		}
		vertString.clear();
	}

	if (objLines[i][0] == 'f' && objLines[i][1] == ' ')
	{
		Strings spacesInFace = SplitStringOnDelimiter(objLines[i], ' ');

		//-----------------TRIANGLE------------------------
		if (spacesInFace.size() == 4)
		{
			for (int j = 1; j < spacesInFace.size(); j++)
			{
				Strings lineString = SplitStringOnDelimiter(spacesInFace[j].c_str(), '/');
				
				if (lineString.size() == 1)
				{
					v.m_position = vertexes[std::stoi(lineString[0].c_str())];
					m_meshVerts.push_back(v);
					v.m_color = Rgba8::WHITE;
					indices.push_back((int)m_meshVerts.size() - 1);
					continue;
				}
				else if (lineString.size() == 2)
				{
					intVec2Value.SetFromTextWithDelimiter(spacesInFace[j].c_str(), '/');
					v.m_position = vertexes[intVec2Value.x - 1];
					v.m_uvTexCoords = uvs[intVec2Value.y - 1];
					m_meshVerts.push_back(v);
					indices.push_back((int)m_meshVerts.size() - 1);
					v.m_color = Rgba8::WHITE;
					continue;
				}
				else if (lineString.size() == 3)
				{
					intVec3Value.SetFromTextWithDelimiter(spacesInFace[j].c_str(), '/');
					v.m_position = vertexes[intVec3Value.x - 1];
					if (intVec3Value.y - 1 != -1)
					{
						v.m_uvTexCoords = uvs[intVec3Value.y - 1];
					}
					v.m_normal = normals[intVec3Value.z - 1];
					m_meshVerts.push_back(v);
					indices.push_back((int)m_meshVerts.size() - 1);
					v.m_color = Rgba8::WHITE;
					continue;
				}
				m_meshVerts.push_back(v);
			}

		}
		else if (spacesInFace.size() == 5 && spacesInFace[spacesInFace.size() - 1] != "\r" )
		{
			for (int j = 1; j < spacesInFace.size(); j++)
			{
				Strings lineString = SplitStringOnDelimiter(spacesInFace[j].c_str(), '/');
				intVec3Value.SetFromTextWithDelimiter(spacesInFace[j].c_str(), '/');
				v.m_position = vertexes[intVec3Value.x - 1];
				v.m_uvTexCoords = uvs[intVec3Value.y - 1];
				v.m_normal = normals[intVec3Value.z - 1];
				v.m_color = Rgba8::WHITE;
				m_meshVerts.push_back(v);
				indices.push_back((int)m_meshVerts.size() - 1);
			}

		}
		if (spacesInFace.size() == 5 && spacesInFace[spacesInFace.size() - 1] == "\r")
		{
			for (int j = 1; j < spacesInFace.size() - 1; j++)
			{
				Strings lineString = SplitStringOnDelimiter(spacesInFace[j].c_str(), '/');
				intVec3Value.SetFromTextWithDelimiter(spacesInFace[j].c_str(), '/');
				v.m_position = vertexes[intVec3Value.x - 1];
				v.m_uvTexCoords = uvs[intVec3Value.y - 1];
				v.m_normal = normals[intVec3Value.z - 1];
				v.m_color = Rgba8::WHITE;
				m_meshVerts.push_back(v);
				indices.push_back((int)m_meshVerts.size() - 1);
			}

		}
		else if (spacesInFace.size() == 6 && spacesInFace[spacesInFace.size()-1] == "\r")
		{
			for (int k = 1; k < spacesInFace.size() - 2; k++)
			{
				intVec3Value.SetFromTextWithDelimiter(spacesInFace[k].c_str(), '/');
				v.m_position = vertexes[intVec3Value.x - 1];
				v.m_uvTexCoords = uvs[intVec3Value.y - 1];
				v.m_normal = normals[intVec3Value.z - 1];
				v.m_color = Rgba8::WHITE;
				m_meshVerts.push_back(v);
				indices.push_back((int)m_meshVerts.size() - 1);
			}

			//-----------------SPLITTING QUAD INTO TWO TRIANGLES------------------------
			for (int l = 1; l < 5; l++)
			{
				if (l != 2)
				{
					intVec3Value.SetFromTextWithDelimiter(spacesInFace[l].c_str(), '/');
					v.m_position = vertexes[intVec3Value.x - 1];
					v.m_uvTexCoords = uvs[intVec3Value.y - 1];
					v.m_normal = normals[intVec3Value.z - 1];
					v.m_color = Rgba8::WHITE;
					m_meshVerts.push_back(v);
					indices.push_back((int)m_meshVerts.size() - 1);
				}

			}
		}
	}
}

m_cpuMesh = new CPUMesh(m_meshVerts, indices);
m_importOptions = importOptions;
return false;
}

bool MeshBuilder::Save()
{
	std::string saveFile = "Data/Models/" + m_modelName + "/" + m_modelName + ".AlbusSave";
	FileWriteFromBuffer(m_cpuMesh->m_vertices, saveFile);
	return false;
}

bool MeshBuilder::Load()
{
	std::string saveFile = "Data/Models/" + m_modelName + ".AlbusSave";
	std::vector<Vertex_PNCU> outData;
	if (!FileExists(saveFile))
	{
		return  false;
	}
	FileReadToBuffer(outData, saveFile);
	for (int i = 0; i < (int)outData.size(); i++)
	{
		m_cpuMesh->m_vertices[i] = outData[i];
	}
	return false;
}

bool MeshBuilder::Load(std::string fileName)
{
	std::vector<Vertex_PNCU> outData;
	if (!FileExists(fileName))
	{
		return  false;
	}
	FileReadToBuffer(outData, fileName);
	m_cpuMesh = new CPUMesh();
	m_cpuMesh->m_vertices.reserve(outData.size());
	for (int i = 0; i < (int)outData.size(); i++)
	{
		m_cpuMesh->m_vertices.push_back(outData[i]);
		Vertex_PNCUTB tangentVertex;
		tangentVertex.m_position = outData[i].m_position;
		tangentVertex.m_color = Vec4(0,0,0,0);
		tangentVertex.m_tangent = Vec3(0,0,0);
		tangentVertex.m_normal = outData[i].m_normal;
		tangentVertex.m_uvTexCoords = outData[i].m_uvTexCoords;
		m_cpuMesh->m_verticesWithTangent.push_back(tangentVertex);
	}

	return false;
}
void MeshBuilder::ApplyMeshOptions()
{
	TransformVertexArrayUsingMatrix3D((int)m_cpuMesh->m_vertices.size(), m_cpuMesh->m_vertices, m_importOptions.m_transform);
	TransformVertexArrayUsingMatrix3D((int)m_cpuMesh->m_verticesWithTangent.size(), m_cpuMesh->m_verticesWithTangent, m_importOptions.m_transform);
}

void MeshBuilder::ApplyInvertVTexture()
{
	for (int i = 0; i < (int)m_cpuMesh->m_vertices.size(); i++)
	{
		m_cpuMesh->m_vertices[i].m_uvTexCoords.y = 1 - m_cpuMesh->m_vertices[i].m_uvTexCoords.y;
	}
}
void MeshBuilder::ApplyTransform(Mat44 const& transform)
{
	m_importOptions.m_transform = transform;
}


void MeshBuilder::ReverseWindingOrder()
{
	
}

bool MeshBuilder::UpdateFromBuilder(MeshBuilder const& builder)
{
	UNUSED((void) builder);
	return false;
}

std::string MeshBuilder::GetFilePath()
{
	return m_filePath;
}

CPUMesh::CPUMesh(std::vector<Vertex_PNCU> vertices, std::vector<int> indices)
{

	for (int i = 0; i < vertices.size(); i++)
	{
		m_vertices.push_back(vertices[i]);
		Vertex_PNCUTB vert = vertices[i];
		m_verticesWithTangent.push_back(vert);
	}
	for (int i = 0; i < indices.size(); i++)
	{
		m_indices.push_back(indices[i]);
	}
}
