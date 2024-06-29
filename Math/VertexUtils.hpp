#pragma once
#include "Engine/Core/Vertex_PCU.hpp"
#include <vector>
#include "Engine/Math/Capsule2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/LineSegment2.hpp"
#include <string>
#include "Engine/Math/AABB3.hpp"
#include "Engine/Core/Vertex_PNCUTB.hpp"
#include "Engine/Core/Vertex_PNCU.hpp"
#include "Engine/Math/ConvexShapes.hpp"

struct Mat44;

void TransformVertexArray3D(int vertices, Vertex_PCU* positionVertexArrays, float uniformScale, float rotationDegrees, const Vec3& translatePos);
void TransformVertexArrayUsingMatrix3D(int vertices, std::vector<Vertex_PCU>& positionVertexArrays, Mat44 transformMatrix);
void TransformPNCUTBArrayUsingMatrix3D(int vertices, std::vector<Vertex_PNCUTB>& positionVertexArrays, Mat44 transformMatrix);
void TransformVertexArrayUsingMatrix3D(int vertices, std::vector<Vertex_PNCU>& positionVertexArrays, Mat44 transformMatrix);
void TransformVertexArrayUsingMatrix3D(int vertices, std::vector<Vertex_PNCUTB>& positionVertexArrays, Mat44 transformMatrix);
void TransformTranslateVertexArrayUsingMatrix3D(int vertices, std::vector<Vertex_PCU>& positionVertexArrays, Mat44 transformMatrix);
void TransformTranslateVertexArrayUsingMatrix3D(int vertices, std::vector<Vertex_PNCU>& positionVertexArrays, Mat44 transformMatrix);

void TransformPosition3D(Vec3& posToTransform, float uniformScale, float rotationDegrees, const Vec3& translatePos);
void TransformVertexArray3D(int vertices, std::vector<Vertex_PCU>& positionVertexArrays, float uniformScale, float rotationDegrees, const Vec3& translatePos);
void TransformVertexArray3D(int vertices, std::vector<Vertex_PNCUTB>& positionVertexArrays, float uniformScale, float rotationDegrees, const Vec3& translatePos);

//-------------------------ADD VERTS FUNCTIONS 2D--------------------------------------------------
void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Capsule2 const& capsule, Rgba8 const& color);
void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Vec2 const& boneStart, Vec2 const& boneEnd,float radius, Rgba8 const& color);
void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, Rgba8 const& color);
void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, int divisions, Rgba8 const& color);
void AddVertsForHalfDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float discOrientation, float radius, Rgba8 const& color);
void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Rgba8 const& color);
void AddVertsForAABB2DWireFrame(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, float thickness, Rgba8 const& color);
void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Vec2 UvAtMins, Vec2 UvAtMaxs, Rgba8 const& color);
void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, AABB2 const& UvBounds, Rgba8 const& color);
void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, OBB2 const& bounds, Rgba8 const& color);
void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, OBB2 const& bounds, AABB2 uvBounds, Rgba8 const& color);
void AddVertsForConvexPoly2D(std::vector<Vertex_PCU>& verts,ConvexPoly2D const& poly2D,float edgeThickness, AABB2 uvBounds, Rgba8 const& color);
void AddVertsForDebugDisk(std::vector<Vertex_PCU>& verts, Vec2 center, float radius, Rgba8 color, float thickness);

void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, LineSegment2 const& lineSegment, float thickness, Rgba8 const& color);
void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, Vec2 const& start, Vec2 const& end,float thickness, Rgba8 const& color);
void AddVertsForRing2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, float ringThickness, Rgba8 const& color);
void AddVertsForTexts2D(std::vector<Vertex_PCU>& vertexArray, Vec2 const& textMins, AABB2 const& UvBounds,
	float cellHeight, std::string const& text, Rgba8 const& tint = Rgba8::WHITE, float cellAspect = 1.f);
void AddVertsForArrow2D(std::vector<Vertex_PCU>& verts, Vec2 tailPos, Vec2 tipPos, float arrowSize, float lineThickness, Rgba8 color);
void AddVertsForIndexedNormalSphere3D(std::vector<Vertex_PNCUTB>& verts, std::vector<unsigned int>& indices, const float& radius, const Vec3& center, const AABB2& UVs, Vec4 const& color, int indexCount = 0 );
//-------------------------ADD VERTS FUNCTIONS 3D--------------------------------------------------
void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForQuadPNCU3D(std::vector<Vertex_PNCU>& verts, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForQuadPNCU3D(std::vector<Vertex_PNCU>& verts,Vec3 normal, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, const Vec3& m_mins, const Vec3& m_maxs,const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForRoundedQuad3D(std::vector<Vertex_PNCU>& verts, const Vec3& m_mins, const Vec3& m_maxs, const Rgba8& color, const AABB2& UVs);
void AddVertsForRoundedQuad3D(std::vector<Vertex_PNCU>& vertexes, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForIndexedPNCUQuad3D(std::vector<Vertex_PNCU>& verts, std::vector<unsigned int>& indexedVerts, Vec3& normal, int indexCount, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color, const AABB2& UVs);
void AddVertsForIndexedPNCUQuadtangent3D(std::vector<Vertex_PNCUTB>& verts, std::vector<unsigned int>& indexedVerts, Vec3& normal, int indexCount, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Vec4& color, const AABB2& UVs);
void AddVertsForIndexedPNCUQuadtangent3D(std::vector<Vertex_PNCUTB>& verts, std::vector<unsigned int>& indexedVerts, Vec3& normal, int indexCount,
	AABB3 bounds, const Vec4& color, const AABB2& UVs);
void AddVertsForIndexedAABB3Tangent(std::vector<Vertex_PNCUTB>& verts, std::vector<unsigned int>& indexedVerts, int& indexCount,AABB3 const& bounds, const Vec4& color, const AABB2& UVs);

void AddVertsForIndexedQuad3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexedVerts,int indexCount ,const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForIndexedQuad3D(std::vector<Vertex_PCU>& verts, std::vector<int>& indexedVerts,int indexCount ,const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForIndexedQuad3D(std::vector<Vertex_PNCU>& vertexes, std::vector<unsigned int>& indexedVerts, int indexCount, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);

void AddVertsForAABB3D(std::vector<Vertex_PCU>& verts, const AABB3& bounds, const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForNormalAABB3D(std::vector<Vertex_PNCU>& verts, const AABB3& bounds, const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForNormalAABB3D(std::vector<Vertex_PNCU>& verts, Vec3 normal, const AABB3& bounds, const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForSphere3D(std::vector<Vertex_PCU>& verts, const float& radius, const Vec3& center, const AABB2& UVs, const Rgba8& color);
void AddVertsForNormalSphere3D(std::vector<Vertex_PNCU>& verts, const float& radius, const Vec3& center, const AABB2& UVs, const Rgba8& color);
void AddVertsForNormalSphere3D(std::vector<Vertex_PNCUTB>& verts, std::vector<unsigned int> indexes, const float& radius, const Vec3& center, const AABB2& UVs, const Rgba8& color);
void AddVertsForZCylinder(std::vector<Vertex_PCU>& verts, const float& height, const float& radius, const AABB2& UVs, const Rgba8& color);
void AddVertsForArrow3D(std::vector<Vertex_PCU>& verts, const float& height, const float& radius, const AABB2& UVs, const Rgba8& color);
void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, const float& height, const float& radius, const AABB2& UVs, const Rgba8& color);

AABB3 GetVertexBounds(std::vector<Vertex_PCU> verts) ;
AABB3 GetVertexBounds(std::vector<Vertex_PNCU> verts) ;