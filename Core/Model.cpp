#include "Model.hpp"
#include <string>
#include <Engine/Core/FileUtils.hpp>
#include <Engine/Core/Vertex_PNCU.hpp>
#include <Engine/Math/IntVec3.hpp>

#include <Engine/Renderer/Renderer.hpp>
#include <d3d11.h>
#include <Engine/Math/VertexUtils.hpp>

Mesh::~Mesh()
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

bool Mesh::ImportFromOBJFile(char const* m_fileName, MeshImportOptions& importOptions)
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
//for (i = 0; i < objLines.size(); i++)
//{
//	if (objLines[i][0] == 'v' && objLines[i][1] == ' ')
//	{
//		readStartPoint = 1;
//		for (m = 1; m < objLines[i].size(); m++)
//		{
//			if (objLines[i][m] == ' ')
//			{
//				readStartPoint++;
//			}
//			else
//			{
//				break;
//			}
//		}
//
//		for (int j = readStartPoint; j < objLines[i].size(); j++)
//		{
//			vertString.push_back(objLines[i][j]);
//		}
//		vec3Value.SetFromTextWithSpace(vertString.c_str());
//		vertexes.push_back(vec3Value);
//		vertString.clear();
//	}
//
//	if (objLines[i][0] == 'v' && objLines[i][1] == 'n')
//	{
//		readStartPoint = 2;
//		for (m = 2; m < objLines[i].size(); m++)
//		{
//			if (objLines[i][m] == ' ')
//			{
//				readStartPoint++;
//			}
//			else
//			{
//				break;
//			}
//		}
//		for (int j = readStartPoint ; j < objLines[i].size(); j++)
//		{
//			vertString.push_back(objLines[i][j]);
//		}
//		vec3Value.SetFromTextWithSpace(vertString.c_str());
//		normals.push_back(vec3Value);
//		vertString.clear();
//	}
//
//	if (objLines[i][0] == 'v' && objLines[i][1] == 't')
//	{
//		readStartPoint = 2;
//		for (m = 2; m < objLines[i].size(); m++)
//		{
//			if (objLines[i][m] == ' ')
//			{
//				readStartPoint++;
//			}
//			else
//			{
//				break;
//			}
//		}
//
//		for (int j = readStartPoint; j < objLines[i].size(); j++)
//		{
//			vertString.push_back(objLines[i][j]);
//		}
//		if (SplitStringOnDelimiter(vertString, ' ').size() == 3)
//		{
//			vec3Value.SetFromTextWithSpace(vertString.c_str());
//			uvs.push_back(Vec2(vec3Value));
//		}
//		else
//		{	
//			Vec2 uvVec2;
//			uvVec2.SetFromText(vertString.c_str(),' ');
//			uvs.push_back(uvVec2);
//		}
//		vertString.clear();
//	}
//
//	if (objLines[i][0] == 'f' && objLines[i][1] == ' ')
//	{
//		Strings spacesInFace = SplitStringOnDelimiter(objLines[i], ' ');
//
//		//-----------------TRIANGLE------------------------
//		if (spacesInFace.size() == 4)
//		{
//			for (int j = 1; j < spacesInFace.size(); j++)
//			{
//				Strings lineString = SplitStringOnDelimiter(spacesInFace[j].c_str(), '/');
//				
//				if (lineString.size() == 1)
//				{
//					v.m_position = vertexes[std::stoi(lineString[0].c_str())];
//					m_meshVerts.push_back(v);
//					v.m_color = Rgba8::WHITE;
//					indices.push_back((int)m_meshVerts.size() - 1);
//					continue;
//				}
//				else if (lineString.size() == 2)
//				{
//					intVec2Value.SetFromTextWithDelimiter(spacesInFace[j].c_str(), '/');
//					v.m_position = vertexes[intVec2Value.x - 1];
//					v.m_uvTexCoords = uvs[intVec2Value.y - 1];
//					m_meshVerts.push_back(v);
//					indices.push_back((int)m_meshVerts.size() - 1);
//					v.m_color = Rgba8::WHITE;
//					continue;
//				}
//				else if (lineString.size() == 3)
//				{
//					intVec3Value.SetFromTextWithDelimiter(spacesInFace[j].c_str(), '/');
//					v.m_position = vertexes[intVec3Value.x - 1];
//					if (intVec3Value.y - 1 != -1)
//					{
//						v.m_uvTexCoords = uvs[intVec3Value.y - 1];
//					}
//					v.m_normal = normals[intVec3Value.z - 1];
//					m_meshVerts.push_back(v);
//					indices.push_back((int)m_meshVerts.size() - 1);
//					v.m_color = Rgba8::WHITE;
//					continue;
//				}
//				m_meshVerts.push_back(v);
//			}
//
//		}
//		else if (spacesInFace.size() == 5 && spacesInFace[spacesInFace.size() - 1] != "\r" )
//		{
//			for (int j = 1; j < spacesInFace.size(); j++)
//			{
//				Strings lineString = SplitStringOnDelimiter(spacesInFace[j].c_str(), '/');
//				intVec3Value.SetFromTextWithDelimiter(spacesInFace[j].c_str(), '/');
//				v.m_position = vertexes[intVec3Value.x - 1];
//				v.m_uvTexCoords = uvs[intVec3Value.y - 1];
//				v.m_normal = normals[intVec3Value.z - 1];
//				v.m_color = Rgba8::WHITE;
//				m_meshVerts.push_back(v);
//				indices.push_back((int)m_meshVerts.size() - 1);
//			}
//
//		}
//		if (spacesInFace.size() == 5 && spacesInFace[spacesInFace.size() - 1] == "\r")
//		{
//			for (int j = 1; j < spacesInFace.size() - 1; j++)
//			{
//				Strings lineString = SplitStringOnDelimiter(spacesInFace[j].c_str(), '/');
//				intVec3Value.SetFromTextWithDelimiter(spacesInFace[j].c_str(), '/');
//				v.m_position = vertexes[intVec3Value.x - 1];
//				v.m_uvTexCoords = uvs[intVec3Value.y - 1];
//				v.m_normal = normals[intVec3Value.z - 1];
//				v.m_color = Rgba8::WHITE;
//				m_meshVerts.push_back(v);
//				indices.push_back((int)m_meshVerts.size() - 1);
//			}
//
//		}
//		else if (spacesInFace.size() == 6 && spacesInFace[spacesInFace.size()-1] == "\r")
//		{
//			for (int k = 1; k < spacesInFace.size() - 2; k++)
//			{
//				intVec3Value.SetFromTextWithDelimiter(spacesInFace[k].c_str(), '/');
//				v.m_position = vertexes[intVec3Value.x - 1];
//				v.m_uvTexCoords = uvs[intVec3Value.y - 1];
//				v.m_normal = normals[intVec3Value.z - 1];
//				v.m_color = Rgba8::WHITE;
//				m_meshVerts.push_back(v);
//				indices.push_back((int)m_meshVerts.size() - 1);
//			}
//
//			//-----------------SPLITTING QUAD INTO TWO TRIANGLES------------------------
//			for (int l = 1; l < 5; l++)
//			{
//				if (l != 2)
//				{
//					intVec3Value.SetFromTextWithDelimiter(spacesInFace[l].c_str(), '/');
//					v.m_position = vertexes[intVec3Value.x - 1];
//					v.m_uvTexCoords = uvs[intVec3Value.y - 1];
//					v.m_normal = normals[intVec3Value.z - 1];
//					v.m_color = Rgba8::WHITE;
//					m_meshVerts.push_back(v);
//					indices.push_back((int)m_meshVerts.size() - 1);
//				}
//
//			}
//		}
//	}
//}


