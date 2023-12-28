#include "VertexUtils.hpp"
#include "Engine/Math/VertexUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include <Engine/Core/Vertex_PNCUTB.hpp>

void TransformVertexArray3D(int vertices, Vertex_PCU* positionVertexArrays, float uniformScale, float rotationDegrees, const Vec3& translatePos)
{
	for (int vertexIndex = 0; vertexIndex < vertices; vertexIndex++)
	{
		TransformPosition3D(positionVertexArrays[vertexIndex].m_position, uniformScale, rotationDegrees, translatePos);
	}
}
void TransformVertexArray3D(int vertices, std::vector<Vertex_PCU>& positionVertexArrays, float uniformScale, float rotationDegrees, const Vec3& translatePos)
{
	for (int vertexIndex = 0; vertexIndex < vertices; vertexIndex++)
	{
		TransformPosition3D(positionVertexArrays[vertexIndex].m_position, uniformScale, rotationDegrees, translatePos);
	}
}
void TransformVertexArray3D(int vertices, std::vector<Vertex_PNCUTB>& positionVertexArrays, float uniformScale, float rotationDegrees, const Vec3& translatePos)
{
	for (int vertexIndex = 0; vertexIndex < vertices; vertexIndex++)
	{
		TransformPosition3D(positionVertexArrays[vertexIndex].m_position, uniformScale, rotationDegrees, translatePos);
	}
}

void TransformVertexArrayUsingMatrix3D(int vertices,std::vector<Vertex_PCU>& positionVertexArrays, Mat44 transformMatrix)
{
	for (int i = 0; i < vertices; i++)
	{
		positionVertexArrays[i].m_position = transformMatrix.TransformVectorQuantity3D(positionVertexArrays[i].m_position);
		//positionVertexArrays[i].m_position = transformMatrix.TransformPosition3D(positionVertexArrays[i].m_position);
	}
	
}

void TransformPNCUTBArrayUsingMatrix3D(int vertices, std::vector<Vertex_PNCUTB>& positionVertexArrays, Mat44 transformMatrix)
{
	for (int i = 0; i < vertices; i++)
	{
		positionVertexArrays[i].m_position = transformMatrix.TransformVectorQuantity3D(positionVertexArrays[i].m_position);
		positionVertexArrays[i].m_position = transformMatrix.TransformPosition3D(positionVertexArrays[i].m_position);
	}

}

void TransformVertexArrayUsingMatrix3D(int vertices, std::vector<Vertex_PNCU>& positionVertexArrays, Mat44 transformMatrix)
{
	for (int i = 0; i < vertices; i++)
	{
		positionVertexArrays[i].m_position = transformMatrix.TransformVectorQuantity3D(positionVertexArrays[i].m_position);
	}

}
void TransformVertexArrayUsingMatrix3D(int vertices, std::vector<Vertex_PNCUTB>& positionVertexArrays, Mat44 transformMatrix)
{
	for (int i = 0; i < vertices; i++)
	{
		//positionVertexArrays[i].m_position = transformMatrix.TransformVectorQuantity3D(positionVertexArrays[i].m_position);
		positionVertexArrays[i].m_position = transformMatrix.TransformPosition3D(positionVertexArrays[i].m_position);
		positionVertexArrays[i].m_normal = transformMatrix.TransformVectorQuantity3D(positionVertexArrays[i].m_normal);
	}

}
void TransformTranslateVertexArrayUsingMatrix3D(int vertices, std::vector<Vertex_PCU>& positionVertexArrays, Mat44 transformMatrix)
{
	for (int i = 0; i < vertices; i++)
	{
		positionVertexArrays[i].m_position = transformMatrix.TransformVectorQuantity3D(positionVertexArrays[i].m_position);
		positionVertexArrays[i].m_position = transformMatrix.TransformPosition3D(positionVertexArrays[i].m_position);
	}

}
void TransformTranslateVertexArrayUsingMatrix3D(int vertices, std::vector<Vertex_PNCU>& positionVertexArrays, Mat44 transformMatrix)
{
	for (int i = 0; i < vertices; i++)
	{
		positionVertexArrays[i].m_position = transformMatrix.TransformVectorQuantity3D(positionVertexArrays[i].m_position);
		positionVertexArrays[i].m_position = transformMatrix.TransformPosition3D(positionVertexArrays[i].m_position);
	}

}
void TransformPosition3D(Vec3& posToTransform, float uniformScale, float rotationDegrees, const Vec3& translatePos)
{
	/*Vec3 Normalizedvalue = Normalize(posToTransform);*/
	//Getting x and y for easier calculation
	float x = posToTransform.x;
	float y = posToTransform.y;
	float z = posToTransform.z;

	//Changing the scale of single vertex of the Entity
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;

	//Rotating the position
	Vec3 rotateXYZ = Vec3(x, y,z);
	rotateXYZ = rotateXYZ.GetRotatedAboutZDegrees(rotationDegrees);

	//changing the position single vertex of the Entity
	posToTransform.x = rotateXYZ.x + translatePos.x;
	posToTransform.y = rotateXYZ.y + translatePos.y;
	posToTransform.z = rotateXYZ.z + translatePos.z;
}

void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Capsule2 const& capsule, Rgba8 const& color)
{
	//AddVertsForDisc2D(verts, capsule.m_start, capsule.radius, color);
	//AddVertsForDisc2D(verts, capsule.m_end, capsule.radius, color);

	LineSegment2 boneOfCapsule = LineSegment2(capsule.m_start, capsule.m_end);
	AddVertsForLineSegment2D(verts, boneOfCapsule, capsule.radius*2,color);

	Vec2 startToEnd = capsule.m_end - capsule.m_start;
	Vec2 endToStart = capsule.m_start - capsule.m_end;
	float discOrientationStart = startToEnd.GetOrientationDegrees();
	float discOrientationEnd = endToStart.GetOrientationDegrees();
	AddVertsForHalfDisc2D(verts, capsule.m_start, discOrientationStart,capsule.radius, color);
	AddVertsForHalfDisc2D(verts, capsule.m_end, discOrientationEnd,capsule.radius, color);
}
void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Vec2 const& boneStart, Vec2 const& boneEnd, float radius, Rgba8 const& color)
{
	LineSegment2 boneOfCapsule = LineSegment2(boneStart, boneEnd);
	AddVertsForLineSegment2D(verts, boneOfCapsule, radius, color);
}

