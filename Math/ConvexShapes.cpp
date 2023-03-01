#include "Engine/Math/ConvexShapes.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vec2.hpp"



void ConvexPoly2D::AddPointAndCleanup(Vec2 point)
{
	m_points.push_back(point);

	//TO DO :: MAKE SURE POINTS INSIDE ARE CLEANED UP
}

int ConvexPoly2D::GetTotalPoints() const
{
	return (int)m_points.size();
}

Vec2 ConvexPoly2D::GetPointAtIndex(int index) const
{
	return m_points[index];
}

void ConvexPoly2D::GetMinsAndMaxs(Vec2& boundingMins, Vec2& boundingMaxs)
{
	for (int i = 0; i < m_points.size() - 1; i++)
	{
		if (m_points[i].x < boundingMins.x)
		{
			boundingMins.x = m_points[i].x;
		}
		if (m_points[i].y < boundingMins.y)
		{
			boundingMins.y = m_points[i].y;
		}
		if (m_points[i].x > boundingMaxs.x)
		{
			boundingMaxs.x = m_points[i].x;
		}
		if (m_points[i].y > boundingMaxs.y)
		{
			boundingMaxs.y = m_points[i].y;
		}
	}
}

bool ConvexPoly2D::CheckIfItOverlapsAABB2(AABB2 bounds)
{
	Vec2 mins = bounds.m_mins;
	Vec2 maxs = bounds.m_maxs;

	for (int i = 0; i < m_points.size() - 1; i++)
	{
		if (m_points[i].x >= mins.x && m_points[i].y >= mins.y && m_points[i].x <= maxs.x
			&& m_points[i].y <= maxs.y)
		{
			return true;
		}
	}
	return false;
}

bool ConvexPoly2D::ConditionToBePartOfBVH(AABB2 bounds)
{
	return false;
}


bool ConvexPoly2D::CheckIfInsideAABB2(AABB2 bounds)
{
	Vec2 mins = bounds.m_mins;
	Vec2 maxs = bounds.m_maxs;

	for (int i = 0; i < m_points.size() - 1; i++)
	{
		if (m_points[i].x < mins.x)
		{
			return false;
		}
		if (m_points[i].y < mins.y)
		{
			return false;
		}
		if (m_points[i].x > maxs.x)
		{
			return false;
		}
		if (m_points[i].y > maxs.y)
		{
			return false;
		}
	}
	return true;
}
ConvexHull2D ConvexPoly2D::ConvertToConvexHull()
{

	ConvexHull2D hull;
	for (int i = 0; i < m_points.size()-1; i++)
	{
		Plane2D plane;

		Vec2 direction = (m_points[i] - m_points[i+1]).GetNormalized();
		plane.m_planeNormal = direction.GetRotated90Degrees();
		plane.m_distance = DotProduct2D(m_points[i], plane.m_planeNormal);
		hull.m_boundingPlanes.push_back(plane);
	}
	return hull;
}

bool ConvexHull2D::IsPointInside(Vec2 const& point)
{
	for (int i = 0; i < m_boundingPlanes.size(); i++)
	{
		float dotValue = DotProduct2D(point , m_boundingPlanes[i].m_planeNormal);
		if (dotValue > m_boundingPlanes[i].m_distance)
		{
			return false;
		}
	}
	return true;
}
