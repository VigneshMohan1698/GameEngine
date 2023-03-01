#include "AABB3.hpp"

const AABB3 AABB3::ZERO_TO_ONE = AABB3(0.0f, 0.0f,0.0f,1.0f, 1.0f, 1.0f);
AABB3::~AABB3()
{
}

AABB3::AABB3(AABB3 const& copyForm)
{
	m_mins = copyForm.m_mins;
	m_maxs = copyForm.m_maxs;
}

AABB3::AABB3(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	m_mins.x = minX;
	m_mins.y = minY;
	m_mins.z = minZ;

	m_maxs.x = maxX;
	m_maxs.y = maxY;
	m_maxs.z = maxZ;
}

AABB3::AABB3(Vec3 const& mins, Vec3 const& maxs)
{
	m_mins = mins;
	m_maxs = maxs;
}

AABB3 AABB3::operator+(Vec3 positionToAdd)
{
	AABB3 addedValue;
	addedValue.m_mins = m_mins +  positionToAdd;
	addedValue.m_maxs = m_maxs + positionToAdd;

	return addedValue;
}

Vec3 AABB3::GetCenterOfAABB3() const
{
	Vec3 center;
	center.x = (m_mins.x + m_maxs.x) / 2;
	center.y = (m_mins.y + m_maxs.y) / 2;
	center.z = (m_mins.z + m_maxs.z) / 2;
	return center;
}
Vec3 AABB3::GetDimensions() const
{
	float dimensionX = m_maxs.x - m_mins.x;
	float dimensionY = m_maxs.y - m_mins.y;
	float dimensionZ = m_maxs.z - m_mins.z;
	return Vec3(dimensionX, dimensionY, dimensionZ);
}

Vec3 AABB3::GetCenter() const
{
	Vec3 displacement = (m_maxs - m_mins) / 2.0f;
	return m_mins + displacement;
}