void AddVertsForDebugDisk(std::vector<Vertex_PCU>& verts, Vec2 center, float radius, Rgba8 color, float thickness)
{
	const float totalDegrees = 360.0f;
	const int totalSides = 32;
	const int totalVertices = totalSides * 6;
	float thicknessHalf = thickness * 0.5f;
	float splitDegrees = totalDegrees / (float)totalSides;
	float radiusInner = radius - thicknessHalf;
	float radiusOuter = radius + thicknessHalf;
	Vertex_PCU tempVertexArrays[totalVertices];
	for (int sideIndex = 0; sideIndex < totalSides; sideIndex++)
	{
		float point1degrees = splitDegrees * sideIndex;
		float point2Degrees = splitDegrees * (sideIndex + 1);
		Vec3 startPoint1 = Vec3(center.x + (CosDegrees(point1degrees) * radiusInner), center.y + (SinDegrees(point1degrees) * radiusInner), 0.f);
		Vec3 endPoint1 = Vec3(center.x + (CosDegrees(point1degrees) * radiusOuter), center.y + (SinDegrees(point1degrees) * radiusOuter), 0.f);
		Vec3 startPoint2 = Vec3(center.x + (CosDegrees(point2Degrees) * radiusInner), center.y + (SinDegrees(point2Degrees) * radiusInner), 0.f);
		Vec3 endPoint2 = Vec3(center.x + (CosDegrees(point2Degrees) * radiusOuter), center.y + (SinDegrees(point2Degrees) * radiusOuter), 0.f);
		int boxIndex = sideIndex * 6;
		tempVertexArrays[boxIndex].m_position = startPoint1;
		tempVertexArrays[boxIndex + 1].m_position = endPoint1;
		tempVertexArrays[boxIndex + 2].m_position = startPoint2;
		tempVertexArrays[boxIndex + 3].m_position = startPoint2;
		tempVertexArrays[boxIndex + 4].m_position = endPoint1;
		tempVertexArrays[boxIndex + 5].m_position = endPoint2;

		tempVertexArrays[boxIndex].m_color = color;
		tempVertexArrays[boxIndex + 1].m_color = color;
		tempVertexArrays[boxIndex + 2].m_color = color;
		tempVertexArrays[boxIndex + 3].m_color = color;
		tempVertexArrays[boxIndex + 4].m_color = color;
		tempVertexArrays[boxIndex + 5].m_color = color;

	}
	for (int i = 0; i < totalVertices; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
}
void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, Rgba8 const& color)
{
	const float totalDegrees = 360.0f;
	const int totalSides = 32;
	const int totalVertices = totalSides * 6;
	
	float splitDegrees = totalDegrees / (float)totalSides;
	float radiusInner = 0.0f;
	float radiusOuter = radius;
	Vertex_PCU tempVertexArrays[totalVertices];
	for (int sideIndex = 0; sideIndex < totalSides; sideIndex++)
	{
		float point1degrees = splitDegrees * sideIndex;
		float point2Degrees = splitDegrees * (sideIndex + 1);
		Vec3 startPoint1 = Vec3(center.x + (CosDegrees(point1degrees) * radiusInner), center.y + (SinDegrees(point1degrees) * radiusInner), 0.f);
		Vec3 endPoint1 = Vec3(center.x + (CosDegrees(point1degrees) * radiusOuter), center.y + (SinDegrees(point1degrees) * radiusOuter), 0.f);
		Vec3 startPoint2 = Vec3(center.x + (CosDegrees(point2Degrees) * radiusInner), center.y + (SinDegrees(point2Degrees) * radiusInner), 0.f);
		Vec3 endPoint2 = Vec3(center.x + (CosDegrees(point2Degrees) * radiusOuter), center.y + (SinDegrees(point2Degrees) * radiusOuter), 0.f);
		int boxIndex = sideIndex * 6;
		tempVertexArrays[boxIndex].m_position = startPoint1;
		tempVertexArrays[boxIndex + 1].m_position = endPoint1;
		tempVertexArrays[boxIndex + 2].m_position = startPoint2;
		tempVertexArrays[boxIndex + 3].m_position = startPoint2;
		tempVertexArrays[boxIndex + 4].m_position = endPoint1;
		tempVertexArrays[boxIndex + 5].m_position = endPoint2;

		tempVertexArrays[boxIndex].m_color = color;
		tempVertexArrays[boxIndex + 1].m_color = color;
		tempVertexArrays[boxIndex + 2].m_color = color;
		tempVertexArrays[boxIndex + 3].m_color = color;
		tempVertexArrays[boxIndex + 4].m_color = color;
		tempVertexArrays[boxIndex + 5].m_color = color;

	}
	for (int i = 0; i < totalVertices; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
}
void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius,int divisions, Rgba8 const& color)
{
	const float totalDegrees = 360.0f;
	const int totalSides = divisions;
	int totalVertices = totalSides * 6;

	float splitDegrees = totalDegrees / (float)totalSides;
	float radiusInner = 0.0f;
	float radiusOuter = radius;
	std::vector<Vertex_PCU> tempVertexArrays;
	Vertex_PCU vert;
	for (int i = 0; i < totalVertices; i++)
	{
		tempVertexArrays.push_back(vert);
	}
	for (int sideIndex = 0; sideIndex < totalSides; sideIndex++)
	{
		float point1degrees = splitDegrees * sideIndex;
		float point2Degrees = splitDegrees * (sideIndex + 1);
		Vec3 startPoint1 = Vec3(center.x + (CosDegrees(point1degrees) * radiusInner), center.y + (SinDegrees(point1degrees) * radiusInner), 0.f);
		Vec3 endPoint1 = Vec3(center.x + (CosDegrees(point1degrees) * radiusOuter), center.y + (SinDegrees(point1degrees) * radiusOuter), 0.f);
		Vec3 startPoint2 = Vec3(center.x + (CosDegrees(point2Degrees) * radiusInner), center.y + (SinDegrees(point2Degrees) * radiusInner), 0.f);
		Vec3 endPoint2 = Vec3(center.x + (CosDegrees(point2Degrees) * radiusOuter), center.y + (SinDegrees(point2Degrees) * radiusOuter), 0.f);
		int boxIndex = sideIndex * 6;
		tempVertexArrays[boxIndex].m_position = startPoint1;
		tempVertexArrays[boxIndex + 1].m_position = endPoint1;
		tempVertexArrays[boxIndex + 2].m_position = startPoint2;
		tempVertexArrays[boxIndex + 3].m_position = startPoint2;
		tempVertexArrays[boxIndex + 4].m_position = endPoint1;
		tempVertexArrays[boxIndex + 5].m_position = endPoint2;

		tempVertexArrays[boxIndex].m_color = color;
		tempVertexArrays[boxIndex + 1].m_color = color;
		tempVertexArrays[boxIndex + 2].m_color = color;
		tempVertexArrays[boxIndex + 3].m_color = color;
		tempVertexArrays[boxIndex + 4].m_color = color;
		tempVertexArrays[boxIndex + 5].m_color = color;

	}
	for (int i = 0; i < totalVertices; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
}
void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, Vec3 const& center, float radius, Rgba8 const& color, const AABB2& UVs)
{
	UNUSED((void)UVs);
	const float totalDegrees = 360.0f;
	const int totalSides = 16;
	const int totalVertices = totalSides * 6;

	float splitDegrees = totalDegrees / (float)totalSides;
	float radiusInner = 0.0f;
	float radiusOuter = radius;
	Vertex_PCU tempVertexArrays[totalVertices];
	for (int sideIndex = 0; sideIndex < totalSides; sideIndex++)
	{
		float point1degrees = splitDegrees * sideIndex;
		float point2Degrees = splitDegrees * (sideIndex + 1);
		Vec3 startPoint1 = Vec3(center.x + (CosDegrees(point1degrees) * radiusInner), center.y + (SinDegrees(point1degrees) * radiusInner), center.z);
		Vec3 endPoint1 = Vec3(center.x + (CosDegrees(point1degrees) * radiusOuter), center.y + (SinDegrees(point1degrees) * radiusOuter), center.z);
		Vec3 startPoint2 = Vec3(center.x + (CosDegrees(point2Degrees) * radiusInner), center.y + (SinDegrees(point2Degrees) * radiusInner), center.z);
		Vec3 endPoint2 = Vec3(center.x + (CosDegrees(point2Degrees) * radiusOuter), center.y + (SinDegrees(point2Degrees) * radiusOuter), center.z);
		int boxIndex = sideIndex * 6;
		tempVertexArrays[boxIndex].m_position = startPoint1;
		tempVertexArrays[boxIndex + 1].m_position = endPoint1;
		tempVertexArrays[boxIndex + 2].m_position = startPoint2;
		tempVertexArrays[boxIndex + 3].m_position = startPoint2;
		tempVertexArrays[boxIndex + 4].m_position = endPoint1;
		tempVertexArrays[boxIndex + 5].m_position = endPoint2;

		tempVertexArrays[boxIndex].m_color = color;
		tempVertexArrays[boxIndex + 1].m_color = color;
		tempVertexArrays[boxIndex + 2].m_color = color;
		tempVertexArrays[boxIndex + 3].m_color = color;
		tempVertexArrays[boxIndex + 4].m_color = color;
		tempVertexArrays[boxIndex + 5].m_color = color;

		tempVertexArrays[boxIndex].m_uvTexCoords = Vec2(CosDegrees(point1degrees),SinDegrees(point1degrees));
		tempVertexArrays[boxIndex + 1].m_uvTexCoords = Vec2(CosDegrees(point1degrees), SinDegrees(point1degrees));
		tempVertexArrays[boxIndex + 2].m_uvTexCoords = Vec2(CosDegrees(point2Degrees), SinDegrees(point2Degrees));
		tempVertexArrays[boxIndex + 3].m_uvTexCoords = Vec2(CosDegrees(point2Degrees), SinDegrees(point2Degrees));
		tempVertexArrays[boxIndex + 4].m_uvTexCoords = Vec2(CosDegrees(point1degrees), SinDegrees(point1degrees));
		tempVertexArrays[boxIndex + 5].m_uvTexCoords = Vec2(CosDegrees(point2Degrees), SinDegrees(point2Degrees));

	}
	for (int i = 0; i < totalVertices; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
}
void AddVertsForHalfDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float discOrientation, float radius, Rgba8 const& color)
{
	const float totalDegrees = 180.0f;
	const int totalSides = 32;
	const int totalVertices = totalSides * 6;

	float splitDegrees = totalDegrees / (float)totalSides;
	float radiusInner = 0.0f;
	float radiusOuter = radius;
	Vertex_PCU tempVertexArrays[totalVertices];
	for (int sideIndex = 0; sideIndex < totalSides; sideIndex++)
	{
		float point1degrees = splitDegrees * sideIndex;
		float point2Degrees = splitDegrees * (sideIndex + 1);
		Vec3 startPoint1 = Vec3((CosDegrees(point1degrees) * radiusInner), (SinDegrees(point1degrees) * radiusInner), 0.f);
		Vec3 endPoint1 = Vec3((CosDegrees(point1degrees) * radiusOuter), (SinDegrees(point1degrees) * radiusOuter), 0.f);
		Vec3 startPoint2 = Vec3((CosDegrees(point2Degrees) * radiusInner), (SinDegrees(point2Degrees) * radiusInner), 0.f);
		Vec3 endPoint2 = Vec3((CosDegrees(point2Degrees) * radiusOuter), (SinDegrees(point2Degrees) * radiusOuter), 0.f);
		int boxIndex = sideIndex * 6;
		tempVertexArrays[boxIndex].m_position = startPoint1;
		tempVertexArrays[boxIndex + 1].m_position = endPoint1;
		tempVertexArrays[boxIndex + 2].m_position = startPoint2;
		tempVertexArrays[boxIndex + 3].m_position = startPoint2;
		tempVertexArrays[boxIndex + 4].m_position = endPoint1;
		tempVertexArrays[boxIndex + 5].m_position = endPoint2;

		tempVertexArrays[boxIndex].m_color = color;
		tempVertexArrays[boxIndex + 1].m_color = color;
		tempVertexArrays[boxIndex + 2].m_color = color;
		tempVertexArrays[boxIndex + 3].m_color = color;
		tempVertexArrays[boxIndex + 4].m_color = color;
		tempVertexArrays[boxIndex + 5].m_color = color;

	}

	TransformVertexArray3D(totalVertices, tempVertexArrays, 1.0f, 90.0f + discOrientation, Vec3(center.x, center.y, 0.0f));
	for (int i = 0; i < totalVertices; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
}
void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Rgba8 const& color)
{
	Vertex_PCU tempVertexArrays[6];
	tempVertexArrays[0].m_position = Vec3(bounds.m_mins.x, bounds.m_mins.y,0.0f);
	tempVertexArrays[1].m_position = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, 0.0f);
	tempVertexArrays[2].m_position = Vec3(bounds.m_maxs.x, bounds.m_mins.y, 0.0f);

	tempVertexArrays[3].m_position = Vec3(bounds.m_mins.x, bounds.m_mins.y, 0.0f);
	tempVertexArrays[4].m_position = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, 0.0f);
	tempVertexArrays[5].m_position = Vec3(bounds.m_mins.x, bounds.m_maxs.y, 0.0f);

	tempVertexArrays[0].m_color = color;
	tempVertexArrays[1].m_color = color;
	tempVertexArrays[2].m_color = color;
	tempVertexArrays[3].m_color = color;
	tempVertexArrays[4].m_color = color;
	tempVertexArrays[5].m_color = color;
	for (int i = 0; i < 6; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
}

void AddVertsForAABB2DWireFrame(std::vector<Vertex_PCU>& verts, AABB2 const& bounds,float thickness, Rgba8 const& color)
{
	Vertex_PCU tempVertexArrays[4];
	Vec2 pos1 = Vec2(bounds.m_mins.x, bounds.m_mins.y);
	Vec2 pos3 = Vec2(bounds.m_maxs.x, bounds.m_maxs.y);
	Vec2 pos2 = Vec2(bounds.m_maxs.x, bounds.m_mins.y);
	Vec2 pos4 = Vec2(bounds.m_mins.x, bounds.m_maxs.y);

	AddVertsForLineSegment2D(verts, pos1, pos2, thickness, color);
	AddVertsForLineSegment2D(verts, pos2, pos3, thickness, color);
	AddVertsForLineSegment2D(verts, pos3, pos4, thickness, color);
	AddVertsForLineSegment2D(verts, pos4, pos1, thickness, color);
}

