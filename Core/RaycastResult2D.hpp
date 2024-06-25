#pragma once
#include "Engine/Math/Vec2.hpp"
struct RaycastResult2D
{
public:
	bool didImpact = false;
	Vec2 impactPosition;
	float impactDistance = 0.0f;
	Vec2 impactSurfaceNormal;
	bool startPosInside = false;
};

struct PlaneRaycastResult2D : RaycastResult2D
{
public:
	bool isRayIntersectingFromFrontOfPlane = false;
};
