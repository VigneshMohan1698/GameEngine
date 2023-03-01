#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vec2.hpp"

const AABB2 AABB2::ZERO_TO_ONE = AABB2(0.0f, 0.0f, 1.0f, 1.0f);
AABB2::~AABB2()
{
}

AABB2::AABB2(AABB2 const& copyForm)
{
	m_mins = copyForm.m_mins;
	m_maxs = copyForm.m_maxs;
}

AABB2::AABB2(float minX, float minY, float maxX, float maxY)
{
	m_mins.x = minX;
	m_mins.y = minY;
	
	m_maxs.x = maxX;
	m_maxs.y = maxY;
}

AABB2::AABB2(Vec2 const& mins, Vec2 const& maxs)
{
	m_mins = mins;
	m_maxs = maxs;
}
AABB2::AABB2(IntVec2 const& mins, IntVec2 const& maxs)
{
	m_mins = Vec2(mins);
	m_maxs = Vec2(maxs);
}
bool AABB2::IsPointInside(Vec2 const& point) const
{
	if (point.x >= m_mins.x && point.y >= m_mins.y &&
		point.x <= m_maxs.x && point.y <= m_maxs.y)
	{
		return true;
	}
	else return false;
}

Vec2 AABB2::GetCenter() const
{
	float centerX = (m_mins.x + m_maxs.x) * 0.5f;
	float centerY = (m_mins.y + m_maxs.y) * 0.5f;
	return  Vec2(centerX, centerY);
}

Vec2 AABB2::GetDimensions() const
{
	float dimensionX = m_maxs.x - m_mins.x;
	float dimensionY = m_maxs.y - m_mins.y;
	return Vec2(dimensionX, dimensionY);
}

Vec2 AABB2::GetNearestPoint(Vec2 const& referencePoint) const
{
	if (referencePoint.x >= m_maxs.x && referencePoint.y >= m_maxs.y)
		return m_maxs;
	else if (referencePoint.x >= m_maxs.x && referencePoint.y <= m_mins.y)
		return Vec2(m_maxs.x, m_mins.y);
	else if (referencePoint.x <= m_mins.x && referencePoint.y <= m_mins.y)
		return m_mins;
	else if (referencePoint.x <= m_mins.x && referencePoint.y >= m_maxs.y)
		return Vec2(m_mins.x, m_maxs.y);
	else if (referencePoint.x < m_mins.x)
		return Vec2(m_mins.x, referencePoint.y);
	else if (referencePoint.y < m_mins.y)
		return Vec2(referencePoint.x, m_mins.y);
	else if (referencePoint.x > m_maxs.x)
		return Vec2(m_maxs.x, referencePoint.y);
	else if (referencePoint.y > m_maxs.y)
		return Vec2(referencePoint.x, m_maxs.y);
	else
		return referencePoint;
	
}

Vec2 AABB2::GetPointAtUV(Vec2 const& uv) const
{
	float x = Interpolate(m_mins.x, m_maxs.x, uv.x);
	float y = Interpolate(m_mins.y, m_maxs.y, uv.y);
	return Vec2(x, y);

}

Vec2 AABB2::GetUVForPoint(Vec2 const& point) const
{
	Vec2 uv;
	uv.x = GetFractionWithin(point.x, m_mins.x, m_maxs.x);
	uv.y = GetFractionWithin(point.y, m_mins.y, m_maxs.y);
	return uv;
}
AABB2 AABB2::GetFractionalAABB2(Vec2 const& fractionMins, Vec2 const& fractionMaxs) const
{
	AABB2 returnValue;
	Vec2 dimensions = GetDimensions();
	returnValue.m_mins = m_mins + (dimensions * fractionMins);
	returnValue.m_maxs = m_mins + (dimensions * fractionMaxs);
	return returnValue;
}
void AABB2::Translate(Vec2 const& translation)
{
	m_mins += translation;
	m_maxs += translation;
}

void AABB2::SetCenter(Vec2 const& newCenter)
{
	Vec2 dimensions = GetDimensions();
	m_mins.x = (float)(newCenter.x) - (float)(dimensions.x * 0.5);
	m_mins.y = (float)(newCenter.y) - (float)(dimensions.y * 0.5);
	m_maxs.x = (float)(newCenter.x) + (float)(dimensions.x * 0.5);
	m_maxs.y = (float)(newCenter.y) + (float)(dimensions.y * 0.5);
}

void AABB2::SetDimensions(Vec2 const& newDimensions)
{
	Vec2 center = GetCenter();
	m_mins.x = (float)(center.x) - (float)(newDimensions.x * 0.5);
	m_mins.y = (float)(center.y) - (float)(newDimensions.y * 0.5);
	m_maxs.x = (float)(center.x) + (float)(newDimensions.x * 0.5);
	m_maxs.y = (float)(center.y) + (float)(newDimensions.y * 0.5);
}

void AABB2::StretchToIncludePoint(Vec2 const& point)
{
	if (point.x > m_maxs.x )
	{
		m_maxs.x = point.x;
	}
	if (point.x < m_mins.x)
	{
		m_mins.x = point.x;
	}
	if (point.y > m_maxs.y)
	{
		m_maxs.y = point.y;
	}
	if (point.y < m_mins.y)
	{
		m_mins.y = point.y;
	}
}
void   AABB2::operator+(const AABB2& AABBtoAdd) 
{
	m_mins = AABBtoAdd.m_mins;
	m_maxs = AABBtoAdd.m_maxs;
}