void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Vec2 UvAtMins, Vec2 UvAtMaxs, Rgba8 const& color) 
{
	Vertex_PCU tempVertexArrays[6];
	tempVertexArrays[0].m_position = Vec3(bounds.m_mins.x, bounds.m_mins.y, 0.0f);
	tempVertexArrays[1].m_position = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, 0.0f);
	tempVertexArrays[2].m_position = Vec3(bounds.m_maxs.x, bounds.m_mins.y, 0.0f);

	tempVertexArrays[3].m_position = Vec3(bounds.m_mins.x, bounds.m_mins.y, 0.0f);
	tempVertexArrays[4].m_position = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, 0.0f);
	tempVertexArrays[5].m_position = Vec3(bounds.m_mins.x, bounds.m_maxs.y, 0.0f);

	tempVertexArrays[0].m_uvTexCoords = Vec2(UvAtMins.x, UvAtMins.y);
	tempVertexArrays[1].m_uvTexCoords = Vec2(UvAtMaxs.x, UvAtMaxs.y);
	tempVertexArrays[2].m_uvTexCoords = Vec2(UvAtMaxs.x, UvAtMins.y);

	tempVertexArrays[3].m_uvTexCoords = Vec2(UvAtMins.x, UvAtMins.y);
	tempVertexArrays[4].m_uvTexCoords = Vec2(UvAtMaxs.x, UvAtMaxs.y);
	tempVertexArrays[5].m_uvTexCoords = Vec2(UvAtMins.x, UvAtMaxs.y);

	tempVertexArrays[0].m_color = color;
	tempVertexArrays[1].m_color = color;
	tempVertexArrays[2].m_color = color;
	tempVertexArrays[3].m_color = color;
	tempVertexArrays[4].m_color = color;
	tempVertexArrays[5].m_color = color;
	for (int i = 0; i < 6; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
}
void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, AABB2 const& UvBounds, Rgba8 const& color)
{
	Vertex_PCU tempVertexArrays[6];
	tempVertexArrays[0].m_position = Vec3(bounds.m_mins.x, bounds.m_mins.y, 0.0f);
	tempVertexArrays[1].m_position = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, 0.0f);
	tempVertexArrays[2].m_position = Vec3(bounds.m_maxs.x, bounds.m_mins.y, 0.0f);

	tempVertexArrays[3].m_position = Vec3(bounds.m_mins.x, bounds.m_mins.y, 0.0f);
	tempVertexArrays[4].m_position = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, 0.0f);
	tempVertexArrays[5].m_position = Vec3(bounds.m_mins.x, bounds.m_maxs.y, 0.0f);

	tempVertexArrays[0].m_uvTexCoords = Vec2(UvBounds.m_mins.x, UvBounds.m_mins.y);
	tempVertexArrays[1].m_uvTexCoords = Vec2(UvBounds.m_maxs.x, UvBounds.m_maxs.y);
	tempVertexArrays[2].m_uvTexCoords = Vec2(UvBounds.m_maxs.x, UvBounds.m_mins.y);

	tempVertexArrays[3].m_uvTexCoords = Vec2(UvBounds.m_mins.x, UvBounds.m_mins.y);
	tempVertexArrays[4].m_uvTexCoords = Vec2(UvBounds.m_maxs.x, UvBounds.m_maxs.y);
	tempVertexArrays[5].m_uvTexCoords = Vec2(UvBounds.m_mins.x, UvBounds.m_maxs.y);

	tempVertexArrays[0].m_color = color;
	tempVertexArrays[1].m_color = color;
	tempVertexArrays[2].m_color = color;
	tempVertexArrays[3].m_color = color;
	tempVertexArrays[4].m_color = color;
	tempVertexArrays[5].m_color = color;
	for (int i = 0; i < 6; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
}
void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, OBB2 const& bounds, Rgba8 const& color)
{
	Vertex_PCU tempVertexArrays[6];
	//Vec2 m_mins = Vec2(bounds.m_center.x - (bounds.m_iBasisNormal.x * bounds.m_halfDimensions.x), bounds.m_center.y - (bounds.m_iBasisNormal.y * bounds.m_halfDimensions.y));
	//Vec2 m_maxs = Vec2(bounds.m_center.x + (bounds.m_iBasisNormal.x * bounds.m_halfDimensions.x), bounds.m_center.y + (bounds.m_iBasisNormal.y * bounds.m_halfDimensions.y));
	
	Vec2 rightDisplacement = bounds.m_iBasisNormal * bounds.m_halfDimensions.x;
	Vec2 m_jbasisNormal = Vec2(-bounds.m_iBasisNormal.y, bounds.m_iBasisNormal.x);
	Vec2 upDisplacement = m_jbasisNormal * bounds.m_halfDimensions.y;

	Vec2 A = bounds.m_center + rightDisplacement + upDisplacement;
	Vec2 B = bounds.m_center - rightDisplacement + upDisplacement;
	Vec2 C = bounds.m_center - rightDisplacement - upDisplacement;
	Vec2 D = bounds.m_center + rightDisplacement - upDisplacement;

	tempVertexArrays[0].m_position = Vec3(C.x, C.y, 0.0f);
	tempVertexArrays[1].m_position = Vec3(B.x, B.y, 0.0f);
	tempVertexArrays[2].m_position = Vec3(A.x, A.y, 0.0f);

	tempVertexArrays[3].m_position = Vec3(A.x, A.y, 0.0f);
	tempVertexArrays[4].m_position = Vec3(C.x, C.y, 0.0f);
	tempVertexArrays[5].m_position = Vec3(D.x, D.y, 0.0f);

	tempVertexArrays[0].m_color = color;
	tempVertexArrays[1].m_color = color;
	tempVertexArrays[2].m_color = color;
	tempVertexArrays[3].m_color = color;
	tempVertexArrays[4].m_color = color;
	tempVertexArrays[5].m_color = color;
	for (int i = 0; i < 6; i++)
	{
		/*Vec2 m_jBasisNormal = Vec2(-bounds.m_iBasisNormal.y, bounds.m_iBasisNormal.x);
		Vec2 position = Vec2(tempVertexArrays[i].m_position.x, tempVertexArrays[i].m_position.y);
		TransformPositionXY3D(tempVertexArrays[i].m_position, bounds.m_iBasisNormal, m_jBasisNormal, Vec2(0.0f, 0.0f));*/
		verts.push_back(tempVertexArrays[i]);
	}
}
void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, OBB2 const& bounds,AABB2 uvBounds, Rgba8 const& color)
{
	Vertex_PCU tempVertexArrays[6];
	//Vec2 m_mins = Vec2(bounds.m_center.x - (bounds.m_iBasisNormal.x * bounds.m_halfDimensions.x), bounds.m_center.y - (bounds.m_iBasisNormal.y * bounds.m_halfDimensions.y));
	//Vec2 m_maxs = Vec2(bounds.m_center.x + (bounds.m_iBasisNormal.x * bounds.m_halfDimensions.x), bounds.m_center.y + (bounds.m_iBasisNormal.y * bounds.m_halfDimensions.y));

	Vec2 rightDisplacement = bounds.m_iBasisNormal * bounds.m_halfDimensions.x;
	Vec2 m_jbasisNormal = Vec2(-bounds.m_iBasisNormal.y, bounds.m_iBasisNormal.x);
	Vec2 upDisplacement = m_jbasisNormal * bounds.m_halfDimensions.y;

	Vec2 A = bounds.m_center + rightDisplacement + upDisplacement;
	Vec2 B = bounds.m_center - rightDisplacement + upDisplacement;
	Vec2 C = bounds.m_center - rightDisplacement - upDisplacement;
	Vec2 D = bounds.m_center + rightDisplacement - upDisplacement;

	tempVertexArrays[0].m_position = Vec3(C.x, C.y, 0.0f);
	tempVertexArrays[1].m_position = Vec3(B.x, B.y, 0.0f);
	tempVertexArrays[2].m_position = Vec3(A.x, A.y, 0.0f);

	tempVertexArrays[3].m_position = Vec3(A.x, A.y, 0.0f);
	tempVertexArrays[4].m_position = Vec3(C.x, C.y, 0.0f);
	tempVertexArrays[5].m_position = Vec3(D.x, D.y, 0.0f);

	tempVertexArrays[0].m_uvTexCoords = Vec2(uvBounds.m_mins.x, uvBounds.m_mins.y);
	tempVertexArrays[1].m_uvTexCoords = Vec2(uvBounds.m_maxs.x, uvBounds.m_maxs.y);
	tempVertexArrays[2].m_uvTexCoords = Vec2(uvBounds.m_maxs.x, uvBounds.m_mins.y);
											 			
	tempVertexArrays[3].m_uvTexCoords = Vec2(uvBounds.m_mins.x, uvBounds.m_mins.y);
	tempVertexArrays[4].m_uvTexCoords = Vec2(uvBounds.m_maxs.x, uvBounds.m_maxs.y);
	tempVertexArrays[5].m_uvTexCoords = Vec2(uvBounds.m_mins.x, uvBounds.m_maxs.y);

	tempVertexArrays[0].m_color = color;
	tempVertexArrays[1].m_color = color;
	tempVertexArrays[2].m_color = color;
	tempVertexArrays[3].m_color = color;
	tempVertexArrays[4].m_color = color;
	tempVertexArrays[5].m_color = color;
	for (int i = 0; i < 6; i++)
	{
		/*Vec2 m_jBasisNormal = Vec2(-bounds.m_iBasisNormal.y, bounds.m_iBasisNormal.x);
		Vec2 position = Vec2(tempVertexArrays[i].m_position.x, tempVertexArrays[i].m_position.y);
		TransformPositionXY3D(tempVertexArrays[i].m_position, bounds.m_iBasisNormal, m_jBasisNormal, Vec2(0.0f, 0.0f));*/
		verts.push_back(tempVertexArrays[i]);
	}
}
void AddVertsForConvexPoly2D(std::vector<Vertex_PCU>& verts, ConvexPoly2D const& poly2D,float edgeThickness, AABB2 uvBounds, Rgba8 const& color)
{

	UNUSED((void)edgeThickness);
	//-------ADD THE INNER AREAS--------------
	for (int i = 1; i < poly2D.GetTotalPoints()-1; i ++ )
	{
		Vertex_PCU vert;
		vert.m_position = Vec3(poly2D.GetPointAtIndex(0));
		vert.m_color = color;
		vert.m_uvTexCoords = uvBounds.m_mins;

		verts.push_back(vert);
		vert.m_position = Vec3(poly2D.GetPointAtIndex(i ));
		verts.push_back(vert);
		vert.m_position = Vec3(poly2D.GetPointAtIndex(i + 1));
		verts.push_back(vert);
		
	}

}

void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, LineSegment2 const& lineSegment, float thickness, Rgba8 const& color)
{
	float halfWidth = thickness * 0.5f;
	Vec2 end = lineSegment.m_end;
	Vec2 start = lineSegment.m_start;
	Vec2 unitForwardVector = (end - start) / (end - start).GetLength();
	Vec2 unitLeftVector = unitForwardVector.GetRotated90Degrees();
	Vec2 leftTopCorner = start + (halfWidth * unitLeftVector);
	Vec2 leftBottomCorner = start - (halfWidth * unitLeftVector);
	Vec2 rightTopCorner = end + (halfWidth * unitLeftVector);
	Vec2 rightBottomCorner = end - (halfWidth * unitLeftVector);

	Vertex_PCU tempVertexArrays[6];
	tempVertexArrays[0].m_position = Vec3(leftTopCorner.x, leftTopCorner.y, 0.f);
	tempVertexArrays[1].m_position = Vec3(leftBottomCorner.x, leftBottomCorner.y, 0.f);
	tempVertexArrays[2].m_position = Vec3(rightTopCorner.x, rightTopCorner.y, 0.f);
	tempVertexArrays[3].m_position = Vec3(leftBottomCorner.x, leftBottomCorner.y, 0.f);
	tempVertexArrays[4].m_position = Vec3(rightBottomCorner.x, rightBottomCorner.y, 0.f);
	tempVertexArrays[5].m_position = Vec3(rightTopCorner.x, rightTopCorner.y, 0.f);

	tempVertexArrays[0].m_color = color;
	tempVertexArrays[1].m_color = color;
	tempVertexArrays[2].m_color = color;
	tempVertexArrays[3].m_color = color;
	tempVertexArrays[4].m_color = color;
	tempVertexArrays[5].m_color = color;

	for (int i = 0; i < 6; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
}
void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color)
{
	float halfWidth = thickness * 0.5f;
	Vec2 unitForwardVector = (end - start) / (end - start).GetLength();
	Vec2 unitLeftVector = unitForwardVector.GetRotated90Degrees();
	Vec2 leftTopCorner = start + (halfWidth * unitLeftVector);
	Vec2 leftBottomCorner = start - (halfWidth * unitLeftVector);
	Vec2 rightTopCorner = end + (halfWidth * unitLeftVector);
	Vec2 rightBottomCorner = end - (halfWidth * unitLeftVector);

	Vertex_PCU tempVertexArrays[6];
	tempVertexArrays[0].m_position = Vec3(leftTopCorner.x, leftTopCorner.y, 0.f);
	tempVertexArrays[1].m_position = Vec3(leftBottomCorner.x, leftBottomCorner.y, 0.f);
	tempVertexArrays[2].m_position = Vec3(rightTopCorner.x, rightTopCorner.y, 0.f);
	tempVertexArrays[3].m_position = Vec3(leftBottomCorner.x, leftBottomCorner.y, 0.f);
	tempVertexArrays[4].m_position = Vec3(rightBottomCorner.x, rightBottomCorner.y, 0.f);
	tempVertexArrays[5].m_position = Vec3(rightTopCorner.x, rightTopCorner.y, 0.f);

	tempVertexArrays[0].m_color = color;
	tempVertexArrays[1].m_color = color;
	tempVertexArrays[2].m_color = color;
	tempVertexArrays[3].m_color = color;
	tempVertexArrays[4].m_color = color;
	tempVertexArrays[5].m_color = color;

	for (int i = 0; i < 6; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
}
void AddVertsForRing2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, float thickness, Rgba8 const& color)
{
	const float totalDegrees = 360.0f;
	const int totalSides = 32;
	const int totalVertices = totalSides * 6;
	float thicknessHalf = thickness * 0.5f;
	float splitDegrees = totalDegrees / (float)totalSides;
	float radiusInner = radius - thicknessHalf;
	float radiusOuter = radius + thicknessHalf;
	Vertex_PCU tempVertexArrays[totalVertices];
	for (int sideIndex = 0; sideIndex < totalSides; sideIndex++)
	{
		float point1degrees = splitDegrees * sideIndex;
		float point2Degrees = splitDegrees * (sideIndex + 1);
		Vec3 startPoint1 = Vec3(center.x + (CosDegrees(point1degrees) * radiusInner), center.y + (SinDegrees(point1degrees) * radiusInner), 0.f);
		Vec3 endPoint1 = Vec3(center.x + (CosDegrees(point1degrees) * radiusOuter), center.y + (SinDegrees(point1degrees) * radiusOuter), 0.f);
		Vec3 startPoint2 = Vec3(center.x + (CosDegrees(point2Degrees) * radiusInner), center.y + (SinDegrees(point2Degrees) * radiusInner), 0.f);
		Vec3 endPoint2 = Vec3(center.x + (CosDegrees(point2Degrees) * radiusOuter), center.y + (SinDegrees(point2Degrees) * radiusOuter), 0.f);
		int boxIndex = sideIndex * 6;
		tempVertexArrays[boxIndex].m_position = startPoint1;
		tempVertexArrays[boxIndex + 1].m_position = endPoint1;
		tempVertexArrays[boxIndex + 2].m_position = startPoint2;
		tempVertexArrays[boxIndex + 3].m_position = startPoint2;
		tempVertexArrays[boxIndex + 4].m_position = endPoint1;
		tempVertexArrays[boxIndex + 5].m_position = endPoint2;

		tempVertexArrays[boxIndex].m_color = color;
		tempVertexArrays[boxIndex + 1].m_color = color;
		tempVertexArrays[boxIndex + 2].m_color = color;
		tempVertexArrays[boxIndex + 3].m_color = color;
		tempVertexArrays[boxIndex + 4].m_color = color;
		tempVertexArrays[boxIndex + 5].m_color = color;

		for (int i = 0; i < totalVertices; i++)
		{
			verts.push_back(tempVertexArrays[i]);
		}
	}
}
void AddVertsForTexts2D(std::vector<Vertex_PCU>& vertexArray, Vec2 const& textMins, AABB2 const& UvBounds, float cellHeight, std::string const& text, Rgba8 const& tint, float cellAspect)
 {
	UNUSED((void)text);
	Vertex_PCU tempVertexArrays[6];
	AABB2 bounds;
	bounds.m_maxs = textMins;
	bounds.m_mins.x = textMins.x - cellHeight * cellAspect;
	bounds.m_mins.y = textMins.y + cellHeight;
	tempVertexArrays[0].m_position = Vec3(bounds.m_mins.x, bounds.m_mins.y, 0.0f);
	tempVertexArrays[1].m_position = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, 0.0f);
	tempVertexArrays[2].m_position = Vec3(bounds.m_maxs.x, bounds.m_mins.y, 0.0f);

	tempVertexArrays[3].m_position = Vec3(bounds.m_mins.x, bounds.m_mins.y, 0.0f);
	tempVertexArrays[4].m_position = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, 0.0f);
	tempVertexArrays[5].m_position = Vec3(bounds.m_mins.x, bounds.m_maxs.y, 0.0f);

	tempVertexArrays[0].m_uvTexCoords = Vec2(UvBounds.m_mins.x, UvBounds.m_mins.y);
	tempVertexArrays[1].m_uvTexCoords = Vec2(UvBounds.m_maxs.x, UvBounds.m_maxs.y);
	tempVertexArrays[2].m_uvTexCoords = Vec2(UvBounds.m_maxs.x, UvBounds.m_mins.y);

	tempVertexArrays[3].m_uvTexCoords = Vec2(UvBounds.m_mins.x, UvBounds.m_mins.y);
	tempVertexArrays[4].m_uvTexCoords = Vec2(UvBounds.m_maxs.x, UvBounds.m_maxs.y);
	tempVertexArrays[5].m_uvTexCoords = Vec2(UvBounds.m_mins.x, UvBounds.m_maxs.y);

	tempVertexArrays[0].m_color = tint;
	tempVertexArrays[1].m_color = tint;
	tempVertexArrays[2].m_color = tint;
	tempVertexArrays[3].m_color = tint;
	tempVertexArrays[4].m_color = tint;
	tempVertexArrays[5].m_color = tint;
	for (int i = 0; i < 6; i++)
	{
		vertexArray.push_back(tempVertexArrays[i]);
	}
}
void AddVertsForArrow2D(std::vector<Vertex_PCU>& verts, Vec2 tailPos, Vec2 tipPos, float arrowSize, float lineThickness, Rgba8 color)
{
	LineSegment2 line = LineSegment2(tailPos, tipPos);
	AddVertsForLineSegment2D(verts, line, lineThickness, color);

	Vec2 arrowHead1 = tipPos;
	Vec2 arrowHead2 = tipPos;
	float orientation = (tipPos - tailPos).GetOrientationDegrees();
	arrowHead1.SetOrientationDegrees(orientation - 135.0f);
	arrowHead2.SetOrientationDegrees(orientation + 135.0f);
	arrowHead1.SetLength(arrowSize);
	arrowHead2.SetLength(arrowSize);

	arrowHead1 += tipPos;
	arrowHead2 += tipPos;
	line = LineSegment2(tipPos, arrowHead1);
	AddVertsForLineSegment2D(verts, line, lineThickness, color);

	line = LineSegment2(tipPos, arrowHead2);
	AddVertsForLineSegment2D(verts, line, lineThickness, color);
}

