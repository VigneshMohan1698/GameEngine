#pragma once
#include "Engine/Core/Vertex_PCU.hpp"

struct AABB3
{
const static AABB3 ZERO_TO_ONE;
public:
	Vec3 m_mins;
	Vec3 m_maxs;

public:
	~AABB3();
	AABB3() {}
	AABB3(AABB3 const& copyForm);
	explicit AABB3(float minX, float minY, float maxX, float maxY, float minZ, float maxZ);
	explicit AABB3(Vec3 const& mins, Vec3 const& maxs);

	AABB3			operator+(Vec3 positionToAdd);
	Vec3			GetCenterOfAABB3() const;
	Vec3			GetDimensions() const;
	Vec3			GetCenter() const;
};
	