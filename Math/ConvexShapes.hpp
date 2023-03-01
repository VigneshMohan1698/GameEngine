#pragma once
#include "Engine/Core/Vertex_PCU.hpp"
#include <vector>
#include "Engine/Math/AABB2.hpp"

struct Plane2D
{
	Vec2 m_planeNormal;
	float m_distance;
};

struct Plane3D
{
	Vec3 m_planeNormal;
	float m_distance;
};


class ConvexHull2D
{
	public:
	std::vector<Plane2D> m_boundingPlanes;
	bool IsPointInside(Vec2 const& point);
};

class ConvexPoly2D
{
	private:
		std::vector<Vec2> m_points;
	public:
		void AddPointAndCleanup(Vec2 point);
		int GetTotalPoints() const;
		Vec2 GetPointAtIndex(int index) const;
		ConvexHull2D ConvertToConvexHull();
		void GetMinsAndMaxs(Vec2& boundingMins, Vec2& boundingMaxs);
		bool CheckIfInsideAABB2(AABB2 bounds);
		bool CheckIfItOverlapsAABB2(AABB2 bounds);
		bool ConditionToBePartOfBVH(AABB2 bounds);

};