void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color, const AABB2& UVs)
{
	Vertex_PCU tempVertexArrays[6];
	tempVertexArrays[0].m_position = bottomLeft;
	tempVertexArrays[1].m_position = bottomRight;
	tempVertexArrays[2].m_position = topLeft;

	tempVertexArrays[3].m_position = topLeft;
	tempVertexArrays[4].m_position = bottomRight;
	tempVertexArrays[5].m_position = topRight;

	tempVertexArrays[0].m_color = color;
	tempVertexArrays[1].m_color = color;
	tempVertexArrays[2].m_color = color;
	tempVertexArrays[3].m_color = color;
	tempVertexArrays[4].m_color = color;
	tempVertexArrays[5].m_color = color;

	tempVertexArrays[0].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_mins.y);
	tempVertexArrays[1].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	tempVertexArrays[2].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);

	tempVertexArrays[3].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	tempVertexArrays[4].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	tempVertexArrays[5].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_maxs.y);

	for (int i = 0; i < 6; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
}
void AddVertsForQuadPNCU3D(std::vector<Vertex_PNCU>& verts, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color, const AABB2& UVs)
{
	Vertex_PNCU tempVertexArrays[6];
	tempVertexArrays[0].m_position = bottomLeft;
	tempVertexArrays[1].m_position = bottomRight;
	tempVertexArrays[2].m_position = topLeft;

	tempVertexArrays[3].m_position = topLeft;
	tempVertexArrays[4].m_position = bottomRight;
	tempVertexArrays[5].m_position = topRight;

	tempVertexArrays[0].m_color = color;
	tempVertexArrays[1].m_color = color;
	tempVertexArrays[2].m_color = color;
	tempVertexArrays[3].m_color = color;
	tempVertexArrays[4].m_color = color;
	tempVertexArrays[5].m_color = color;

	tempVertexArrays[0].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_mins.y);
	tempVertexArrays[1].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	tempVertexArrays[2].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);

	tempVertexArrays[3].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	tempVertexArrays[4].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	tempVertexArrays[5].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_maxs.y);

	Vec3 a = (bottomLeft - bottomRight).GetNormalized();
	Vec3 b = (topRight - bottomRight).GetNormalized();
	Vec3 normal = CrossProduct3D(a, b).GetNormalized();

	tempVertexArrays[0].m_normal = normal;
	tempVertexArrays[1].m_normal = normal;
	tempVertexArrays[2].m_normal = normal;
	tempVertexArrays[4].m_normal = normal;
	tempVertexArrays[5].m_normal = normal;

	for (int i = 0; i < 6; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
}
void AddVertsForQuadPNCU3D(std::vector<Vertex_PNCU>& verts,Vec3 normal, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color, const AABB2& UVs)
{
	Vertex_PNCU tempVertexArrays[6];
	tempVertexArrays[0].m_position = bottomLeft;
	tempVertexArrays[1].m_position = bottomRight;
	tempVertexArrays[2].m_position = topLeft;

	tempVertexArrays[3].m_position = topLeft;
	tempVertexArrays[4].m_position = bottomRight;
	tempVertexArrays[5].m_position = topRight;

	tempVertexArrays[0].m_color = color;
	tempVertexArrays[1].m_color = color;
	tempVertexArrays[2].m_color = color;
	tempVertexArrays[3].m_color = color;
	tempVertexArrays[4].m_color = color;
	tempVertexArrays[5].m_color = color;

	tempVertexArrays[0].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_mins.y);
	tempVertexArrays[1].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	tempVertexArrays[2].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);

	tempVertexArrays[3].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	tempVertexArrays[4].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	tempVertexArrays[5].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_maxs.y);

	//Vec3 a = (bottomLeft - bottomRight).GetNormalized();
	//Vec3 b = (topRight - bottomRight).GetNormalized();
	//Vec3 normal = CrossProduct3D(a, b).GetNormalized();

	tempVertexArrays[0].m_normal = normal;
	tempVertexArrays[1].m_normal = normal;
	tempVertexArrays[2].m_normal = normal;
	tempVertexArrays[4].m_normal = normal;
	tempVertexArrays[5].m_normal = normal;

	for (int i = 0; i < 6; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
}
void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, const Vec3& m_mins, const Vec3& m_maxs, const Rgba8& color, const AABB2& UVs)
{
	Vec3 topleft = Vec3(m_maxs.x , m_maxs.y, m_maxs.z);
	Vec3 bottomleft = Vec3(m_mins.x, m_maxs.y, m_mins.z);
	Vec3 topRight = Vec3(m_mins.x, m_mins.y, m_maxs.z);
	Vec3 bottomRight = Vec3(m_maxs.x, m_mins.y, m_mins.z);
	AddVertsForQuad3D(verts, topleft, bottomleft, bottomRight, topRight, color, UVs);
}
void AddVertsForRoundedQuad3D(std::vector<Vertex_PNCU>& verts, const Vec3& m_mins, const Vec3& m_maxs, const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE)
{
	Vec3 topleft = Vec3(m_maxs.x, m_maxs.y, m_maxs.z);
	Vec3 bottomleft = Vec3(m_mins.x, m_maxs.y, m_mins.z);
	Vec3 topRight = Vec3(m_mins.x, m_mins.y, m_maxs.z);
	Vec3 bottomRight = Vec3(m_maxs.x, m_mins.y, m_mins.z);
	AddVertsForRoundedQuad3D(verts, topleft, bottomleft, bottomRight, topRight, color, UVs);
}
void AddVertsForRoundedQuad3D(std::vector<Vertex_PNCU>& verts, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color, const AABB2& UVs)
{
	float quadWidth = GetDistance3D(topLeft, topRight);
	Vec3 topMid = topLeft + (topRight - topLeft).GetNormalized() * quadWidth * 0.5f;
	Vec3 bottomMid = bottomLeft + (bottomRight - bottomLeft).GetNormalized() * quadWidth * 0.5f;
	
	float midbottomxUV = (UVs.m_mins.x + UVs.m_maxs.x) * 0.5f;
	float midbottomyUV = UVs.m_mins.y;

	float midtopxUV = (UVs.m_mins.x + UVs.m_maxs.x) * 0.5f;
	float midtopyUV = UVs.m_maxs.y;

	Vec2 bottomMidUVS = Vec2(midbottomxUV, midbottomyUV);
	Vec2 topMidUVS = Vec2(midtopxUV, midtopyUV);

	Vec3 leftNormal = (bottomRight - bottomLeft).GetNormalized();
	Vec3 rightNormal = (bottomLeft - bottomRight).GetNormalized();
	Vec3 centerNormal = (bottomLeft - topLeft).GetNormalized();
	Vec3 topCenterNormal = CrossProduct3D(leftNormal, -centerNormal);
	Vec3 bottomCenterNormal = CrossProduct3D(rightNormal, centerNormal);
	Vertex_PNCU tempVertexArrays[12];

	//-----------------POSITIONS----------------------
	tempVertexArrays[0].m_position = bottomLeft;
	tempVertexArrays[1].m_position = bottomMid;
	tempVertexArrays[2].m_position = topLeft;

	tempVertexArrays[3].m_position = topLeft;
	tempVertexArrays[4].m_position = bottomMid;
	tempVertexArrays[5].m_position = topMid;

	tempVertexArrays[6].m_position = bottomMid;
	tempVertexArrays[7].m_position = bottomRight;
	tempVertexArrays[8].m_position = topMid;

	tempVertexArrays[9].m_position = topMid;
	tempVertexArrays[10].m_position = bottomRight;
	tempVertexArrays[11].m_position = topRight;

	//-----------------NORMALS------------------------
	tempVertexArrays[0].m_normal = leftNormal;
	tempVertexArrays[1].m_normal = bottomCenterNormal;
	tempVertexArrays[2].m_normal = leftNormal;

	tempVertexArrays[3].m_normal = leftNormal;
	tempVertexArrays[4].m_normal = bottomCenterNormal;
	tempVertexArrays[5].m_normal = topCenterNormal;

	tempVertexArrays[6].m_normal = bottomCenterNormal;
	tempVertexArrays[7].m_normal = rightNormal;
	tempVertexArrays[8].m_normal = topCenterNormal;

	tempVertexArrays[9].m_normal = topCenterNormal;
	tempVertexArrays[10].m_normal = rightNormal;
	tempVertexArrays[11].m_normal = rightNormal;

	//---------------UVS-------------------
	tempVertexArrays[0].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_mins.y);
	tempVertexArrays[1].m_uvTexCoords = bottomMidUVS;
	tempVertexArrays[2].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);

	tempVertexArrays[3].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	tempVertexArrays[4].m_uvTexCoords = bottomMidUVS;
	tempVertexArrays[5].m_uvTexCoords = topMidUVS;

	tempVertexArrays[6].m_uvTexCoords = bottomMidUVS;
	tempVertexArrays[7].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	tempVertexArrays[8].m_uvTexCoords = topMidUVS;

	tempVertexArrays[9].m_uvTexCoords = topMidUVS;
	tempVertexArrays[10].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	tempVertexArrays[11].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_maxs.y);

	for (int i = 0; i < 12; i++)
	{
		tempVertexArrays[i].m_color = color;
		verts.push_back(tempVertexArrays[i]);
	}
}
void AddVertsForIndexedPNCUQuad3D(std::vector<Vertex_PNCU>& verts, std::vector<unsigned int>& indexedVerts,Vec3& normal, int indexCount,
const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color, const AABB2& UVs)
{
	Vertex_PNCU tempVertexArrays[4];
	tempVertexArrays[0].m_position = topLeft;
	tempVertexArrays[1].m_position = bottomLeft;
	tempVertexArrays[2].m_position = bottomRight;
	tempVertexArrays[3].m_position = topRight;

	tempVertexArrays[0].m_color = color;
	tempVertexArrays[1].m_color = color;
	tempVertexArrays[2].m_color = color;
	tempVertexArrays[3].m_color = color;
	tempVertexArrays[0].m_normal = normal;
	tempVertexArrays[1].m_normal = normal;
	tempVertexArrays[2].m_normal = normal;
	tempVertexArrays[3].m_normal = normal;

	tempVertexArrays[0].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	tempVertexArrays[1].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_mins.y);
	tempVertexArrays[2].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	tempVertexArrays[3].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_maxs.y);


	for (int i = 0; i < 4; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
	indexedVerts.push_back(indexCount);
	indexedVerts.push_back(indexCount + 1);
	indexedVerts.push_back(indexCount + 2);
	indexedVerts.push_back(indexCount);
	indexedVerts.push_back(indexCount + 2);
	indexedVerts.push_back(indexCount + 3);
}

