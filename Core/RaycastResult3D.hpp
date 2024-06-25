



















#pragma once
#include "Engine/Math/Vec3.hpp"

struct RaycastResult3D
{
	Vec3 m_start = Vec3::ZERO;
	Vec3 m_direction = Vec3::ZERO;
	float m_distance = 0.0f;
	bool m_didImpact = false;
	Vec3 m_impactPosition = Vec3::ZERO;
	bool m_hitFloor = false;
	float m_impactFraction = 0.0f;
	float m_impactDistance = 0.0f;
	Vec3 m_impactSurfaceNormal = Vec3::ZERO;
};


struct PlaneRaycastResult3D : RaycastResult3D
{
public:
	bool isRayIntersectingFromFrontOfPlane = false;
};