std::string bufferString;
Strings entries;
std::vector<Vec3> positions;
//std::vector<Vec3> normals;
//std::vector<Vec2> uvs;
std::vector<std::string> faces;
std::vector<int> m_meshIndices;

FileReadToString(bufferString, m_fileName);
entries = SplitStringOnDelimiter(bufferString, '\n');

for (int i = 0; i < entries.size(); i++)
{
		Strings tokens = SplitStringOnDelimiter(entries[i], ' ');
		//found positions
		if (tokens[0] == "v")
		{
			Vec3 pos;
			pos.SetFromText(MakeValuesStringFromTokens(tokens).c_str());
			positions.push_back(pos);
		}
		//found normal
		else if (tokens[0] == "vn")
		{
			Vec3 normal;
			normal.SetFromText(MakeValuesStringFromTokens(tokens).c_str());
			normals.push_back(normal);
		}
		//found uv
		else if (tokens[0] == "vt")
		{
			Vec2 uv;
			uv.SetFromText(MakeValuesStringFromTokens(tokens).c_str());
			uvs.push_back(uv);
		}
		//found face
		else if (tokens[0] == "f")
		{
			int numberOfClusters = 0;
			int baseIndex = (int)m_meshVerts.size();
			for (int j = 0; j < tokens.size(); j++)
			{
				if (tokens[j].find('/') != std::string::npos)
				{
					numberOfClusters++;
					Strings listIndices = SplitStringOnDelimiter(tokens[j], '/');
					Vertex_PNCU vert;
					vert.m_position = positions[atoi(listIndices[0].c_str()) - 1];	//subtracting by 1 because the indices in the obj file start from 1 (and not 0)
					vert.m_color = Rgba8::WHITE;
					if (listIndices.size() > 1 && listIndices[1] != "")
					{
						vert.m_uvTexCoords = uvs[atoi(listIndices[1].c_str()) - 1];
					}
					if (listIndices.size() > 2)
					{
						vert.m_normal = normals[atoi(listIndices[2].c_str()) - 1];
					}

					m_meshVerts.push_back(vert);
				}
			}

			//add indices for index buffer
			if (numberOfClusters > 3)
			{
				//if there were 4 clusters, mean it was a quad
				m_meshIndices.push_back(baseIndex + 0);
				m_meshIndices.push_back(baseIndex + 1);
				m_meshIndices.push_back(baseIndex + 2);
				m_meshIndices.push_back(baseIndex + 0);
				m_meshIndices.push_back(baseIndex + 2);
				m_meshIndices.push_back(baseIndex + 3);
			}
			else
			{
				//only 3 clusters, means it was a triangle
				m_meshIndices.push_back(baseIndex + 0);
				m_meshIndices.push_back(baseIndex + 1);
				m_meshIndices.push_back(baseIndex + 2);
			}
		}
	}

	m_cpuMesh = new CPUMesh(m_meshVerts, m_meshIndices);
	m_importOptions = importOptions;
	return false;
}