void AddVertsForIndexedPNCUQuadtangent3D(std::vector<Vertex_PNCUTB>& verts, std::vector<unsigned int>& indexedVerts, Vec3& normal, int indexCount,
	AABB3 bounds, const Vec4& color, const AABB2& UVs)
{
	float minx = bounds.m_mins.x;
	float miny = bounds.m_mins.y;
	float minz = bounds.m_mins.z;
	float maxx = bounds.m_maxs.x;
	float maxy = bounds.m_maxs.y;
	float maxz = bounds.m_maxs.z;

	Vec3 bottomLeft = Vec3(maxx, miny, minz);
	Vec3 bottomRight = Vec3(maxx, maxy, minz);
	Vec3 topLeft = Vec3(minx, miny, maxz);
	Vec3 topRight = Vec3(minx, maxy, maxz);


	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvs;
	std::vector<Vec3> tangents(6, Vec3());
	std::vector<Vec3> bitangents(6, Vec3());

	//------------------Adding 6 verts. 2 triangles. Each vert inside a triangle will have same tangent and bitangent ----------------
	//tangents.reserve(6);
	//bitangents.reserve(6);
	//MikktSpace tangentSpace;
	Vertex_PNCUTB tempVertexArrays[4];
	tempVertexArrays[0].m_position = topLeft;
	tempVertexArrays[1].m_position = bottomLeft;
	tempVertexArrays[2].m_position = bottomRight;
	tempVertexArrays[3].m_position = topRight;

	tempVertexArrays[0].m_color = color;
	tempVertexArrays[1].m_color = color;
	tempVertexArrays[2].m_color = color;
	tempVertexArrays[3].m_color = color;

	tempVertexArrays[0].m_normal = normal;
	tempVertexArrays[1].m_normal = normal;
	tempVertexArrays[2].m_normal = normal;
	tempVertexArrays[3].m_normal = normal;

	tempVertexArrays[0].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	tempVertexArrays[1].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_mins.y);
	tempVertexArrays[2].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	tempVertexArrays[3].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_maxs.y);

	positions.push_back(tempVertexArrays[0].m_position);
	positions.push_back(tempVertexArrays[1].m_position);
	positions.push_back(tempVertexArrays[2].m_position);
	positions.push_back(tempVertexArrays[0].m_position);
	positions.push_back(tempVertexArrays[2].m_position);
	positions.push_back(tempVertexArrays[3].m_position);

	normals.push_back(tempVertexArrays[0].m_normal);
	normals.push_back(tempVertexArrays[1].m_normal);
	normals.push_back(tempVertexArrays[2].m_normal);
	normals.push_back(tempVertexArrays[0].m_normal);
	normals.push_back(tempVertexArrays[2].m_normal);
	normals.push_back(tempVertexArrays[3].m_normal);

	uvs.push_back(tempVertexArrays[0].m_uvTexCoords);
	uvs.push_back(tempVertexArrays[1].m_uvTexCoords);
	uvs.push_back(tempVertexArrays[2].m_uvTexCoords);
	uvs.push_back(tempVertexArrays[0].m_uvTexCoords);
	uvs.push_back(tempVertexArrays[2].m_uvTexCoords);
	uvs.push_back(tempVertexArrays[3].m_uvTexCoords);

	//tangents.push_back(tempVertexArrays[0].m_tangent);
	//tangents.push_back(tempVertexArrays[1].m_tangent);
	//tangents.push_back(tempVertexArrays[2].m_tangent);
	//tangents.push_back(tempVertexArrays[0].m_tangent);
	//tangents.push_back(tempVertexArrays[2].m_tangent);
	//tangents.push_back(tempVertexArrays[3].m_tangent);

	//------------------Old Method--------------------
	/*Vec3 deltaPos1 = tempVertexArrays[1].m_position - tempVertexArrays[0].m_position;
	Vec3 deltaPos2 = tempVertexArrays[2].m_position - tempVertexArrays[0].m_position;

	Vec2 deltaUV1 = tempVertexArrays[1].m_uvTexCoords - tempVertexArrays[0].m_uvTexCoords;
	Vec2 deltaUV2 = tempVertexArrays[2].m_uvTexCoords - tempVertexArrays[0].m_uvTexCoords;

	float division = deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x;
	float r = 1.0f;
	if (division != 0.0f)
	{
		r = 1.0f/ division;
	}

	Vec3 tangent = (deltaPos1 * deltaUV2.x - deltaPos2 * deltaUV1.x) * r;
	Vec3 bitangent = (deltaPos2 * deltaUV1.y - deltaPos1 * deltaUV2.y) * r;

	for (int i = 0; i < 4; i++)
	{
		tempVertexArrays[i].m_tangent = tangent.GetNormalized();
		tempVertexArrays[i].m_bitangent = bitangent.GetNormalized();
		verts.push_back(tempVertexArrays[i]);

	}*/

	//------------------Proper Method--------------------
	int vertexCount = (int)positions.size();
	int totalTriangles = vertexCount / 3;

	for (int i = 0; i < totalTriangles; i++)
	{
		int triangleCount = i * 3;
		Vec3 v1 = positions[triangleCount];
		Vec3 v2 = positions[triangleCount + 1];
		Vec3 v3 = positions[triangleCount + 2];

		Vec2 w1 = uvs[triangleCount];
		Vec2 w2 = uvs[triangleCount + 1];
		Vec2 w3 = uvs[triangleCount + 2];

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

		tangents[triangleCount] = tang.GetNormalized();
		tangents[triangleCount + 1] = tang.GetNormalized();
		tangents[triangleCount + 2] = tang.GetNormalized();

		bitangents[triangleCount] = bitang.GetNormalized();
		bitangents[triangleCount + 1] = bitang.GetNormalized();
		bitangents[triangleCount + 2] = bitang.GetNormalized();
	}
	//----------Since triangles are index it goes 0,1,2,3. Which means the tangent of the 4th index is actually the 5th one in the array-----------
	//----------This is a hack--------------
	tangents[3] = tangents[5];
	bitangents[3] = bitangents[5];
	for (int i = 0; i < 4; i++)
	{
		tempVertexArrays[i].m_tangent = tangents[i].GetNormalized();
		tempVertexArrays[i].m_bitangent = bitangents[i].GetNormalized();
		verts.push_back(tempVertexArrays[i]);
	}

	indexedVerts.push_back(indexCount);
	indexedVerts.push_back(indexCount + 1);
	indexedVerts.push_back(indexCount + 2);
	indexedVerts.push_back(indexCount);
	indexedVerts.push_back(indexCount + 2);
	indexedVerts.push_back(indexCount + 3);

}

void AddVertsForIndexedPNCUQuadtangent3D(std::vector<Vertex_PNCUTB>& verts, std::vector<unsigned int>& indexedVerts, Vec3& normal, int indexCount,
	const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Vec4& color, const AABB2& UVs)
{
	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvs;
	std::vector<Vec3> tangents(6,Vec3());
	std::vector<Vec3> bitangents(6, Vec3());

	//------------------Adding 6 verts. 2 triangles. Each vert inside a triangle will have same tangent and bitangent ----------------
	//tangents.reserve(6);
	//bitangents.reserve(6);
	//MikktSpace tangentSpace;
	Vertex_PNCUTB tempVertexArrays[4];
	tempVertexArrays[0].m_position = topLeft;
	tempVertexArrays[1].m_position = bottomLeft;
	tempVertexArrays[2].m_position = bottomRight;
	tempVertexArrays[3].m_position = topRight;

	tempVertexArrays[0].m_color = color;
	tempVertexArrays[1].m_color = color;
	tempVertexArrays[2].m_color = color;
	tempVertexArrays[3].m_color = color;

	tempVertexArrays[0].m_normal = normal;
	tempVertexArrays[1].m_normal = normal;
	tempVertexArrays[2].m_normal = normal;
	tempVertexArrays[3].m_normal = normal;

	tempVertexArrays[0].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	tempVertexArrays[1].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_mins.y);
	tempVertexArrays[2].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	tempVertexArrays[3].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_maxs.y);

	positions.push_back(tempVertexArrays[0].m_position);
	positions.push_back(tempVertexArrays[1].m_position);
	positions.push_back(tempVertexArrays[2].m_position);
	positions.push_back(tempVertexArrays[0].m_position);
	positions.push_back(tempVertexArrays[2].m_position);
	positions.push_back(tempVertexArrays[3].m_position);

	normals.push_back(tempVertexArrays[0].m_normal);
	normals.push_back(tempVertexArrays[1].m_normal);
	normals.push_back(tempVertexArrays[2].m_normal);
	normals.push_back(tempVertexArrays[0].m_normal);
	normals.push_back(tempVertexArrays[2].m_normal);
	normals.push_back(tempVertexArrays[3].m_normal);

	uvs.push_back(tempVertexArrays[0].m_uvTexCoords);
	uvs.push_back(tempVertexArrays[1].m_uvTexCoords);
	uvs.push_back(tempVertexArrays[2].m_uvTexCoords);
	uvs.push_back(tempVertexArrays[0].m_uvTexCoords);
	uvs.push_back(tempVertexArrays[2].m_uvTexCoords);
	uvs.push_back(tempVertexArrays[3].m_uvTexCoords);

	//tangents.push_back(tempVertexArrays[0].m_tangent);
	//tangents.push_back(tempVertexArrays[1].m_tangent);
	//tangents.push_back(tempVertexArrays[2].m_tangent);
	//tangents.push_back(tempVertexArrays[0].m_tangent);
	//tangents.push_back(tempVertexArrays[2].m_tangent);
	//tangents.push_back(tempVertexArrays[3].m_tangent);

	//------------------Old Method--------------------
	/*Vec3 deltaPos1 = tempVertexArrays[1].m_position - tempVertexArrays[0].m_position;
	Vec3 deltaPos2 = tempVertexArrays[2].m_position - tempVertexArrays[0].m_position;

	Vec2 deltaUV1 = tempVertexArrays[1].m_uvTexCoords - tempVertexArrays[0].m_uvTexCoords;
	Vec2 deltaUV2 = tempVertexArrays[2].m_uvTexCoords - tempVertexArrays[0].m_uvTexCoords;

	float division = deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x;
	float r = 1.0f;
	if (division != 0.0f)
	{
		r = 1.0f/ division;
	}

	Vec3 tangent = (deltaPos1 * deltaUV2.x - deltaPos2 * deltaUV1.x) * r;
	Vec3 bitangent = (deltaPos2 * deltaUV1.y - deltaPos1 * deltaUV2.y) * r;

	for (int i = 0; i < 4; i++)
	{
		tempVertexArrays[i].m_tangent = tangent.GetNormalized();
		tempVertexArrays[i].m_bitangent = bitangent.GetNormalized();
		verts.push_back(tempVertexArrays[i]);

	}*/

	//------------------Proper Method--------------------
	int vertexCount = (int)positions.size();
	int totalTriangles = vertexCount / 3;

	for (int i = 0; i < totalTriangles; i++)
	{
		int triangleCount = i * 3;
		Vec3 v1 = positions[triangleCount];
		Vec3 v2 = positions[triangleCount + 1];
		Vec3 v3 = positions[triangleCount + 2];

		Vec2 w1 = uvs[triangleCount];
		Vec2 w2 = uvs[triangleCount + 1];
		Vec2 w3 = uvs[triangleCount + 2];

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

		tangents[triangleCount] = tang.GetNormalized();
		tangents[triangleCount + 1] = tang.GetNormalized();
		tangents[triangleCount + 2] = tang.GetNormalized();

		bitangents[triangleCount] = bitang.GetNormalized();
		bitangents[triangleCount + 1] = bitang.GetNormalized();
		bitangents[triangleCount + 2] = bitang.GetNormalized();
	}
	//----------Since triangles are index it goes 0,1,2,3. Which means the tangent of the 4th index is actually the 5th one in the array-----------
	//----------This is a hack--------------
	tangents[3] = tangents[5];
	bitangents[3] = bitangents[5];
	for (int i = 0; i < 4; i++)
	{
		tempVertexArrays[i].m_tangent = tangents[i].GetNormalized();
		tempVertexArrays[i].m_bitangent = bitangents[i].GetNormalized();
		verts.push_back(tempVertexArrays[i]);
	}

	indexedVerts.push_back(indexCount);
	indexedVerts.push_back(indexCount + 1);
	indexedVerts.push_back(indexCount + 2);
	indexedVerts.push_back(indexCount);
	indexedVerts.push_back(indexCount + 2);
	indexedVerts.push_back(indexCount + 3);

}

void AddVertsForIndexedAABB3Tangent(std::vector<Vertex_PNCUTB>& vertices, std::vector<unsigned int>& indexes, int& indexCount,
	AABB3 const& bounds, const Vec4& color, const AABB2& UVs)
{
	Vec3 bottomLeft, bottomRight, topLeft, topRight;
	float minx, miny, minz, maxx, maxy, maxz;

	minx = bounds.m_mins.x;
	miny = bounds.m_mins.y;
	minz = bounds.m_mins.z;
	maxx = bounds.m_maxs.x;
	maxy = bounds.m_maxs.y;
	maxz = bounds.m_maxs.z;

	bottomLeft = Vec3(maxx, maxy, minz);
	bottomRight = Vec3(maxx, miny, minz);
	topLeft = Vec3(minx, maxy, minz);
	topRight = Vec3(minx, miny, minz);
	
	Vec3 quadNormal;
	quadNormal = Vec3(0.0f,0.0f,-1.0f);

	//------------------------------------FLOOR WALL ---------------------------------------
	AddVertsForIndexedPNCUQuadtangent3D(vertices, indexes, quadNormal, indexCount, topLeft, bottomLeft, bottomRight, topRight, color,
		UVs);
	indexCount += 4;

	bottomLeft = Vec3(maxx, miny, maxz);
	bottomRight = Vec3(maxx, maxy, maxz);
	topLeft = Vec3(minx, miny, maxz);
	topRight = Vec3(minx, maxy, maxz);

	//----------------------------------ROOF WALL-----------------------------------------------
	quadNormal = Vec3(0.0f, 0.0f, 1.0f);

	AddVertsForIndexedPNCUQuadtangent3D(vertices, indexes, quadNormal, indexCount, topLeft, bottomLeft, bottomRight, topRight, color,
		UVs);
	indexCount += 4;

	//----------------------LEFT WALL-------------------------------

	bottomLeft = Vec3(maxx, maxy, minz);
	bottomRight = Vec3(minx, maxy, minz);
	topLeft = Vec3(maxx, maxy, maxz);
	topRight = Vec3(minx, maxy, maxz);

	quadNormal = Vec3(0.0f, 1.0f, 0.0f);
	AddVertsForIndexedPNCUQuadtangent3D(vertices, indexes, quadNormal, indexCount, topLeft, bottomLeft, bottomRight, topRight, color,
		UVs);
	indexCount += 4;

	//-------------------------RIGHT WALL-----------------------------
	bottomLeft = Vec3(minx, miny, minz);
	bottomRight = Vec3(maxx, miny, minz);
	topLeft = Vec3(minx, miny, maxz);
	topRight = Vec3(maxx, miny, maxz);

	quadNormal = Vec3(0.0f, -1.0f, 0.0f);
	AddVertsForIndexedPNCUQuadtangent3D(vertices, indexes, quadNormal, indexCount, topLeft, bottomLeft, bottomRight, topRight, color,
		UVs);
	indexCount += 4;

	//------------------------BACK  WALL---------------------------
	bottomLeft = Vec3(minx, maxy, minz);
	bottomRight = Vec3(minx, miny, minz);
	topLeft = Vec3(minx, maxy, maxz);
	topRight = Vec3(minx, miny, maxz);

	quadNormal = Vec3(-1.0f, 0.0f, 0.0f);
	AddVertsForIndexedPNCUQuadtangent3D(vertices, indexes, quadNormal, indexCount, topLeft, bottomLeft, bottomRight, topRight, color,
		UVs);
	indexCount += 4;
	//----------------------FRONT WALL------------------------------
	bottomLeft = Vec3(maxx, miny, minz);
	bottomRight = Vec3(maxx, maxy, minz);
	topLeft = Vec3(maxx, miny, maxz);
	topRight = Vec3(maxx, maxy, maxz);
	quadNormal = Vec3(1.0f, 0.0f, 0.0f);
	AddVertsForIndexedPNCUQuadtangent3D(vertices, indexes, quadNormal, indexCount, topLeft, bottomLeft, bottomRight, topRight, color,
		UVs);
	indexCount += 4;
}

void AddVertsForIndexedQuad3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexedVerts,int indexCount, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color, const AABB2& UVs)
{
	Vertex_PCU tempVertexArrays[4];
	tempVertexArrays[0].m_position = topLeft;
	tempVertexArrays[1].m_position = bottomLeft;
	tempVertexArrays[2].m_position = bottomRight;
	tempVertexArrays[3].m_position = topRight;

	tempVertexArrays[0].m_color = color;
	tempVertexArrays[1].m_color = color;
	tempVertexArrays[2].m_color = color;
	tempVertexArrays[3].m_color = color;


	tempVertexArrays[0].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	tempVertexArrays[1].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_mins.y);
	tempVertexArrays[2].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	tempVertexArrays[3].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_maxs.y);

	
	for (int i = 0; i < 4; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
	indexedVerts.push_back(indexCount);
	indexedVerts.push_back(indexCount+1);
	indexedVerts.push_back(indexCount+2);
	indexedVerts.push_back(indexCount);
	indexedVerts.push_back(indexCount+2);
	indexedVerts.push_back(indexCount+3);
}
void AddVertsForIndexedQuad3D(std::vector<Vertex_PCU>& verts, std::vector<int>& indexedVerts, int indexCount, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color, const AABB2& UVs)
{
	Vertex_PCU tempVertexArrays[4];
	tempVertexArrays[0].m_position = topLeft;
	tempVertexArrays[1].m_position = bottomLeft;
	tempVertexArrays[2].m_position = bottomRight;
	tempVertexArrays[3].m_position = topRight;

	tempVertexArrays[0].m_color = color;
	tempVertexArrays[1].m_color = color;
	tempVertexArrays[2].m_color = color;
	tempVertexArrays[3].m_color = color;


	tempVertexArrays[0].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_mins.y);
	tempVertexArrays[1].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	tempVertexArrays[2].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_maxs.y);
	tempVertexArrays[3].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);


	for (int i = 0; i < 4; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
	indexedVerts.push_back(indexCount);
	indexedVerts.push_back(indexCount + 1);
	indexedVerts.push_back(indexCount + 2);
	indexedVerts.push_back(indexCount);
	indexedVerts.push_back(indexCount + 2);
	indexedVerts.push_back(indexCount + 3);
}
void AddVertsForIndexedQuad3D(std::vector<Vertex_PNCU>& vertexes, std::vector<unsigned int>& indexedVerts, int indexCount, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color, const AABB2& UVs)
{
	Vertex_PNCU tempVertexArrays[4];
	tempVertexArrays[0].m_position = topLeft;
	tempVertexArrays[1].m_position = bottomLeft;
	tempVertexArrays[2].m_position = bottomRight;
	tempVertexArrays[3].m_position = topRight;

	tempVertexArrays[0].m_color = color;
	tempVertexArrays[1].m_color = color;
	tempVertexArrays[2].m_color = color;
	tempVertexArrays[3].m_color = color;


	tempVertexArrays[0].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	tempVertexArrays[1].m_uvTexCoords = Vec2(UVs.m_mins.x, UVs.m_mins.y);
	tempVertexArrays[2].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	tempVertexArrays[3].m_uvTexCoords = Vec2(UVs.m_maxs.x, UVs.m_maxs.y);

	Vec3 a = (bottomLeft - bottomRight).GetNormalized();
	Vec3 b = (topRight - bottomRight).GetNormalized();
	Vec3 normal = CrossProduct3D(b, a).GetNormalized();

	tempVertexArrays[0].m_normal = normal;
	tempVertexArrays[1].m_normal = normal;
	tempVertexArrays[2].m_normal = normal;
	tempVertexArrays[3].m_normal = normal;

	for (int i = 0; i < 4; i++)
	{
		vertexes.push_back(tempVertexArrays[i]);
	}
	indexedVerts.push_back(indexCount);
	indexedVerts.push_back(indexCount + 1);
	indexedVerts.push_back(indexCount + 2);
	indexedVerts.push_back(indexCount);
	indexedVerts.push_back(indexCount + 2);
	indexedVerts.push_back(indexCount + 3);
}


void AddVertsForAABB3D(std::vector<Vertex_PCU>& verts, const AABB3& bounds, const Rgba8& color, const AABB2& UVs)
{
	Vec3 bottomLeft;
	Vec3 bottomRight;
	Vec3 topRight;
	Vec3 topLeft;
	float minx = bounds.m_mins.x;
	float maxx = bounds.m_maxs.x;
	float miny = bounds.m_mins.y;
	float maxy = bounds.m_maxs.y;
	float minz = bounds.m_mins.z;
	float maxz = bounds.m_maxs.z;

	bottomLeft = Vec3(minx, maxy, minz);
	bottomRight = Vec3(minx, miny, minz);
	topLeft = Vec3(minx, maxy, maxz);
	topRight = Vec3(minx, miny, maxz);
	AddVertsForQuad3D(verts, topLeft, bottomLeft, bottomRight, topRight, color, UVs);

	bottomLeft = Vec3(maxx, miny, minz);
	bottomRight = Vec3(maxx, maxy, minz);
	topLeft = Vec3(maxx, miny, maxz);
	topRight = Vec3(maxx, maxy, maxz);
	AddVertsForQuad3D(verts, topLeft, bottomLeft, bottomRight, topRight, color, UVs);

	bottomLeft = Vec3(minx, miny, minz);
	bottomRight = Vec3(maxx, miny, minz);
	topLeft = Vec3(minx, miny, maxz);
	topRight = Vec3(maxx, miny, maxz);
	AddVertsForQuad3D(verts, topLeft, bottomLeft, bottomRight, topRight, color, UVs);

	bottomLeft = Vec3(maxx, maxy, minz);
	bottomRight = Vec3(minx, maxy, minz);
	topLeft = Vec3(maxx, maxy, maxz);
	topRight = Vec3(minx, maxy, maxz);
	AddVertsForQuad3D(verts, topLeft, bottomLeft, bottomRight, topRight, color, UVs);

	bottomLeft = Vec3(maxx, maxy, minz);
	bottomRight = Vec3(maxx, miny, minz);
	topLeft = Vec3(minx, maxy, minz);
	topRight = Vec3(minx, miny, minz);
	AddVertsForQuad3D(verts, topLeft, bottomLeft, bottomRight, topRight, color, UVs);

	bottomLeft = Vec3(minx, maxy, maxz);
	bottomRight = Vec3(minx, miny, maxz);
	topLeft = Vec3(maxx, maxy, maxz);
	topRight = Vec3(maxx, miny, maxz);
	AddVertsForQuad3D(verts, topLeft, bottomLeft, bottomRight, topRight, color, UVs);
}
void AddVertsForNormalAABB3D(std::vector<Vertex_PNCU>& verts, const AABB3& bounds, const Rgba8& color, const AABB2& UVs)
{
	Vec3 bottomLeft;
	Vec3 bottomRight;
	Vec3 topRight;
	Vec3 topLeft;
	float minx = bounds.m_mins.x;
	float maxx = bounds.m_maxs.x;
	float miny = bounds.m_mins.y;
	float maxy = bounds.m_maxs.y;
	float minz = bounds.m_mins.z;
	float maxz = bounds.m_maxs.z;

	bottomLeft = Vec3(minx, maxy, minz);
	bottomRight = Vec3(minx, miny, minz);
	topLeft = Vec3(minx, maxy, maxz);
	topRight = Vec3(minx, miny, maxz);
	AddVertsForRoundedQuad3D(verts, topLeft, bottomLeft, bottomRight, topRight, color, UVs);

	bottomLeft = Vec3(maxx, miny, minz);
	bottomRight = Vec3(maxx, maxy, minz);
	topLeft = Vec3(maxx, miny, maxz);
	topRight = Vec3(maxx, maxy, maxz);
	AddVertsForRoundedQuad3D(verts, topLeft, bottomLeft, bottomRight, topRight, color, UVs);

	bottomLeft = Vec3(minx, miny, minz);
	bottomRight = Vec3(maxx, miny, minz);
	topLeft = Vec3(minx, miny, maxz);
	topRight = Vec3(maxx, miny, maxz);
	AddVertsForRoundedQuad3D(verts, topLeft, bottomLeft, bottomRight, topRight, color, UVs);

	bottomLeft = Vec3(maxx, maxy, minz);
	bottomRight = Vec3(minx, maxy, minz);
	topLeft = Vec3(maxx, maxy, maxz);
	topRight = Vec3(minx, maxy, maxz);
	AddVertsForRoundedQuad3D(verts, topLeft, bottomLeft, bottomRight, topRight, color, UVs);

	bottomLeft = Vec3(maxx, maxy, minz);
	bottomRight = Vec3(maxx, miny, minz);
	topLeft = Vec3(minx, maxy, minz);
	topRight = Vec3(minx, miny, minz);
	AddVertsForRoundedQuad3D(verts, topLeft, bottomLeft, bottomRight, topRight, color, UVs);

	bottomLeft = Vec3(minx, maxy, maxz);
	bottomRight = Vec3(minx, miny, maxz);
	topLeft = Vec3(maxx, maxy, maxz);
	topRight = Vec3(maxx, miny, maxz);
	AddVertsForRoundedQuad3D(verts, topLeft, bottomLeft, bottomRight, topRight, color, UVs);
}