std::string Mesh::MakeValuesStringFromTokens(const Strings& tokens)
{
	std::string valueString;
	//always ignore the first token, since it's the value type identifier token
	for (int i = 1; i < tokens.size(); i++)
	{
		//if token is non empty
		if (tokens[i].size() > 0)
		{
			valueString.append(tokens[i]);
			if (i != tokens.size() - 1)
				valueString.append(",");
		}
	}

	return valueString;
}

bool Mesh::Save()
{
	std::string saveFile = "Data/Models/" + m_modelName + "/" + m_modelName + ".AlbusSave";
	FileWriteFromBuffer(m_cpuMesh->m_vertices, saveFile);
	return false;
}

bool Mesh::Load()
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

bool Mesh::Load(std::string fileName)
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
void Mesh::ApplyMeshOptions()
{
	Mat44 scaleMatrix = Mat44::CreateUniformScale3D(m_importOptions.m_scale);
	m_importOptions.m_transform.Append(scaleMatrix);
	TransformVertexArrayUsingMatrix3D((int)m_cpuMesh->m_vertices.size(), m_cpuMesh->m_vertices, m_importOptions.m_transform);
	TransformVertexArrayUsingMatrix3D((int)m_cpuMesh->m_verticesWithTangent.size(), m_cpuMesh->m_verticesWithTangent, m_importOptions.m_transform);

}