void AddVertsForSphere3D(std::vector<Vertex_PCU>& verts, const float& radius, const Vec3& center, const AABB2& UVs, const Rgba8& color)
{
	UNUSED((void)UVs);
	UNUSED((void)center);
	int longitudeSlices = 16;
	int latitudeSlices = 8;
	float yawIncrements = 360.0f / longitudeSlices;
	float pitchIncrements = 180.0f / latitudeSlices;
	Vec3 vertexPosition;
	Vertex_PCU vertex;
	for (float y = 0.0f; y < 360.0f; y += yawIncrements)
	{
		float yaw1Degrees = y;
		float yaw2Degrees = yaw1Degrees + yawIncrements;
		float cy1 = CosDegrees(yaw1Degrees);
		float sy1 = SinDegrees(yaw1Degrees);
		float cy2 = CosDegrees(yaw2Degrees);
		float sy2 = SinDegrees(yaw2Degrees);
		float u;
		float v;
		for (float p = -90.0f; p < 90.0f; p += pitchIncrements)
		{
			float pitch1Degrees = p ;
			float pitch2Degrees = pitch1Degrees + pitchIncrements;

			float cp1 = CosDegrees(pitch1Degrees);
			float sp1 = SinDegrees(pitch1Degrees);
			float cp2 = CosDegrees(pitch2Degrees);
			float sp2 = SinDegrees(pitch2Degrees);

			Vertex_PCU BottomLeft;
			Vertex_PCU BottomRight;
			Vertex_PCU TopLeft;
			Vertex_PCU TopRight;

			
			BottomLeft.m_position = Vec3(cy1 * cp2 , cp2 * sy1, -sp2) * radius;
			u = yaw1Degrees / 360.0f;
			v = RangeMap(pitch2Degrees, 90.0f, -90.0f, 0.0f, 1.0f);
			BottomLeft.m_uvTexCoords = Vec2(u, v);
			BottomLeft.m_color = color;

			
			BottomRight.m_position =  Vec3(cp2 * cy2, cp2 * sy2, -sp2) * radius;
			u = yaw2Degrees / 360.0f;
			v = RangeMap(pitch2Degrees, 90.0f, -90.0f, 0.0f, 1.0f);
			BottomRight.m_uvTexCoords = Vec2(u, v);
			BottomRight.m_color = color;

			
			TopLeft.m_position = Vec3(cp1 * cy1, cp1 * sy1, -sp1) * radius;
			u = yaw1Degrees / 360.0f;
			v = RangeMap(pitch1Degrees, 90.0f, -90.0f, 0.0f, 1.0f);
			TopLeft.m_uvTexCoords = Vec2(u, v);
			TopLeft.m_color = color;

			TopRight.m_position = Vec3(cp1 * cy2, cp1 * sy2, -sp1) * radius;
			u = yaw2Degrees / 360.0f;
			v = RangeMap(pitch1Degrees, 90.0f, -90.0f, 0.0f, 1.0f);
			TopRight.m_uvTexCoords = Vec2(u, v);
			TopRight.m_color = color;

			verts.push_back(BottomLeft);
			verts.push_back(BottomRight);
			verts.push_back(TopLeft);
			verts.push_back(TopLeft);
			verts.push_back(BottomRight);
			verts.push_back(TopRight);
		}
	}
}
void AddVertsForNormalSphere3D(std::vector<Vertex_PNCU>& verts, const float& radius, const Vec3& center, const AABB2& UVs, const Rgba8& color)
{
	UNUSED((void)UVs);
	int longitudeSlices = 16;
	int latitudeSlices = 8;
	float yawIncrements = 360.0f / longitudeSlices;
	float pitchIncrements = 180.0f / latitudeSlices;
	Vec3 vertexPosition;
	Vertex_PCU vertex;
	for (float y = 0.0f; y < 360.0f; y += yawIncrements)
	{
		float yaw1Degrees = y;
		float yaw2Degrees = yaw1Degrees + yawIncrements;
		float cy1 = CosDegrees(yaw1Degrees);
		float sy1 = SinDegrees(yaw1Degrees);
		float cy2 = CosDegrees(yaw2Degrees);
		float sy2 = SinDegrees(yaw2Degrees);
		float u;
		float v;
		for (float p = -90.0f; p < 90.0f; p += pitchIncrements)
		{
			float pitch1Degrees = p;
			float pitch2Degrees = pitch1Degrees + pitchIncrements;

			float cp1 = CosDegrees(pitch1Degrees);
			float sp1 = SinDegrees(pitch1Degrees);
			float cp2 = CosDegrees(pitch2Degrees);
			float sp2 = SinDegrees(pitch2Degrees);

			Vertex_PNCU BottomLeft;
			Vertex_PNCU BottomRight;
			Vertex_PNCU TopLeft;
			Vertex_PNCU TopRight;


			BottomLeft.m_position = center + Vec3(cy1 * cp2, cp2 * sy1, -sp2) * radius;
			u = yaw1Degrees / 360.0f;
			v = RangeMap(pitch2Degrees, 90.0f, -90.0f, 0.0f, 1.0f);
			BottomLeft.m_uvTexCoords = Vec2(u, v);
			BottomLeft.m_color = color;


			BottomRight.m_position = center + Vec3(cp2 * cy2, cp2 * sy2, -sp2) * radius;
			u = yaw2Degrees / 360.0f;
			v = RangeMap(pitch2Degrees, 90.0f, -90.0f, 0.0f, 1.0f);
			BottomRight.m_uvTexCoords = Vec2(u, v);
			BottomRight.m_color = color;


			TopLeft.m_position = center +  Vec3(cp1 * cy1, cp1 * sy1, -sp1) * radius;
			u = yaw1Degrees / 360.0f;
			v = RangeMap(pitch1Degrees, 90.0f, -90.0f, 0.0f, 1.0f);
			TopLeft.m_uvTexCoords = Vec2(u, v);
			TopLeft.m_color = color;

			TopRight.m_position = center + Vec3(cp1 * cy2, cp1 * sy2, -sp1) * radius;
			u = yaw2Degrees / 360.0f;
			v = RangeMap(pitch1Degrees, 90.0f, -90.0f, 0.0f, 1.0f);
			TopRight.m_uvTexCoords = Vec2(u, v);
			TopRight.m_color = color;

			BottomLeft.m_normal = (center - BottomLeft.m_position).GetNormalized();
			TopLeft.m_normal = (center - TopLeft.m_position).GetNormalized();
			TopRight.m_normal = (center - TopRight.m_position).GetNormalized();
			BottomRight.m_normal = (center - BottomRight.m_position).GetNormalized();

			verts.push_back(BottomLeft);
			verts.push_back(BottomRight);
			verts.push_back(TopLeft);
			verts.push_back(TopLeft);
			verts.push_back(BottomRight);
			verts.push_back(TopRight);

			
		}
	}
}

void AddVertsForIndexedNormalSphere3D(std::vector<Vertex_PNCUTB>& verts, std::vector<unsigned int>& indices, const float& radius, const Vec3& center, const AABB2& UVs, Vec4 const& color, int& indexCount)
{	
	UNUSED((void )indexCount);
	//int longitudeSlices = 16;
	//int latitudeSlices = 8;
	//float yawIncrements = 360.0f / longitudeSlices;
	//float pitchIncrements = 180.0f / latitudeSlices;
	//Vec3 vertexPosition;
	//Vertex_PCU vertex;
	//for (float y = 0.0f; y < 360.0f; y += yawIncrements)
	//{
	//	float yaw1Degrees = y;
	//	float yaw2Degrees = yaw1Degrees + yawIncrements;
	//	float cy1 = CosDegrees(yaw1Degrees);
	//	float sy1 = SinDegrees(yaw1Degrees);
	//	float cy2 = CosDegrees(yaw2Degrees);
	//	float sy2 = SinDegrees(yaw2Degrees);
	//	float u;
	//	float v;
	//	for (float p = -90.0f; p < 90.0f; p += pitchIncrements)
	//	{
	//		float pitch1Degrees = p;
	//		float pitch2Degrees = pitch1Degrees + pitchIncrements;

	//		float cp1 = CosDegrees(pitch1Degrees);
	//		float sp1 = SinDegrees(pitch1Degrees);
	//		float cp2 = CosDegrees(pitch2Degrees);
	//		float sp2 = SinDegrees(pitch2Degrees);

	//		Vertex_PNCUTB BottomLeft;
	//		Vertex_PNCUTB BottomRight;
	//		Vertex_PNCUTB TopLeft;
	//		Vertex_PNCUTB TopRight;

	//		BottomLeft.m_position = center;
	//		BottomRight.m_position = center;
	//		TopLeft.m_position = center;
	//		TopRight.m_position = center;

	//		BottomLeft.m_position += Vec3(cy1 * cp2, cp2 * sy1, -sp2) * radius;
	//		u = yaw1Degrees / 360.0f;
	//		v = RangeMap(pitch2Degrees, 90.0f, -90.0f, 0.0f, 1.0f);
	//		BottomLeft.m_uvTexCoords = Vec2(u, v);
	//		BottomLeft.m_uvTexCoords = Vec2();
	//		BottomLeft.m_color = color;


	//		BottomRight.m_position += Vec3(cp2 * cy2, cp2 * sy2, -sp2) * radius;
	//		u = yaw2Degrees / 360.0f;
	//		v = RangeMap(pitch2Degrees, 90.0f, -90.0f, 0.0f, 1.0f);
	//		BottomRight.m_uvTexCoords = Vec2(u, v);
	//		BottomRight.m_uvTexCoords = Vec2();
	//		BottomRight.m_color = color;


	//		TopLeft.m_position += Vec3(cp1 * cy1, cp1 * sy1, -sp1) * radius;
	//		u = yaw1Degrees / 360.0f;
	//		v = RangeMap(pitch1Degrees, 90.0f, -90.0f, 0.0f, 1.0f);
	//		TopLeft.m_uvTexCoords = Vec2(u, v);
	//		TopLeft.m_uvTexCoords = Vec2();
	//		TopLeft.m_color = color;

	//		TopRight.m_position += Vec3(cp1 * cy2, cp1 * sy2, -sp1) * radius;
	//		u = yaw2Degrees / 360.0f;
	//		v = RangeMap(pitch1Degrees, 90.0f, -90.0f, 0.0f, 1.0f);
	//		TopRight.m_uvTexCoords = Vec2(u, v);
	//		TopRight.m_uvTexCoords = Vec2();
	//		TopRight.m_color = color;

	//		BottomLeft.m_normal = (center - BottomLeft.m_position).GetNormalized();
	//		TopLeft.m_normal = (center - TopLeft.m_position).GetNormalized();
	//		TopRight.m_normal = (center - TopRight.m_position).GetNormalized();
	//		BottomRight.m_normal = (center - BottomRight.m_position).GetNormalized();

	//		verts.push_back(BottomLeft);
	//		verts.push_back(BottomRight);
	//		verts.push_back(TopLeft);
	//		//verts.push_back(TopLeft);
	//		//verts.push_back(BottomRight);
	//		verts.push_back(TopRight);

	//		indices.push_back(indexCount);
	//		indices.push_back(indexCount + 1);
	//		indices.push_back(indexCount + 2);
	//		indices.push_back(indexCount + 2);
	//		indices.push_back(indexCount + 1);
	//		indices.push_back(indexCount + 3);
	//		indexCount = indexCount + 4;
	//	}
	//}

	UNUSED((void)UVs);
	int longitudeSlices = 16;
	int latitudeSlices = 8;
	float yawIncrements = 360.0f / longitudeSlices;
	float pitchIncrements = 180.0f / latitudeSlices;
	Vec3 vertexPosition;
	Vertex_PCU vertex;
	for (float y = 0.0f; y < 360.0f; y += yawIncrements)
	{
		float yaw1Degrees = y;
		float yaw2Degrees = yaw1Degrees + yawIncrements;
		float cy1 = CosDegrees(yaw1Degrees);
		float sy1 = SinDegrees(yaw1Degrees);
		float cy2 = CosDegrees(yaw2Degrees);
		float sy2 = SinDegrees(yaw2Degrees);
		float u;
		float v;
		for (float p = -90.0f; p < 90.0f; p += pitchIncrements)
		{
			float pitch1Degrees = p;
			float pitch2Degrees = pitch1Degrees + pitchIncrements;

			float cp1 = CosDegrees(pitch1Degrees);
			float sp1 = SinDegrees(pitch1Degrees);
			float cp2 = CosDegrees(pitch2Degrees);
			float sp2 = SinDegrees(pitch2Degrees);

			Vertex_PNCUTB BottomLeft;
			Vertex_PNCUTB BottomRight;
			Vertex_PNCUTB TopLeft;
			Vertex_PNCUTB TopRight;


			BottomLeft.m_position = center + Vec3(cy1 * cp2, cp2 * sy1, -sp2) * radius;
			u = yaw1Degrees / 360.0f;
			v = RangeMap(pitch2Degrees, 90.0f, -90.0f, 0.0f, 1.0f);
			BottomLeft.m_uvTexCoords = Vec2(u, v);
			BottomLeft.m_color = color;


			BottomRight.m_position = center + Vec3(cp2 * cy2, cp2 * sy2, -sp2) * radius;
			u = yaw2Degrees / 360.0f;
			v = RangeMap(pitch2Degrees, 90.0f, -90.0f, 0.0f, 1.0f);
			BottomRight.m_uvTexCoords = Vec2(u, v);
			BottomRight.m_color = color;


			TopLeft.m_position = center + Vec3(cp1 * cy1, cp1 * sy1, -sp1) * radius;
			u = yaw1Degrees / 360.0f;
			v = RangeMap(pitch1Degrees, 90.0f, -90.0f, 0.0f, 1.0f);
			TopLeft.m_uvTexCoords = Vec2(u, v);
			TopLeft.m_color = color;

			TopRight.m_position = center + Vec3(cp1 * cy2, cp1 * sy2, -sp1) * radius;
			u = yaw2Degrees / 360.0f;
			v = RangeMap(pitch1Degrees, 90.0f, -90.0f, 0.0f, 1.0f);
			TopRight.m_uvTexCoords = Vec2(u, v);
			TopRight.m_color = color;

			BottomLeft.m_normal = (center - BottomLeft.m_position).GetNormalized();
			TopLeft.m_normal = (center - TopLeft.m_position).GetNormalized();
			TopRight.m_normal = (center - TopRight.m_position).GetNormalized();
			BottomRight.m_normal = (center - BottomRight.m_position).GetNormalized();

			//BottomLeft.m_uvTexCoords.y = 1 - BottomLeft.m_uvTexCoords.y;
			//TopLeft.m_uvTexCoords.y = 1 - TopLeft.m_uvTexCoords.y;
			//TopRight.m_uvTexCoords.y = 1 - TopRight.m_uvTexCoords.y;
			//BottomRight.m_uvTexCoords.y = 1 - BottomRight.m_uvTexCoords.y;


			verts.push_back(BottomLeft);
			verts.push_back(BottomRight);
			verts.push_back(TopLeft);
			verts.push_back(TopLeft);
			verts.push_back(BottomRight);
			verts.push_back(TopRight);
		}

	}

	for (int i = 0; i < verts.size(); i++)
	{
		indices.push_back(i);
	}
}
void AddVertsForZCylinder(std::vector<Vertex_PCU>&verts,const float& height, const float& radius, const AABB2 & UVs, const Rgba8 & color)
{
	UNUSED((void)UVs);
	int longitudeSlices = 16;
	float yawIncrements = 360.0f / longitudeSlices;
	float y;
	for (y = 0.0f; y < 360.0f; y += yawIncrements)
	{
		float u, v;
		float yaw1Degrees = y;
		float yaw2Degrees = yaw1Degrees + yawIncrements;
		float cy1 = CosDegrees(yaw1Degrees);
		float sy1 = SinDegrees(yaw1Degrees);
		float cy2 = CosDegrees(yaw2Degrees);
		float sy2 = SinDegrees(yaw2Degrees);

		Vertex_PCU BottomLeft;
		Vertex_PCU BottomRight;
		Vertex_PCU TopLeft;
		Vertex_PCU TopRight;

		BottomLeft.m_position = Vec3(cy1 * radius, sy1 * radius, 0.0f) ;
		u = yaw1Degrees / 360.0f;
		v = 0.0f;
		BottomLeft.m_uvTexCoords = Vec2(u, v);
		BottomLeft.m_color = color;


		BottomRight.m_position = Vec3(cy2 * radius,sy2 * radius, 0.0f) ;
		u = yaw2Degrees / 360.0f;
		v = 0.0f;
		BottomRight.m_uvTexCoords = Vec2(u, v);
		BottomRight.m_color = color;


		TopLeft.m_position = Vec3(cy1 * radius, sy1 * radius, height) ;
		u = yaw1Degrees / 360.0f;
		v = 1.0f;
		TopLeft.m_uvTexCoords = Vec2(u, v);
		TopLeft.m_color = color;

		TopRight.m_position = Vec3(cy2 * radius, sy2 * radius, height) ;
		u = yaw2Degrees / 360.0f;
		v = 1.0f;
		TopRight.m_uvTexCoords = Vec2(u, v);
		TopRight.m_color = color;

		verts.push_back(BottomLeft);
		verts.push_back(BottomRight);
		verts.push_back(TopLeft);
		verts.push_back(TopLeft);
		verts.push_back(BottomRight);
		verts.push_back(TopRight);
	}

	Vec3 center = Vec3(0.0f, 0.0f, height);
	const float totalDegrees = 360.0f;
	const int totalSides = 16;
	const int totalVertices = totalSides * 3;

	float splitDegrees = totalDegrees / (float)totalSides;
	float radiusOuter = radius;
	Vertex_PCU tempVertexArrays[totalVertices];
	for (int sideIndex = 0; sideIndex < totalSides; sideIndex++)
	{
		float point1degrees = splitDegrees * sideIndex;
		float point2Degrees = splitDegrees * (sideIndex + 1);
		Vec3 startPoint1 = center;
		Vec3 endPoint1 = Vec3(center.x + (CosDegrees(point1degrees) * radiusOuter), center.y + (SinDegrees(point1degrees) * radiusOuter), center.z);
		Vec3 endPoint2 = Vec3(center.x + (CosDegrees(point2Degrees) * radiusOuter), center.y + (SinDegrees(point2Degrees) * radiusOuter), center.z);
		int boxIndex = sideIndex * 3;
		tempVertexArrays[boxIndex].m_position = startPoint1;
		tempVertexArrays[boxIndex + 1].m_position = endPoint1;
		tempVertexArrays[boxIndex + 2].m_position = endPoint2;

		tempVertexArrays[boxIndex].m_color = color;
		tempVertexArrays[boxIndex + 1].m_color = color;
		tempVertexArrays[boxIndex + 2].m_color = color;

		tempVertexArrays[boxIndex].m_uvTexCoords = Vec2(UVs.m_mins.x + 0.5f, UVs.m_mins.y + 0.5f);
		tempVertexArrays[boxIndex + 1].m_uvTexCoords = Vec2(UVs.m_mins.x + 0.5f + (CosDegrees(point1degrees + 90.0f)* 0.5f), UVs.m_mins.y + 0.5f + (SinDegrees(point1degrees + 90.0f) * 0.5f));
		tempVertexArrays[boxIndex + 2].m_uvTexCoords = Vec2(UVs.m_mins.x + 0.5f + (CosDegrees(point2Degrees + 90.0f) * 0.5f), UVs.m_mins.y + 0.5f + (SinDegrees(point2Degrees + 90.0f) * 0.5f));

	}
	for (int i = 0; i < totalVertices; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
	center = Vec3(0.0f, 0.0f, 0.0f);
	for (int sideIndex = totalSides -1; sideIndex >= 0; sideIndex--)
	{
		float point1degrees = splitDegrees * sideIndex;
		float point2Degrees = splitDegrees * (sideIndex + 1);
		Vec3 startPoint1 = center;
		Vec3 endPoint1 = Vec3(center.x + (CosDegrees(point1degrees) * radiusOuter), center.y + (SinDegrees(point1degrees) * radiusOuter), center.z);
		Vec3 endPoint2 = Vec3(center.x + (CosDegrees(point2Degrees) * radiusOuter), center.y + (SinDegrees(point2Degrees) * radiusOuter), center.z);
		int boxIndex = sideIndex * 3;
		tempVertexArrays[boxIndex].m_position = startPoint1;
		tempVertexArrays[boxIndex + 1].m_position = endPoint2;
		tempVertexArrays[boxIndex + 2].m_position = endPoint1;

		tempVertexArrays[boxIndex].m_color = color;
		tempVertexArrays[boxIndex + 1].m_color = color;
		tempVertexArrays[boxIndex + 2].m_color = color;

		tempVertexArrays[boxIndex].m_uvTexCoords = Vec2(UVs.m_mins.x + 0.5f, UVs.m_mins.y + 0.5f);
		tempVertexArrays[boxIndex + 1].m_uvTexCoords = Vec2(UVs.m_mins.x + 0.5f - (CosDegrees(point2Degrees - 90.0f) * 0.5f), UVs.m_mins.y + 0.5f + (SinDegrees(point2Degrees - 90.0f) * 0.5f));
		tempVertexArrays[boxIndex + 2].m_uvTexCoords = Vec2(UVs.m_mins.x + 0.5f - (CosDegrees(point1degrees - 90.0f) *  0.5f), UVs.m_mins.y + 0.5f + (SinDegrees(point1degrees - 90.0f) * 0.5f));

	}
	for (int i = 0; i < totalVertices; i++)
	{
		verts.push_back(tempVertexArrays[i]);
	}
	  
}
void AddVertsForArrow3D(std::vector<Vertex_PCU>& verts, const float& height, const float& radius, const AABB2& UVs, const Rgba8& color)
{
	UNUSED((void)color);
	UNUSED((void)height);
	UNUSED((void)radius);
	UNUSED((void)color);
	UNUSED((void)UVs);
	UNUSED((void)verts);
}
void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, const float& height, const float& radius, const AABB2& UVs, const Rgba8& color)
{
	UNUSED((void)UVs);
	int longitudeSlices = 16;
	float yawIncrements = 360.0f / longitudeSlices;
	float y;
	for (y = 0.0f; y < 360.0f; y += yawIncrements)
	{
		float u, v;
		float yaw1Degrees = y;
		float yaw2Degrees = yaw1Degrees + yawIncrements;
		float cy1 = CosDegrees(yaw1Degrees);
		float sy1 = SinDegrees(yaw1Degrees);
		float cy2 = CosDegrees(yaw2Degrees);
		float sy2 = SinDegrees(yaw2Degrees);

		Vertex_PCU BottomLeft;
		Vertex_PCU BottomRight;
		Vertex_PCU TopLeft;
		Vertex_PCU TopRight;
		Vertex_PCU center;

		BottomLeft.m_position = Vec3(cy1 * radius, sy1 * radius , 0.0f);
		u = yaw1Degrees / 360.0f;
		v = RangeMap(0.0f, 90.0f, -90.0f, 0.0f, 1.0f);
		BottomLeft.m_uvTexCoords = Vec2(u, v);
		BottomLeft.m_color = color;


		BottomRight.m_position = Vec3(cy2 * radius, sy2 * radius , 0.0f);
		u = yaw2Degrees / 360.0f;
		v = RangeMap(0.0f, 90.0f, -90.0f, 0.0f, 1.0f);
		BottomRight.m_uvTexCoords = Vec2(u, v);
		BottomRight.m_color = color;


		TopLeft.m_position = Vec3(0.0f, 0.0f, height/5.0f);
		u = yaw1Degrees / 360.0f;
		v = RangeMap(height, 90.0f, -90.0f, 0.0f, 1.0f);
		TopLeft.m_uvTexCoords = Vec2(u, v);
		TopLeft.m_color = color;

		TopRight.m_position = Vec3(0.0f, 0.0f, height / 5.0f);
		u = yaw2Degrees / 360.0f;
		v = RangeMap(height, 90.0f, -90.0f, 0.0f, 1.0f);
		TopRight.m_uvTexCoords = Vec2(u, v);
		TopRight.m_color = color;

		center.m_position = Vec3(0.0f, 0.0f,0.0f);
		u = yaw2Degrees / 360.0f;
		v = RangeMap(height, 90.0f, -90.0f, 0.0f, 1.0f);
		TopRight.m_uvTexCoords = Vec2(u, v);
		TopRight.m_color = color;

		verts.push_back(BottomLeft);
		verts.push_back(BottomRight);
		verts.push_back(TopLeft);
		verts.push_back(TopLeft);
		verts.push_back(BottomRight);
		verts.push_back(TopRight);

		verts.push_back(BottomLeft);
		verts.push_back(center);
		verts.push_back(BottomRight);
	}
}
AABB3 GetVertexBounds(std::vector<Vertex_PCU> verts) 
{
	AABB3 bounds;
	bounds.m_mins = verts[0].m_position;
	bounds.m_maxs = verts[verts.size()-1].m_position;
	return bounds;
}

AABB3 GetVertexBounds(std::vector<Vertex_PNCU> verts)
{
	AABB3 bounds;
	bounds.m_mins = verts[0].m_position;
	bounds.m_maxs = verts[verts.size() - 1].m_position;
	return bounds;
}