void Mesh::ApplyInvertVTexture()
{
	for (int i = 0; i < (int)m_cpuMesh->m_vertices.size(); i++)
	{
		m_cpuMesh->m_vertices[i].m_uvTexCoords.y = 1 - m_cpuMesh->m_vertices[i].m_uvTexCoords.y;
	}
}
void Mesh::ApplyTransform(Mat44 const& transform)
{
	m_importOptions.m_transform = transform;
}


void Mesh::ReverseWindingOrder()
{
	
}

void Mesh::GetTransformedVertices(Vec3& position, std::vector<Vertex_PNCUTB>& vertices)
{
	vertices.clear();
	Mat44 transformMatrix = Mat44::CreateTranslation3D(position);
	for (int i = 0; i < m_cpuMesh->m_verticesWithTangent.size(); i++)
	{
		vertices.push_back(m_cpuMesh->m_verticesWithTangent[i]);
	}

	TransformVertexArrayUsingMatrix3D((int)vertices.size(), vertices, transformMatrix);
}

bool Mesh::UpdateFromBuilder(Mesh const& builder)
{
	UNUSED((void) builder);
	return false;
}

std::string Mesh::GetFilePath()
{
	return m_filePath;
}

CPUMesh::CPUMesh(std::vector<Vertex_PNCU>& vertices, std::vector<int>& indices)
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

	//----Calculating Tangent and Bitangent -----------
	for (int i = 0; i < m_verticesWithTangent.size() -2; i++)
	{
		Vec3 deltaPos1 = m_verticesWithTangent[i + 1].m_position - m_verticesWithTangent[i].m_position;
		Vec3 deltaPos2 = m_verticesWithTangent[i+2].m_position - m_verticesWithTangent[i].m_position;

		Vec2 deltaUV1 = m_verticesWithTangent[i+1].m_uvTexCoords - m_verticesWithTangent[i].m_uvTexCoords;
		Vec2 deltaUV2 = m_verticesWithTangent[i+2].m_uvTexCoords - m_verticesWithTangent[i].m_uvTexCoords;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		Vec3 tangent = (deltaPos1 * deltaUV2.x - deltaPos2 * deltaUV1.x) * r;
		Vec3 bitangent = (deltaPos2 * deltaUV1.y - deltaPos1 * deltaUV2.y) * r;

		m_verticesWithTangent[i].m_tangent = tangent.GetNormalized();
		m_verticesWithTangent[i].m_bitangent = bitangent.GetNormalized();
	}

	int vertexCount = (int)m_verticesWithTangent.size();
	int totalTriangles = vertexCount /3;

	//Vec3* tangent = new Vec3[vertexCount * 2];
	/*Vec3* bitangent = tangent + vertexCount;*/

	for (int i = 0; i < totalTriangles; i++)
	{
		int triangleCount = i * 3;
		Vec3 v1  = m_verticesWithTangent[triangleCount ].m_position;
		Vec3 v2  = m_verticesWithTangent[triangleCount + 1].m_position;
		Vec3 v3  = m_verticesWithTangent[triangleCount + 2].m_position;

		Vec2 w1 = m_verticesWithTangent[triangleCount].m_uvTexCoords;
		Vec2 w2 = m_verticesWithTangent[triangleCount + 1].m_uvTexCoords;
		Vec2 w3 = m_verticesWithTangent[triangleCount + 2].m_uvTexCoords;

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;

		float r = 1.0F / (s1 * t2 - s2 * t1);
		Vec3 tang = Vec3((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r);
		Vec3 bitang = Vec3((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r);

		m_verticesWithTangent[triangleCount].m_tangent = tang.GetNormalized();
		m_verticesWithTangent[triangleCount+1].m_tangent = tang.GetNormalized();
		m_verticesWithTangent[triangleCount+2 ].m_tangent = tang.GetNormalized();

		m_verticesWithTangent[triangleCount].m_bitangent = bitang.GetNormalized();
		m_verticesWithTangent[triangleCount + 1].m_bitangent = bitang.GetNormalized();
		m_verticesWithTangent[triangleCount + 2].m_bitangent = bitang.GetNormalized();
	}
	
}


