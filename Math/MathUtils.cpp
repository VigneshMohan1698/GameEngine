#include "Engine/Math/MathUtils.hpp"
#include "math.h"
//#include "Game/GameCommon.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/FloatRange.hpp"

#pragma once




bool DoDiscsOverlap(const Vec2& centerA, float radiusA, const Vec2& centerB, float radiusB)
{
	if (GetDistance2D(centerA, centerB) < (radiusA + radiusB))
		return true;
	return false;
}
bool DoSpheresOverlap(const Vec3& centerA, float radiusA, const Vec3& centerB, float  radiusB)
{
	if (GetDistance3D(centerA, centerB) - (radiusA + radiusB) < 0)
		return true;
	return false;
}
bool DoSpheresOverlap(const Vec2& centerA, float radiusA, const Vec2& centerB, float  radiusB)
{
	if (GetDistance2D(centerA, centerB) - (radiusA + radiusB) < 0)
		return true;
	return false;
}
void TransformPosition2D(Vec2& posToTransform, float uniformScale, float rotationDegrees, const Vec2& translatePos)
{
	/*Vec3 Normalizedvalue = Normalize(posToTransform);*/
	//Getting x and y for easier calculation
	float x = posToTransform.x;
	float y = posToTransform.y;

	//Changing the scale of single vertex of the Entity
	x *= uniformScale;
	y *= uniformScale;
	//Rotating the position
	Vec2 rotateXY = Vec2(x, y);
	rotateXY.RotateDegrees(rotationDegrees);

	//changing the position single vertex of the Entity
	posToTransform.x = rotateXY.x + translatePos.x;
	posToTransform.y = rotateXY.y + translatePos.y;
}
void TransformPositionXY3D(Vec3& posToTransform, float uniformScale, float rotationDegrees, const Vec2& translatePos)
{

	/*Vec3 Normalizedvalue = Normalize(posToTransform);*/
	//Getting x and y for easier calculation
	float x = posToTransform.x;
	float y = posToTransform.y;

	//Changing the scale of single vertex of the Entity
	x *= uniformScale;
	y *= uniformScale;
	//Rotating the position
	Vec2 rotateXY = Vec2(x, y);
	rotateXY.RotateDegrees(rotationDegrees);

	//changing the position single vertex of the Entity
	posToTransform.x = rotateXY.x + translatePos.x;
	posToTransform.y = rotateXY.y + translatePos.y;
}

Vec2 const GetNearestPointOnDisc2D(Vec2 const& point, Vec2 const& center, float radius)
{
	if (IsPointInsideDisc(center, point, radius))
		return point;

	Vec2 displacement = point - center;


	displacement.ClampLength(radius);
	return center + displacement;
}
Vec2 const GetNearestPointOnInfiniteLine2D(Vec2 const& referencePos, LineSegment2 const& infiniteLine)
{
	Vec2 startVectorToPoint = referencePos - infiniteLine.m_start;
	Vec2 lineSegmentVector = infiniteLine.m_end - infiniteLine.m_start;
	Vec2 projectedVector = GetProjectedOnto2D(startVectorToPoint, lineSegmentVector);

	return infiniteLine.m_start + projectedVector;
}
Vec2 const GetNearestPointOnInfiniteLine2D(Vec2 const& referencePos, Vec2 const& pointOnLine, Vec2 const& anotherPointOnLine)
{
	Vec2 startVectorToPoint = referencePos - pointOnLine;
	Vec2 lineSegmentVector = anotherPointOnLine - pointOnLine;
	Vec2 projectedVector = GetProjectedOnto2D(startVectorToPoint, lineSegmentVector);

	return  pointOnLine + projectedVector;
}
Vec2 const GetNearestPointOnLineSegment2D(Vec2 const& referencePos, LineSegment2 const& lineSegment)
{
	Vec2 PointToStart = referencePos - lineSegment.m_start;
	Vec2 PointToEnd = referencePos - lineSegment.m_end;
	Vec2 Displacement = lineSegment.m_end - lineSegment.m_start;
	if (DotProduct2D(Displacement, PointToEnd) >= 0)
	{
		return lineSegment.m_end;
	}
	else if (DotProduct2D(Displacement, PointToStart) < 0)
	{
		return lineSegment.m_start;
	}

	Vec2 projectedVector = GetProjectedOnto2D(PointToStart, Displacement);

	return lineSegment.m_start + projectedVector;
}
Vec2 const GetNearestPointOnLineSegment2D(Vec2 const& referencePos, Vec2 const& lineSegmentStart, Vec2 const& lineSegmentEnd)
{
	if (DotProduct2D(lineSegmentStart, lineSegmentEnd) >= 0)
	{
		return lineSegmentStart;
	}
	else if (DotProduct2D(lineSegmentStart, lineSegmentEnd) < 0)
	{
		return lineSegmentEnd;
	}

	Vec2 startVectorToPoint = referencePos - lineSegmentStart;
	Vec2 lineSegmentVector = lineSegmentEnd - lineSegmentStart;
	Vec2 projectedVector = GetProjectedOnto2D(startVectorToPoint, lineSegmentVector);

	return  lineSegmentStart + projectedVector;
}
Vec2 const GetNearestPointOnCapsule2D(Vec2 const& referencePos, Capsule2 const& capsule)
{
	if (IsPointInsideCapsule2D(referencePos, capsule))
	{
		return referencePos;
	}

	LineSegment2 capsuleLineSegment = LineSegment2(capsule.m_start, capsule.m_end);
	Vec2 nearestPoint = GetNearestPointOnLineSegment2D(referencePos, capsuleLineSegment);

	Vec2 nearestPointToReference = referencePos - nearestPoint;

	Vec2 pointOnCapsule = nearestPoint + nearestPointToReference.GetNormalized() * capsule.radius;
	return pointOnCapsule;
}
Vec2 const GetNearestPointOnCapsule2D(Vec2 const& referencePos, Vec2 const& boneStart, Vec2 const& boneEnd, float radius)
{
	if (IsPointInsideCapsule2D(referencePos, boneStart, boneEnd, radius))
	{
		return referencePos;
	}

	LineSegment2 capsuleLineSegment = LineSegment2(boneStart, boneEnd);
	Vec2 nearestPoint = GetNearestPointOnLineSegment2D(referencePos, capsuleLineSegment);

	nearestPoint.SetLength(radius);
	return nearestPoint;
}
Vec2 const GetNearestPointOnAABB2D(Vec2 const& referencePos, AABB2 const& box)
{
	return box.GetNearestPoint(referencePos);
}
Vec2 const GetNearestPointOnOBB2D(Vec2 const& referencePos, OBB2 const& orientedBox)
{
	Vec2 displacement = referencePos  - orientedBox.m_center;
	Vec2 m_jbasisNormal = Vec2(-orientedBox.m_iBasisNormal.y, orientedBox.m_iBasisNormal.x);

	float Pi = DotProduct2D(displacement, orientedBox.m_iBasisNormal);
	float Pj = DotProduct2D(displacement, m_jbasisNormal);

	float Ni = Clamp(Pi, -orientedBox.m_halfDimensions.x, orientedBox.m_halfDimensions.x);
	float Nj = Clamp(Pj, -orientedBox.m_halfDimensions.y, orientedBox.m_halfDimensions.y);

	Vec2 N = orientedBox.m_center + (Ni * orientedBox.m_iBasisNormal) + (Nj * m_jbasisNormal);
	return N;
}
Vec3 const GetNearestPointOnAABB3(Vec3 const& referencePoint, AABB3 const& bounds)
{
	Vec3 m_maxs = bounds.m_maxs;
	Vec3 m_mins = bounds.m_mins;
	Vec3 returnValue;
	returnValue = referencePoint;

	if (referencePoint.x >= m_maxs.x) { returnValue.x = m_maxs.x; };
	if (referencePoint.y >= m_maxs.y) { returnValue.y = m_maxs.y; };
	if (referencePoint.z >= m_maxs.z) { returnValue.z = m_maxs.z; };
	if (referencePoint.x <= m_mins.x) { returnValue.x = m_mins.x; };
	if (referencePoint.y <= m_mins.y) { returnValue.y = m_mins.y; };
	if (referencePoint.z <= m_mins.z) { returnValue.z = m_mins.z; };

	return returnValue;
}

Vec3 const GetNearestPointOnSphere3D(Vec3 const& referencePoint, Vec3 const& sphereCenter, float const& sphereRadius)
{
	Vec3 returnValue, displacement;
	displacement = referencePoint - sphereCenter;
	displacement = displacement.GetClamped(sphereRadius);
	returnValue = sphereCenter + displacement;
	return returnValue;
}
Vec3 const GetNearestPointOnZCylinder(Vec3 const& referencePoint, Vec2 const& cylinderXY, float const& cylinderRadius, FloatRange const& zRange)
{
	Vec3 returnValue;
	Vec3 cylinderPos = Vec3(cylinderXY.x, cylinderXY.y, zRange.m_min);
	Vec2 displacement2D = Vec2(referencePoint) - cylinderXY;

	displacement2D.ClampLength(cylinderRadius);
	returnValue = Vec3(displacement2D);
	returnValue += cylinderPos;
	returnValue.z = Clamp(referencePoint.z, zRange.m_min, zRange.m_max);
	
	return returnValue;
}

RaycastResult2D RaycastVsDisc2D(Vec2 startingPoint, Vec2 endingPoint, Vec2 discCenter, float DiscRadius, float maxDistance, float stepsToTake)
{
	UNUSED((void)maxDistance);
	RaycastResult2D returnValue;

	Vec2 directionVector = (endingPoint - startingPoint).GetNormalized();
	Vec2 curentPoint = startingPoint;
	while ((curentPoint - endingPoint).GetLength() >= 0.2f)
	{
		curentPoint += directionVector * stepsToTake;
		if (IsPointInsideDisc(discCenter, curentPoint, DiscRadius))
		{
			returnValue.didImpact = true;
			returnValue.impactPosition = curentPoint;
			returnValue.impactDistance = (curentPoint - startingPoint).GetLength();
			Vec2 impactVector = (curentPoint - startingPoint).GetReflected(curentPoint - discCenter);
			returnValue.impactSurfaceNormal = impactVector;
			return returnValue;
		}
	}
	return returnValue;
}
RaycastResult2D RaycastVsDisc2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, Vec2 discCenter, float discRadius)
{
	RaycastResult2D returnValue;
	Vec2 leftNormal = Vec2(-fwdNormal.y, fwdNormal.x);
	Vec2 startToCenter = discCenter - startPos;

	float startToCenterJ = DotProduct2D(startToCenter, leftNormal);

	if (startToCenterJ > discRadius || startToCenterJ < -discRadius)
	{
		returnValue.didImpact = false;
		return returnValue;
	}
	float startToCenterI = DotProduct2D(startToCenter, fwdNormal);
	if (startToCenterI < -discRadius || startToCenterI > maxDist + discRadius)
	{
		returnValue.didImpact = false;
		return returnValue;
	}
	if (IsPointInsideDisc(discCenter, startPos, discRadius))
	{
		returnValue.didImpact = true;
		returnValue.impactDistance = 0.0f;
		returnValue.impactSurfaceNormal = -fwdNormal;
		returnValue.impactPosition = startPos;
		return returnValue;
	}
	float distanceToIntersectPoint = sqrtf((discRadius * discRadius) - (startToCenterJ * startToCenterJ));
	float ImpactDistance = startToCenterI - distanceToIntersectPoint;
	if (ImpactDistance < 0 || ImpactDistance > maxDist)
	{
		returnValue.didImpact = false;
		return returnValue;
	}
	returnValue.impactPosition = startPos + (fwdNormal * ImpactDistance);
	returnValue.didImpact = true;
	returnValue.impactDistance = ImpactDistance;
	returnValue.impactSurfaceNormal = (returnValue.impactPosition - discCenter).GetNormalized();
	return returnValue;
}
RaycastResult2D RaycasVsConvexHull2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, ConvexHull2D hull)
{
	PlaneRaycastResult2D planesRaycast;
	RaycastResult2D convexShapeRaycast;
	float maximumEntryPointToShape = maxDist;
	if (hull.IsPointInside(startPos))
	{
		convexShapeRaycast.didImpact = true;
		convexShapeRaycast.impactPosition = startPos;
		convexShapeRaycast.impactDistance = 0.0f;
		convexShapeRaycast.impactSurfaceNormal = -fwdNormal;
		return convexShapeRaycast;
	}
	for (int i = 0; i < hull.m_boundingPlanes.size(); i++)
	{
		planesRaycast = RaycasVsPlane2D(startPos , fwdNormal, maxDist, hull.m_boundingPlanes[i]);

		//------Check if Ray intersects each plane and it is the maximum entry point---------
		if (planesRaycast.didImpact && planesRaycast.isRayIntersectingFromFrontOfPlane && planesRaycast.impactDistance < maximumEntryPointToShape)
		{
			bool pointIsOnConvexShape = true;
			//-------Check if the impacted plane point is behind all the other planes in the hull-------------
			//------i.e., Impact Position dot N < plane Distance
			//------So set it true and change to false if  Impact Position dot N > plane Distance
			for (int j = 0; j < hull.m_boundingPlanes.size(); j++)
			{
				Plane2D& hullPlane = hull.m_boundingPlanes[j];
				if (i == j)
				{
					continue;
				}
				float dotWithOtherPlane = DotProduct2D(planesRaycast.impactPosition, hullPlane.m_planeNormal);
				if (dotWithOtherPlane >  hullPlane.m_distance)
				{
					pointIsOnConvexShape = false;
				}
			}
			if (pointIsOnConvexShape)
			{
				convexShapeRaycast.didImpact = true;
				convexShapeRaycast.impactDistance = planesRaycast.impactDistance;
				convexShapeRaycast.impactPosition = planesRaycast.impactPosition;
				convexShapeRaycast.impactSurfaceNormal = planesRaycast.impactSurfaceNormal;
			}
		}
	}
	return convexShapeRaycast;
}
PlaneRaycastResult2D RaycasVsPlane2D(const Vec2& startPos, const Vec2& fwdNormal, const float& maxDist, Plane2D plane)
{
	PlaneRaycastResult2D planeRaycast;
	Vec2 endPos = startPos + (fwdNormal * maxDist);
	float planeDistance = plane.m_distance;
	float startPointDot = DotProduct2D(plane.m_planeNormal, startPos);
	float endPointDot = DotProduct2D(plane.m_planeNormal, endPos);

	//-------------EARLY OUTS FOR IF RAY START AND END IS ON SAME SIDE-------------
	if (startPointDot > planeDistance && endPointDot > planeDistance)
	{
		return planeRaycast;
	}
	if (startPointDot < planeDistance && endPointDot < planeDistance)
	{
		return planeRaycast;
	}

	//---------SIMILAR TRIANGLES IN PLANE SPACE-------------
	float rayProject = DotProduct2D(fwdNormal, plane.m_planeNormal);
	float startPointAltitude = startPointDot - plane.m_distance;
	float impactDistance = fabsf(startPointAltitude / rayProject);
	if (impactDistance > maxDist)
	{
		return planeRaycast;
	}
	planeRaycast.didImpact = true;
	planeRaycast.impactDistance = impactDistance;
	planeRaycast.impactPosition = startPos + (fwdNormal * impactDistance);
	planeRaycast.impactSurfaceNormal = fwdNormal.GetReflected(plane.m_planeNormal);

	if (startPointDot > planeDistance && endPointDot < planeDistance)
	{
		// If Ray goes from front of the plane to the back
		planeRaycast.isRayIntersectingFromFrontOfPlane = true; 
	}
	else 
	{
		// If Ray goes from back of the plane to the front
		planeRaycast.isRayIntersectingFromFrontOfPlane = false; 
	}

	return planeRaycast;
}

PlaneRaycastResult3D RaycastVsPlane3D(const Vec3& startPos, const Vec3& fwdNormal, const float& maxDist, Plane3D plane)
{
	PlaneRaycastResult3D result;

	float planeDistance = plane.m_distance;
	Vec3 endPost = startPos  + fwdNormal * maxDist;
	float startDot = DotProduct3D(startPos, plane.m_planeNormal);
	float endDot = DotProduct3D(endPost, plane.m_planeNormal);

	//-------------EARLY OUTS FOR IF RAY START AND END IS ON SAME SIDE-------------
	if (startDot > planeDistance && endDot > planeDistance)
	{
		return result;
	}
	if (startDot < planeDistance && endDot < planeDistance)
	{
		return result;
	}

	float rayProject = DotProduct3D(fwdNormal, plane.m_planeNormal.GetNormalized());
	float startPointAltitude = startDot - plane.m_distance;
	float impactDistance = fabsf(startPointAltitude / rayProject);
	if (impactDistance > maxDist)
	{
		return result;
	}
	result.m_didImpact = true;
	result.m_impactPosition = startPos + fwdNormal * impactDistance;
	Vec3 planeNormalInverse = -plane.m_planeNormal;
	result.m_impactSurfaceNormal = fwdNormal.GetReflected(planeNormalInverse).GetNormalized();
	return result;
}

RaycastResult2D RaycastVsAABB2(Vec2 startPos, Vec2 fwdNormal, float maxDist, AABB2 aabbBounds)
{
	RaycastResult2D returnValue;
	Vec2 endPos = startPos + (fwdNormal * maxDist);
	if (IsPointInsideAABB2D((startPos), aabbBounds))
	{
		returnValue.didImpact = true;
		returnValue.impactPosition = startPos;
		Vec2 dir = (endPos - startPos).GetNormalized();
		returnValue.impactSurfaceNormal = returnValue.impactPosition - (dir*5.0f);
		return returnValue;
	}
	if (startPos.x < aabbBounds.m_mins.x && endPos.x < aabbBounds.m_mins.x)
	{
		return returnValue;
	}
	if (startPos.y < aabbBounds.m_mins.y && endPos.y < aabbBounds.m_mins.y)
	{
		return returnValue;
	}
	if (startPos.x > aabbBounds.m_maxs.x && endPos.x > aabbBounds.m_maxs.x)
	{
		return returnValue;
	}
	if (startPos.y > aabbBounds.m_maxs.y && endPos.y > aabbBounds.m_maxs.y)
	{
		return returnValue;
	}
	UNUSED((void)maxDist);
	Vec2 I;
	float Ix, Iy,t;
	float tleft, tright, ttop, tbottom;
	float left = aabbBounds.m_mins.x;
	float bottom = aabbBounds.m_mins.y;
	float right = aabbBounds.m_maxs.x;
	float top = aabbBounds.m_maxs.y;


	tleft = (left - startPos.x) / fwdNormal.x;
	tbottom = (bottom - startPos.y) / fwdNormal.y;
	tright = (right - startPos.x) / fwdNormal.x;
	ttop = (top - startPos.y) / fwdNormal.y;

	Ix = GetSmallest(tleft, tright);
	Iy = GetSmallest(ttop, tbottom);
	t = GetLargest(Ix, Iy);
	if (Ix > maxDist && Iy < maxDist) { t = Iy; };
	if (Iy > maxDist && Ix < maxDist) { t = Ix; };
	if (fabsf(t) < maxDist)
	{
		I = startPos + t * fwdNormal;
		Ix = I.x;
		Iy = I.y;
	}
	if (GetDistance2D(I,GetNearestPointOnAABB2D(I, aabbBounds)) < 0.0001f)
	{
		returnValue.didImpact = true;
		returnValue.impactPosition = Vec2(Ix, Iy);
		returnValue.impactDistance = GetDistance2D(returnValue.impactPosition,startPos);
		returnValue.impactSurfaceNormal = returnValue.impactPosition;
		if (returnValue.impactPosition.x == left)
		{
			returnValue.impactSurfaceNormal.x = returnValue.impactPosition.x - 5.0f;
		}
		else if(returnValue.impactPosition.x == right)
		{
			returnValue.impactSurfaceNormal.x = returnValue.impactPosition.x + 5.0f;
		}
		else if (returnValue.impactPosition.y == top)
		{
			returnValue.impactSurfaceNormal.y = returnValue.impactPosition.y + 5.0f;
		}
		else if (returnValue.impactPosition.y == bottom)
		{
			returnValue.impactSurfaceNormal.y = returnValue.impactPosition.y - 5.0f;
		}
	}
	return returnValue;
}
RaycastResult2D RaycasVsOBB2(Vec2 startPos, Vec2 fwdNormal, float maxDist, OBB2 obbBounds) 
{
	RaycastResult2D returnValue;
	AABB2 local;
	local.m_mins.x = -obbBounds.m_halfDimensions.x;
	local.m_mins.y =  -obbBounds.m_halfDimensions.y;
	local.m_maxs.x = obbBounds.m_halfDimensions.x;
	local.m_maxs.y = obbBounds.m_halfDimensions.y;

	Vec2 displacement = startPos - obbBounds.m_center;
	Vec2 m_jbasisNormal = Vec2(-obbBounds.m_iBasisNormal.y, obbBounds.m_iBasisNormal.x);

	float Pi = DotProduct2D(displacement, obbBounds.m_iBasisNormal);
	float Pj = DotProduct2D(displacement, m_jbasisNormal);

	Vec2 localStart = Vec2(Pi, Pj);
	Vec2 localFwdNormal;
	Vec2 obbJbasis = Vec2(-obbBounds.m_iBasisNormal.y, obbBounds.m_iBasisNormal.x);
	localFwdNormal.x = DotProduct2D(obbBounds.m_iBasisNormal, fwdNormal);
	localFwdNormal.y = DotProduct2D(obbJbasis, fwdNormal);

	returnValue = RaycastVsAABB2(localStart, localFwdNormal, maxDist, local);
	if (returnValue.didImpact)
	{
		returnValue.impactPosition = obbBounds.m_center + (returnValue.impactPosition.x * obbBounds.m_iBasisNormal) + (returnValue.impactPosition.y* m_jbasisNormal);
		Vec2 dir = (obbBounds.m_center - returnValue.impactPosition).GetNormalized();
		returnValue.impactSurfaceNormal = returnValue.impactPosition - (dir * 5.0f);
	}
	return returnValue;
}
RaycastResult2D RaycastVsLineSegment2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, LineSegment2 line)
{
	RaycastResult2D result;
	Vec2 j = Vec2(-fwdNormal.y, fwdNormal.x);
	// R -> Ray start M-> Ray end S-> line start E->line end
	Vec2 S = line.m_start;
	Vec2 E = line.m_end;
	Vec2 R = startPos;
	Vec2 M = startPos + fwdNormal * maxDist;

	Vec2 RS = S - R;
	Vec2 RE = E - R;
	Vec2 SE = E - S;
	float RSj = DotProduct2D(RS, j);
	float REj = DotProduct2D(RE, j);
	if (RSj * REj >= 0)
	{
		return result;
	}
	float RSi = DotProduct2D(RS, fwdNormal);
	float REi = DotProduct2D(RE, fwdNormal);
	if (RSi < 0 && REi < 0) { return result; };
	if (RSi > maxDist && REi > maxDist) { return result; };
	float Ifraction = RSj / (RSj - REj);
	Vec2 I = S + Ifraction * SE;
	float impactDistance = GetDistance2D(I, R);
	if (impactDistance > 0.0f && impactDistance < maxDist)
	{
		result.didImpact = true;
		result.impactDistance = impactDistance;
		result.impactPosition = R + impactDistance * fwdNormal;
		Vec2 impactNormal = SE.GetRotated90Degrees().GetNormalized();
		if ((RSi > 0.0f || RSj > 0.0f) && RSj >0.0f)
		{
			impactNormal = -impactNormal;
		}
		result.impactSurfaceNormal = result.impactPosition + 5.0f * impactNormal;
	}
	return result;
}
RaycastResult3D RaycastVsAABB3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, AABB3 aabbBounds)
{
	RaycastResult2D aabb2raycast;
	RaycastResult3D returnValue;
	AABB2 aab2;
	Vec3 end = startPos + (fwdNormal * maxDist);

	aab2.m_mins = Vec2(aabbBounds.m_mins);
	aab2.m_maxs = Vec2(aabbBounds.m_maxs);
	aabb2raycast = RaycastVsAABB2(Vec2(startPos), Vec2(fwdNormal).GetNormalized(), maxDist, aab2);

	//--------------CHECK IF IT IMPACTS IN 2D IF NOT RETURN -------------------------
	if (!aabb2raycast.didImpact)
	{
		return returnValue;
	}

	//--------------- CHECK Z IMPACT -----------------------
	float length2, length, t = 0.0f;
	length = GetDistance2D(Vec2(startPos), Vec2(end)); 
	if (startPos.z - end.z != 0.0f)
	{
		length2 = aabb2raycast.impactDistance;
		t = (length2 * maxDist) / length;
	}
	returnValue.m_impactPosition = startPos + (fwdNormal * t);
	returnValue.m_impactDistance = GetDistance3D(returnValue.m_impactPosition, startPos);

	float deltaZ, directionZ;
	if (returnValue.m_impactDistance >= maxDist)
	{
		return returnValue;
	}
	if (returnValue.m_impactPosition.z < aabbBounds.m_maxs.z && returnValue.m_impactPosition.z > aabbBounds.m_mins.z)
	{
		returnValue.m_didImpact = true;
		returnValue.m_impactSurfaceNormal = returnValue.m_impactPosition;
		if (returnValue.m_impactPosition.x <= aabbBounds.m_mins.x)
		{
			returnValue.m_impactSurfaceNormal.x = returnValue.m_impactPosition.x - 0.3f;
		}
		else if(returnValue.m_impactPosition.x >= aabbBounds.m_maxs.x)
		{
			returnValue.m_impactSurfaceNormal.x = returnValue.m_impactPosition.x + 0.3f;
		}		
		else if (returnValue.m_impactPosition.y <= aabbBounds.m_mins.y)
		{
			returnValue.m_impactSurfaceNormal.y = returnValue.m_impactPosition.y - 0.3f;
		}
		else if(returnValue.m_impactPosition.y >= aabbBounds.m_maxs.y)
		{
			returnValue.m_impactSurfaceNormal.y = returnValue.m_impactPosition.y + 0.3f;
		}

	}
	//---------------------CHECKING AABB TOP AND BOTTOM -----------------------------
	else if(startPos.z > aabbBounds.m_maxs.z)
	{
		directionZ = fabsf(fwdNormal.z);
		deltaZ = fabsf(startPos.z - aabbBounds.m_maxs.z);
		t = (deltaZ / directionZ);
		if (t <= maxDist)
		{
			returnValue.m_impactPosition = startPos + (fwdNormal * t);
			if (returnValue.m_impactPosition.z <= aabbBounds.m_maxs.z && returnValue.m_impactPosition.z >= aabbBounds.m_mins.z && 
				returnValue.m_impactPosition.x <= aabbBounds.m_maxs.x && returnValue.m_impactPosition.x >= aabbBounds.m_mins.x && 
				returnValue.m_impactPosition.y <= aabbBounds.m_maxs.y && returnValue.m_impactPosition.y >= aabbBounds.m_mins.y)
			{
				returnValue.m_didImpact = true;
				returnValue.m_impactDistance = GetDistance3D(returnValue.m_impactPosition, startPos);
				returnValue.m_impactSurfaceNormal = returnValue.m_impactPosition;
				returnValue.m_impactSurfaceNormal.z = returnValue.m_impactPosition.z + 0.3f;
			}
			
		}
	}
	else if (startPos.z < aabbBounds.m_mins.z)
	{
		directionZ = fabsf(fwdNormal.z);
		deltaZ = fabsf(startPos.z - aabbBounds.m_mins.z);
		t = (deltaZ / directionZ);
		if (t <= maxDist)
		{
			returnValue.m_impactPosition = startPos + (fwdNormal * t);
			if (returnValue.m_impactPosition.z <= aabbBounds.m_maxs.z && returnValue.m_impactPosition.z >= aabbBounds.m_mins.z &&
				returnValue.m_impactPosition.x <= aabbBounds.m_maxs.x && returnValue.m_impactPosition.x >= aabbBounds.m_mins.x &&
				returnValue.m_impactPosition.y <= aabbBounds.m_maxs.y && returnValue.m_impactPosition.y >= aabbBounds.m_mins.y)
			{
				returnValue.m_didImpact = true;
				returnValue.m_impactSurfaceNormal = returnValue.m_impactPosition;
				returnValue.m_impactSurfaceNormal.z = returnValue.m_impactPosition.z - 0.3f;
			}
		}
	}

	return returnValue;
}
RaycastResult3D RaycastVsZCylinder(Vec3 startPos, Vec3 fwdNormal, float maxDist, Vec3 cylinderPosition, FloatRange zRange, float cylinderRadius)
{

	RaycastResult3D returnValue;
	Vec3 end = startPos + (fwdNormal * maxDist);
	float height, radius;
	Vec3 actorPosition = cylinderPosition;
	height = zRange.m_max - zRange.m_min;
	radius = cylinderRadius;
	Vec3 cylinderTop = Vec3(cylinderPosition.x, cylinderPosition.y, cylinderPosition.z + height);
	Vec3 cylinderBottom = cylinderPosition;
	float cylinderTopZ = cylinderTop.z;
	float cylinderBottomZ = cylinderBottom.z;

	float t = 0.0f, length2, length;
	length = GetDistance2D(Vec2(startPos), Vec2(end));
	RaycastResult2D discImpact = RaycastVsDisc2D(Vec2(startPos.x, startPos.y), Vec2(fwdNormal.x, fwdNormal.y).GetNormalized(), maxDist, Vec2(actorPosition.x, actorPosition.y), radius);
	Vec3 impactPoint;
	if (discImpact.didImpact)
	{
		//-------------------------------------CHECKING CYLINDER SIDE COLLISIONS----------------------------------
		if (startPos.z - end.z != 0.0f)
		{
			length2 = discImpact.impactDistance;
			t = (length2 * maxDist) / length;
		}
		returnValue.m_impactPosition = startPos + (fwdNormal * t);

		float deltaZ, directionZ;
		directionZ = fabsf(fwdNormal.z);
		//------------------------------------CHECKING IF RAY START IS INSIDE CYLINDER----------------------------------
		if (returnValue.m_impactPosition == startPos)
		{
			returnValue.m_didImpact = true;
			returnValue.m_impactDistance = 0.0f;
			returnValue.m_impactSurfaceNormal = returnValue.m_impactPosition + (fwdNormal * -0.3f);
			return returnValue;
		}

		//------------------------------------CHECKING TOP CIRCLE COLLISIONS----------------------------------
		else if (returnValue.m_impactPosition.z >= cylinderTopZ)
		{

			deltaZ = startPos.z - cylinderTopZ;
			t = (deltaZ / directionZ);
			returnValue.m_impactPosition = startPos + (fwdNormal * t);
			if (IsPointInsideDisc(Vec2(cylinderTop), Vec2(returnValue.m_impactPosition), radius))
			{
				returnValue.m_didImpact = true;
				returnValue.m_impactSurfaceNormal = returnValue.m_impactPosition;
				returnValue.m_impactSurfaceNormal.z += 0.3f;
				returnValue.m_impactDistance = GetDistance3D(startPos, returnValue.m_impactPosition);
			}

		}
		//------------------------------------CHECKING BOTTOM CIRCLE COLLISIONS----------------------------------
		else if (returnValue.m_impactPosition.z <= cylinderBottomZ)
		{
			deltaZ = cylinderBottomZ - startPos.z;
			t = (deltaZ / directionZ);
			returnValue.m_impactPosition = startPos + (fwdNormal * t);
			if (IsPointInsideDisc(Vec2(cylinderBottom), Vec2(returnValue.m_impactPosition), radius))
			{
				returnValue.m_didImpact = true;
				returnValue.m_impactSurfaceNormal = returnValue.m_impactPosition;
				returnValue.m_impactSurfaceNormal.z -= 0.3f;
				returnValue.m_impactDistance = GetDistance3D(startPos, returnValue.m_impactPosition);
			}
		}
		//--------------------------------------SIDE COLLISION FINAL ----------------------------
		else
		{
			returnValue.m_didImpact = true;
			Vec3 normalCenter = actorPosition;
			normalCenter.z = returnValue.m_impactPosition.z;
			Vec3 normal;
			normal = (normalCenter - returnValue.m_impactPosition).GetNormalized();
			returnValue.m_impactSurfaceNormal = returnValue.m_impactPosition - (0.3f * normal);
			returnValue.m_impactDistance = GetDistance3D(startPos, returnValue.m_impactPosition);
		}
	}
	return returnValue;
}
RaycastResult3D RaycastVsSphere3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, Vec3 spherePosition, float const& sphereRadius)
{
	RaycastResult3D returnValue;
	Vec3 endPos = startPos + (maxDist * fwdNormal);

	Vec3 startToCenter = spherePosition - startPos;
	Vec3 endToStart = endPos - startPos;
	float dot = DotProduct3D(startToCenter, fwdNormal);
	
	Vec3 pointOnStartToEnd = startPos + fwdNormal * dot;
	Vec3 leftNormal = (spherePosition - pointOnStartToEnd);
	float h = leftNormal.GetLength();
	if (h > sphereRadius || h < -sphereRadius)
	{
		returnValue.m_didImpact = false;
		return returnValue;
	}
	float a = sqrtf((sphereRadius * sphereRadius) - (h * h));
	Vec3 pointOfContact = startPos + ((dot-a)*fwdNormal);

	if (GetDistance3D(pointOfContact, startPos) <= maxDist)
	{
		returnValue.m_didImpact = true;
		returnValue.m_impactPosition = pointOfContact;
		returnValue.m_impactDistance = GetDistance3D(pointOfContact, startPos);
		Vec3 normal;
		normal = (spherePosition - returnValue.m_impactPosition).GetNormalized();
		returnValue.m_impactSurfaceNormal = returnValue.m_impactPosition - (0.3f * normal);
	}
	return returnValue;
}

bool OverlapAABB2DVsAABB2D(const AABB3& a, const AABB3& b)
{
	Vec3 bmins = b.m_mins;
	Vec3 amins = a.m_mins;
	Vec3 amaxs = a.m_maxs;
	Vec3 bmaxs = b.m_maxs;
	if (amins.y > bmaxs.y)
	{
		return false;
	}
	if (bmins.y > amaxs.y)
	{
		return false;
	}
	if (amins.z > bmaxs.z)
	{
		return false;
	}
	if (bmins.z > amaxs.z)
	{
		return false;
	}

	return true;
}


bool OverlapAABB3DVsAABB3D(const AABB3& a, const AABB3& b)
{
	Vec3 bmins = b.m_mins;
	Vec3 amins = a.m_mins;
	Vec3 amaxs = a.m_maxs;
	Vec3 bmaxs = b.m_maxs;
	if (amins.x > bmaxs.x )
	{
		return false;
	}
	if (bmins.x > amaxs.x)
	{
		return false;
	}
	if (amins.y > bmaxs.y)
	{
		return false;
	}
	if (bmins.y > amaxs.y)
	{
		return false;
	}	
	if (amins.z > bmaxs.z)
	{
		return false;
	}
	if (bmins.z > amaxs.z)
	{
		return false;
	}

	return true;
}


bool OverlapAABB3DVsSphere(const AABB3& a, const Vec3& sphereCenter, const float& sphereRadius)
{
	Vec3 nearestPoint = GetNearestPointOnAABB3(sphereCenter, a);
	float distance = GetDistance3D(nearestPoint, sphereCenter);
	if (distance * distance < sphereRadius * sphereRadius)
	{
		return true;
	}
	return false;
}
bool OverlapZClyinderVsSphere(Vec2 const& cylinderXY, float const& cylinderRadius, FloatRange const& zRange, const Vec3& sphereCenter, const float& sphereRadius)
{
	Vec3 nearestPoint = GetNearestPointOnZCylinder(sphereCenter, cylinderXY, cylinderRadius, zRange);
	float distance = GetDistance3D(nearestPoint, sphereCenter);
	if (distance * distance < sphereRadius * sphereRadius)
	{
		return true;
	}
	return false;
}
bool OverlapZCylinderVsZCylinder(Vec2 const& cylinderXYA, float const& cylinderRadiusA, FloatRange const& zRangeA, Vec2 const& cylinderXYB, float const& cylinderRadiusB, FloatRange const& zRangeB)
{
	if (zRangeA.m_min > zRangeB.m_max)
	{
		return false;
	}
	if (zRangeB.m_min > zRangeA.m_max)
	{
		return false;
	}
	if (!DoDiscsOverlap(cylinderXYA, cylinderRadiusA, cylinderXYB, cylinderRadiusB))
	{
		return false;
	}
	return true;
}
bool OverlapZCylinderVsAABB3D( Vec2 const& cylinderXY, float const& cylinderRadius, FloatRange const& zRange, const AABB3& bounds)
{
	AABB2 bounds2D;
	bounds2D.m_maxs = Vec2(bounds.m_maxs);
	bounds2D.m_mins = Vec2(bounds.m_mins);
	Vec2 nearestPoint = GetNearestPointOnAABB2D(cylinderXY, bounds2D);

	if (!IsPointInsideDisc(cylinderXY, nearestPoint, cylinderRadius))
	{
		return false;
	}
	if (bounds.m_mins.z > zRange.m_max)
	{
		return false;
	}
	if (bounds.m_maxs.z < zRange.m_min)
	{
		return false;
	}
	return true;
}

bool IsPointInsideDisc(const Vec2& center, const Vec2& point, float radius)
{
	float displacementSquared = GetDistanceSquared2D(center, point);
	return displacementSquared < (radius * radius);
}

bool IsPointInsideOrientedSector2D(const Vec2& point, const Vec2& center, float orientation, float aperture, float radius)
{
	if (!IsPointInsideDisc(point, center, radius))
		return false;

	float orientationFromPoint = (point - center).GetOrientationDegrees();
	float shortestAngle = GetShortestAngularDispDegrees(orientation, orientationFromPoint);
	return shortestAngle > -aperture / 2 && shortestAngle < aperture / 2;
}
bool IsPointInsideDirectedSector2D(const Vec2& point, const Vec2& center, const Vec2& i, float aperture, float radius)
{
	if (!IsPointInsideDisc(point, center, radius))
		return false;

	float shortestAngle = GetAngleDegreesBetweenVectors2D(i, (point - center));
	return shortestAngle > -aperture / 2 && shortestAngle < aperture / 2;
}
bool IsPointInsideAABB2D(Vec2 const& point, AABB2 const& box)
{
	if (point.x >= box.m_mins.x && point.y >= box.m_mins.y &&
		point.x <= box.m_maxs.x && point.y <= box.m_maxs.y)
	{
		return true;
	}
	else return false;
}
bool IsPointInsideCapsule2D(Vec2 const& point, Capsule2 const& capsule)
{
	LineSegment2 capsuleLineSegment = LineSegment2(capsule.m_start, capsule.m_end);
	Vec2 nearestPoint = GetNearestPointOnLineSegment2D(point, capsuleLineSegment);
	Vec2 nearesPointToReferencePoint = point - nearestPoint;
	if (nearesPointToReferencePoint.GetLength() < capsule.radius)
		return true;
	else
		return false;
}
bool IsPointInsideCapsule2D(Vec2 const& point, Vec2 const& boneStart, Vec2 const& boneEnd, float radius)
{
	LineSegment2 capsuleLineSegment = LineSegment2(boneStart, boneEnd);
	Vec2 nearestPoint = GetNearestPointOnLineSegment2D(point, capsuleLineSegment);

	if (nearestPoint.GetLength() < radius)
		return true;
	else
		return false;
}
bool IsPointInsideOBB2D(Vec2 const& point, OBB2 const& orientedBox)
{
	Vec2 rightDisplacement = orientedBox.m_iBasisNormal * orientedBox.m_halfDimensions.x;
	Vec2 m_jbasisNormal = Vec2(-orientedBox.m_iBasisNormal.y, orientedBox.m_iBasisNormal.x);
	Vec2 upDisplacement = m_jbasisNormal * orientedBox.m_halfDimensions.y;

	Vec2 A = orientedBox.m_center + rightDisplacement + upDisplacement;
	Vec2 B = orientedBox.m_center - rightDisplacement + upDisplacement;
	Vec2 C = orientedBox.m_center - rightDisplacement - upDisplacement;
	Vec2 D = orientedBox.m_center + rightDisplacement - upDisplacement;

	Vec2 displacement = point - orientedBox.m_center;
	float Pi = DotProduct2D(displacement, orientedBox.m_iBasisNormal);
	float Pj = DotProduct2D(displacement, m_jbasisNormal);

	if (-orientedBox.m_halfDimensions.x < Pi && Pi < orientedBox.m_halfDimensions.x && -orientedBox.m_halfDimensions.y < Pj && Pi < orientedBox.m_halfDimensions.y)
	{
		return true;
	}
	else return false;

	/*Vec2 m_mins = Vec2(orientedBox.m_center.x - (orientedBox.m_iBasisNormal.x * orientedBox.m_halfDimensions.x), orientedBox.m_center.y - (orientedBox.m_iBasisNormal.y * orientedBox.m_halfDimensions.y));
	Vec2 m_maxs = Vec2(orientedBox.m_center.x + (orientedBox.m_iBasisNormal.x * orientedBox.m_halfDimensions.x), orientedBox.m_center.y + (orientedBox.m_iBasisNormal.y * orientedBox.m_halfDimensions.y));
	
	if (point.x >= m_mins.x && point.y >= m_mins.y &&
		point.x <= m_maxs.x && point.y <= m_maxs.y)
	{
		return true;
	}
	else return false;*/
}

bool PushDiscOutOfPoint2D(Vec2& MobileDiscCenter, float radius, const Vec2& fixedPoint)
{
	if (!IsPointInsideDisc(MobileDiscCenter, fixedPoint, radius))
	{
		return false;
	}
	float overlapDepth = 0.0f;
	Vec2 displacement = fixedPoint - MobileDiscCenter;
	overlapDepth = radius - displacement.GetLength();

	Vec2 pushDirection = MobileDiscCenter - fixedPoint;
	pushDirection.SetLength(overlapDepth);
	MobileDiscCenter += pushDirection;
	return true;
}
bool PushDiscOutOfDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2 const& fixedDiscCenter, float fixedDiscRadius)
{
	if (!DoDiscsOverlap(mobileDiscCenter, mobileDiscRadius, fixedDiscCenter, fixedDiscRadius))
		return false;

	float distBetweenCenters = GetDistance2D(fixedDiscCenter, mobileDiscCenter);
	float distToMoveMobileDisc = fixedDiscRadius + mobileDiscRadius - distBetweenCenters;
	Vec2 dispToMove = (mobileDiscCenter - fixedDiscCenter).GetNormalized() * distToMoveMobileDisc;
	mobileDiscCenter += dispToMove;
	return true;

}
bool PushDiscsOutOfEachOther2D(Vec2& discA, float radiusA,Vec2& discB, float radiusB)
{
	if (!DoDiscsOverlap(discA, radiusA, discB, radiusB))
		return false;

	Vec2 displacement = discB - discA;

	float totalRadius = radiusA + radiusB;
	float overlapDepth = totalRadius - displacement.GetLength();

	Vec2 pushDirection = discA - discB;
	pushDirection.SetLength(overlapDepth);
	discA += pushDirection/2;
	discB -= pushDirection/2;
	return true;
}

Vec3 PushAABB3DoutOfAABB3D(AABB3& movingBox, AABB3 fixedBox, float tolerance)
{
	Vec3 bmins = fixedBox.m_mins;
	Vec3 amins = movingBox.m_mins;
	Vec3 amaxs = movingBox.m_maxs;
	Vec3 bmaxs = fixedBox.m_maxs;

	Vec3 pushValue;
	if (amins.x - bmaxs.x > tolerance)
	{
		pushValue.x += amins.x - bmaxs.x;
	}
	if (bmins.x - amaxs.x > tolerance)
	{
		pushValue.x += bmins.x - amaxs.x;
	}
	if (amins.y - bmaxs.y > tolerance)
	{
		pushValue.y += amins.y - bmaxs.y;
	}
	if (bmins.y - amaxs.y > tolerance)
	{
		pushValue.y += bmins.y - amaxs.y;
	}
	if (amins.z - bmaxs.z > tolerance)
	{
		pushValue.z += amins.z - bmaxs.z;
	}
	if (bmins.z - amaxs.z > tolerance)
	{
		pushValue.z += bmins.z - amaxs.z;
	}

	return pushValue;
}
bool BounceDiscOffEachOther(Vec2& discA, const float& radiusA, Vec2& velocityA ,Vec2& discB, const float& radiusB, Vec2& velocityB, const float& elasticity)
{
	if (!DoDiscsOverlap(discA, radiusA, discB, radiusB))
		return false;

	PushDiscsOutOfEachOther2D(discA, radiusA, discB, radiusB);
	Vec2 normalA = (discB - discA).GetNormalized();

	if (DotProduct2D(velocityB, normalA) - DotProduct2D(velocityA, normalA) > 0.f)
		return false;

	Vec2 normalAVelocity = DotProduct2D(velocityA, normalA) * normalA;
	Vec2 tangentA = velocityA - normalAVelocity;
	Vec2 normalB = -normalA;

	Vec2 bDiscNormalVelocity = DotProduct2D(velocityB, normalB) * normalB;
	Vec2 tangentB = velocityB - bDiscNormalVelocity;

	velocityA = (bDiscNormalVelocity * elasticity) + tangentA;
	velocityB = (normalAVelocity * elasticity) + tangentB;

	return true;
}
bool BounceDiscOffDisc(Vec2& mobileDisc, const float& mobileDiscRadius, Vec2& mobileDiscVelocity, const Vec2& fixedDisc, const float& fixedDiscRadius, Vec2& fixedDiscVelocity, float const& elasticity)
{
	UNUSED((void)fixedDiscVelocity);
	if ((!DoDiscsOverlap(mobileDisc, mobileDiscRadius, fixedDisc, fixedDiscRadius)) && fixedDiscRadius != 0.0f)
		return false;

	PushDiscOutOfDisc2D(mobileDisc, mobileDiscRadius, fixedDisc, fixedDiscRadius);

	Vec2 normalVector = (fixedDisc - mobileDisc).GetNormalized();
	Vec2 normalA = GetProjectedOnto2D(mobileDiscVelocity, normalVector);
	Vec2 tangentA = mobileDiscVelocity - normalA;

	mobileDiscVelocity = (-normalA * elasticity ) + tangentA;
	return true;
}
bool BounceDiscOffOBB2D(Vec2& mobileDisc, const float& mobileDiscRadius, Vec2& mobileDiscVelocity, const OBB2& obb, const float& elasticity)
{
	Vec2 point = GetNearestPointOnOBB2D(mobileDisc, obb);
	if (GetDistance2D(mobileDisc, point) < mobileDiscRadius)
	{
		BounceDiscOffPoint(mobileDisc, mobileDiscRadius, mobileDiscVelocity, point, elasticity);
	}
	return false;
}
bool BounceDiscOffCapsule2D(Vec2& mobileDisc, const float& mobileDiscRadius, Vec2& mobileDiscVelocity, const Capsule2& capsule, const float& elasticity)
{
	Vec2 point = GetNearestPointOnCapsule2D(mobileDisc, capsule);
	if (GetDistance2D(mobileDisc, point) < mobileDiscRadius)
	{
		BounceDiscOffPoint(mobileDisc, mobileDiscRadius, mobileDiscVelocity, point, elasticity);
	}
	return false;
}
bool BounceDiscOffPoint(Vec2& mobileDisc, const float& mobileDiscRadius, Vec2& mobileDiscVelocity, const Vec2& point, const float& elasticity)
{
	Vec2 pointVelocity;
	BounceDiscOffDisc(mobileDisc, mobileDiscRadius, mobileDiscVelocity, point, 0.0f, pointVelocity, elasticity);
	return true;
}
bool PushDiscOutOfAABB2D(Vec2& Disc, float radius, const AABB2& box)
{
	Vec2 nearestPoint = box.GetNearestPoint(Disc);

	if (!IsPointInsideDisc(Disc, nearestPoint,radius))
		return false;

	Vec2 pushDirection = Disc - nearestPoint;
	float overlapDepth = radius - pushDirection.GetLength();

	pushDirection.SetLength(overlapDepth);
	Disc += pushDirection;
	return true;
}

float GetFractionWithin(float inputValue, float inputStart, float inputEnd)
{
	float range = inputEnd - inputStart;

	return (inputValue - inputStart) / range;
}
float Interpolate(float pointA, float pointB, float fraction)
{
	float returnValue = pointA + ((pointB - pointA) * fraction);
	return returnValue;
}
Vec2 Interpolate(Vec2 pointA, Vec2 pointB, float fraction)
{
	Vec2 returnValue;
	returnValue.x = pointA.x + ((pointB.x - pointA.x) * fraction);
	returnValue.y = pointA.y + ((pointB.y - pointA.y) * fraction);
	return returnValue;
}
//EulerAngles Interpolate(EulerAngles pointA, EulerAngles pointB, float fraction)
//{
//	EulerAngles returnValue;
//	returnValue.m_yawDegrees = pointA.m_yawDegrees + ((pointB.m_yawDegrees - pointA.m_yawDegrees) * fraction);
//	returnValue.m_pitchDegrees = pointA.m_pitchDegrees + ((pointB.m_pitchDegrees - pointA.m_pitchDegrees) * fraction);
//	returnValue.m_rollDegrees = pointA.m_rollDegrees + ((pointB.m_rollDegrees - pointA.m_rollDegrees) * fraction);
//	return returnValue;
//}
float RangeMap(float inputValue, float inputStart, float inputEnd, float outputStart, float outputEnd)
{
	float fraction = GetFractionWithin(inputValue, inputStart, inputEnd);
	return Interpolate(outputStart, outputEnd, fraction);
}

Vec3 RangeMap(float inputValue, float inputStart, float inputEnd, Vec3 outputStart, Vec3 outputEnd)
{
	float x = RangeMapClamped(inputValue , inputStart ,inputEnd, outputStart.x, outputEnd.x);
	float y = RangeMapClamped(inputValue , inputStart ,inputEnd, outputStart.y, outputEnd.y);
	float z = RangeMapClamped(inputValue , inputStart ,inputEnd, outputStart.z, outputEnd.z);
	return Vec3(x,y,z);
}

float RangeMapZeroToOne(float inputValue, float inputStart, float inputEnd)
{
	float outputStart = 0.0f;
	float outputEnd = 1.0f;
	float fraction = GetFractionWithin(inputValue, inputStart, inputEnd);
	return Interpolate(outputStart, outputEnd, fraction);
}
EulerAngles RangeMapEuler(float inputValue, float inputStart, float inputEnd, EulerAngles outputStart, EulerAngles outputEnd)
{
	float x = RangeMapClamped(inputValue, inputStart, inputEnd, outputStart.m_yawDegrees, outputEnd.m_yawDegrees);
	float y = RangeMapClamped(inputValue, inputStart, inputEnd, outputStart.m_pitchDegrees, outputEnd.m_pitchDegrees);
	float z = RangeMapClamped(inputValue, inputStart, inputEnd, outputStart.m_rollDegrees, outputEnd.m_rollDegrees);
	return EulerAngles(x, y, z);
}
//EulerAngles RangeMap(EulerAngles inputValue, EulerAngles inputStart, EulerAngles inputEnd, EulerAngles outputStart, EulerAngles outputEnd)
//{
//	EulerAngles returnValue;
//	float fractionYaw, fractionPitch, fractionRoll;
//	fractionYaw = GetFractionWithin(inputValue.m_yawDegrees, inputStart.m_yawDegrees, inputEnd.m_yawDegrees);
//	fractionPitch = GetFractionWithin(inputValue.m_pitchDegrees, inputStart.m_pitchDegrees, inputEnd.m_pitchDegrees);
//	fractionRoll = GetFractionWithin(inputValue.m_rollDegrees, inputStart.m_rollDegrees, inputEnd.m_rollDegrees);
//	returnValue.m_yawDegrees = Interpolate(outputStart.m_yawDegrees, outputEnd.m_yawDegrees, fractionYaw);
//	returnValue.m_pitchDegrees = Interpolate(outputStart.m_pitchDegrees, outputEnd.m_pitchDegrees, fractionPitch);
//	returnValue.m_rollDegrees = Interpolate(outputStart.m_rollDegrees, outputEnd.m_rollDegrees, fractionRoll);
//	return returnValue;
//}
float RangeMapClamped(float inputValue, float inputStart, float inputEnd, float outputStart, float outputEnd)
{
	float fraction = GetFractionWithin(inputValue, inputStart, inputEnd);
	float interpolateValue =  Interpolate(outputStart, outputEnd, fraction);
	if (outputStart > outputEnd)
		return Clamp(interpolateValue, outputEnd, outputStart);
	else
		return Clamp(interpolateValue, outputStart, outputEnd);
}
int RoundDownToInt(float value)
{
	int returnValue;
	if (value < 0.0f)
	{
		returnValue = (int)value - 1;
	}
	else
	{
		returnValue = (int)value;
	}

	return returnValue;
}

float GetProjectedLength2D(Vec2 const& project, Vec2 const& onto)
{
	return DotProduct2D(project, onto.GetNormalized());
}
Vec2 const GetProjectedOnto2D(Vec2 const& project, Vec2 const& onto)
{
	Vec2 normalizedOnto = onto.GetNormalized();
	float projLength = DotProduct2D(project, normalizedOnto);
	return Vec2(normalizedOnto.x * projLength, normalizedOnto.y * projLength);
}
float GetProjectedLength3D(Vec3 const& project, Vec3 const& onto)
{
	return DotProduct3D(project, onto.GetNormalized());
}
Vec3 const GetProjectedOnto3D(Vec3 const& project, Vec3 const& onto)
{
	Vec3 normalizedOnto = onto.GetNormalized();
	float projLength = DotProduct3D(project, normalizedOnto);
	return Vec3(normalizedOnto.x * projLength, normalizedOnto.y * projLength, normalizedOnto.z * projLength);
}
void TransformPosition2D(Vec2& posToTransform, Vec2 const& i , Vec2 const& j, const Vec2& translatePos)
{
	posToTransform = translatePos + (i * posToTransform.x) + (j * posToTransform.y);
}
void TransformPositionXY3D(Vec3& posToTransform, Vec2 const& i, Vec2 const& j, const Vec2& translatePos)
{
	Vec2 translatedValue = translatePos + (i * posToTransform.x) + (j * posToTransform.y);
	posToTransform.x = translatedValue.x;
	posToTransform.y = translatedValue.y;
}
void TransformPositionXYZ3D(Vec3& posToTransform, Vec3 const& i, Vec3 const& j, Vec3 const& k, const Vec3& translatePos)
{
	Vec3 translatedValue = translatePos + (i * posToTransform.x) + (j * posToTransform.y) + (k * posToTransform.z);
	posToTransform.x = translatedValue.x;
	posToTransform.y = translatedValue.y;
	posToTransform.z = translatedValue.z;
}
void RotateDegrees(float degreesToRotate, Vec3& positionToRotate)
{
	float x = positionToRotate.x;
	float y = positionToRotate.y;
	float radius = Vec2(x,y).GetLength();
	float deg = atan2f(y, x);
	deg += degreesToRotate;
	x = radius * (float)cos(deg);
	y = radius * (float)sin(deg);
	positionToRotate.x = x;
	positionToRotate.y = y;
}
Vec2 RotateDegrees(float degreesToRotate, float radius)
{
	float x = radius * cosf(degreesToRotate * 3.1415f / 180);
	float y = radius * sinf(degreesToRotate * 3.1415f / 180);
	return Vec2(x, y);
}
void Rotate90Degrees(Vec3& positionToRotate)
{
	float temp = positionToRotate.x;
	positionToRotate.x = positionToRotate.y;
	positionToRotate.y = temp;
}
void Normalize(Vec3& positionToNormalize)
{
	float length = positionToNormalize.GetLength();
	if (length != 0)
	{
		float scale = 1 / length;
		positionToNormalize.x *= scale;
		positionToNormalize.y *= scale;
	}
}
float GetLength(Vec3 positionForLength) 
{
	float x = positionForLength.x;
	float y = positionForLength.y;
	return sqrtf((x * x) + (y * y));
}
float GetDistanceXYSquared3D(const Vec3& posA, const Vec3& posB)
{
	return ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y));
}

float DotProduct2D(Vec2 const& a, Vec2 const& b)
{
	return (a.x * b.x) + (a.y * b.y);
}
float DotProduct3D(Vec3 const& a, Vec3 const& b)
{
	return (a.x * b.x ) + (a.y * b.y ) + (a.z * b.z);
}
float DotProduct4D(Vec4 const& a, Vec4 const& b)
{
	return (a.x * b.x ) + (a.y * b.y) + (a.z * b.z ) + (a.w * b.w);
}
float CrossProduct2D(Vec2 const& a, Vec2 const& b)
{
	Vec3 aVec3 = Vec3(a.x, a.y, 0.0f);
	Vec3 bVec3 = Vec3(b.x, b.y, 0.0f);
	return (CrossProduct3D(aVec3, bVec3)).z;
}
Vec3 CrossProduct3D(Vec3 const& a, Vec3 const& b)
{
	float x = (a.y * b.z) - (a.z * b.y);
	float y = (a.z * b.x) - (a.x * b.z);
	float z = (a.x * b.y) - (a.y * b.x);
	return Vec3(x,y,z);
}
float NormalizeByte(unsigned char byteValue)
{
	return (RangeMap((float)byteValue, 0, 255.0f, 0, 1));
}
unsigned char DenormalizeByte(float zeroToOne)
{
	float rangeMap = 0.0f;
	if (zeroToOne != 1.0f)
	{
		 rangeMap = RangeMap(zeroToOne, 0.0f, 1.0f, 0.0f, 256.0f);
	}
	else
	{
		rangeMap = RangeMap(zeroToOne, 0.0f, 1.0f, 0.0f, 255.0f);
	}
	return (unsigned char)rangeMap;
}
float GetAngleDegreesBetweenVectors2D(Vec2 const& a, Vec2 const& b)
{
	float dotProduct = DotProduct2D(a, b);
	float magnitudes = a.GetLength() * b.GetLength();
	float multiplication = dotProduct / magnitudes;
	float Angle = ConvertRadiansToDegrees(acosf(multiplication));
	return Angle;
}
int GetTaxicabDistance2D(IntVec2 const& a, IntVec2 const& b)
{
	int x = 0;
	int y = 0;
	if (b.x >= a.x)
		x = b.x - a.x;
	if (b.x < a.x)
		x = a.x - b.x;
	if (b.y >= a.y)
		y = b.y - a.y;
	if (b.y < a.y)
		y = a.y - b.y;
	return x + y;
}
float GetSmallest(const float& a, const float& b)
{
	if (a < b) return a;
	else return b;
}
float GetSmallest(const float& a, const float& b, const float& c)
{
	if ((a < b)  && (a < c))
	{
		return a;
	}
	else if((b < a) && (b < c))
	{
		return b;
	}
	else return c;
}
float GetLargest(const float& a, const float& b)
{
	if (a > b) return a;
	else return b;
}
float GetLargest(const float& a, const float& b, const float& c)
{
	if ((a > b) && (a > c))
	{
		return a;
	}
	else if ((b > a) && (b > c))
	{
		return b;
	}
	else return c;
}
int GetSmallest(const int& a, const int& b)
{
	if (a < b) return a;
	else return b;
}
int GetLargest(const int& a, const int& b)
{
	if (a > b) return a;
	else return b;
}
int GetLargest(const int& a, const int& b, const int& c, const int& d)
{
	return GetLargest(GetLargest(a,b), GetLargest(c,d));
}
int GetLargest(const int& a, const int& b, const int& c, const int& d, const int& e, const int& f)
{
	return GetLargest(GetLargest(e,f), GetLargest(GetLargest(a, b), GetLargest(c, d)));
}
float ConvertDegreesToRadians(float degrees)
{
	return (degrees * (3.14159f / 180.0f));
}
float ConvertRadiansToDegrees(float Radians)
{
	return (Radians * (180.0f/ 3.14159f));
}
float CosDegrees(float degrees)
{
	return cosf(degrees * 3.14159f / 180.f);
}
float SinDegrees(float degrees)
{
	return sinf(degrees * 3.14159f / 180.f);
}
float Atan2Degrees(float y, float x)
{
	return (atan2f(y,x) * (180.0f / 3.14159f));
}
float GetDistance2D(const Vec2& posA, const Vec2& posB)
{
	return sqrtf(((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y)));
}
float GetDistanceSquared2D(const Vec2& posA, const Vec2& posB)
{
	return ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y));
}
float GetDistance3D(const Vec3& posA, const Vec3& posB)
{
	return sqrtf(((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z)));
}
float GetDistanceSquared3D(const Vec3& posA, const Vec3& posB)
{
	return ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z));
}
float GetDistanceXY3D(const Vec3& posA, const Vec3& posB)
{
	return sqrtf(((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y)));
}
float ClampZeroToOne(float value)
{
	if (value > 1.0f)
		return 1.0f;
	else if (value < 0.0f)
		return 0.0f;
	else return value;
}
float Clamp(float value, float min, float max)
{
	if (value < min)
		return min;
	else if (value > max)
		return max;
	else
		return value;
}

float GetShortestAngularDispDegrees(float fromDegrees, float toDegrees)
{
	float angDisplacement = toDegrees - fromDegrees;
	while (angDisplacement > 180.f)
	{
		angDisplacement -= 360.0f;
	}
	while (angDisplacement < -180.0f)
	{
		angDisplacement += 360.0f;
	}
	return angDisplacement;
}
float GetTurnedTowardDegrees(float fromDegrees, float toDegrees, float maxTurnDegrees)
{
	float angDisp = GetShortestAngularDispDegrees(fromDegrees, toDegrees);
	if (fabs(angDisp) <= maxTurnDegrees)
	{
		return toDegrees;
	}
	else if (angDisp < 0.0f)
	{
		return fromDegrees - maxTurnDegrees;
	}
	else
	{
		return fromDegrees + maxTurnDegrees;
	}


}
bool DoesFloatlieWithin(float const& inputValue, float const& minimum, float const&  maximum);
bool DoesFloatlieWithin(float const& inputValue, float const& minimum, float const& maximum)
{
	if (inputValue >= minimum && inputValue < maximum)
	{
		return true;
	}
	return false;
}

//---------------------------------------------------------CURVES----------------------------
float ComputeCubicBezier1D(float A, float B, float C, float D, float t)
{
	float AB = Interpolate(A, B, t);
	float BC = Interpolate(B, C, t);
	float CD = Interpolate(C, D, t);
	float ABC = Interpolate(AB, BC, t);
	float BCD = Interpolate(BC, CD, t);
	float ABCD = Interpolate(ABC, BCD, t);
	return ABCD;
}
float ComputeQuinticBezier1D(float A, float B, float C, float D, float E, float F, float t)
{
	float AB = Interpolate(A, B, t);
	float BC = Interpolate(B, C, t);
	float CD = Interpolate(C, D, t);
	float DE = Interpolate(D, E, t);
	float EF = Interpolate(E, F, t);

	float ABC = Interpolate(AB, BC, t);
	float BCD = Interpolate(BC, CD, t);
	float CDE = Interpolate(CD, DE, t);
	float DEF = Interpolate(DE, EF, t);

	float ABCD = Interpolate(ABC, BCD, t);
	float BCDE = Interpolate(BCD, CDE, t);
	float CDEF = Interpolate(CDE, DEF, t);

	float ABCDE = Interpolate(ABCD, BCDE, t);
	float BCDEF = Interpolate(BCDE, CDEF, t);

	float ABCDEF = Interpolate(ABCDE, BCDEF, t);
	return ABCDEF;
}
float Identity(float t)
{
	return t;
}
float SmoothStart2(float t)
{
	return t * t;
}

float SmoothStart3(float t)
{
	return t * t * t;
}

float SmoothStart4(float t)
{
	return t * t * t * t;
}

float SmoothStart5(float t)
{
	return t * t * t * t * t;
}

float SmoothStart6(float t)
{
	return t * t * t * t * t * t;
}

float SmoothStop2(float t)
{
	float value = 1 - t;
	return (1-(value * value));
}

float SmoothStop3(float t)
{
	float value = 1 - t;
	return (1 - (value * value * value));
}

float SmoothStop4(float t)
{
	float value = 1 - t;
	return (1 - (value * value * value * value));
}

float SmoothStop5(float t)
{	
	float value = 1 - t;
	return (1 - (value * value * value * value * value));
}

float SmoothStop6(float t)
{
	float value = 1 - t;
	return (1 - (value * value * value * value * value * value));
}

float SmoothStep3(float t)
{
	float returnvalue = ComputeCubicBezier1D(0, 0, 1, 1, t);
	return returnvalue;
}

float SmoothStep5(float t)
{
	float returnvalue = ComputeQuinticBezier1D(0, 0, 0, 1, 1, 1, t);
	return returnvalue;
}

float Hesitate3(float t)
{
	float returnvalue = ComputeCubicBezier1D(0, 1, 0, 1, t);
	return returnvalue;
}

float Hesitate5(float t)
{
	float returnvalue = ComputeQuinticBezier1D(0, 1, 0, 1, 0, 1, t);
	return returnvalue;
}
float CustomFunky(float t)
{
	float tsquared = t * t;
	float tCube = t * t * t;
	float tquadrapule = t * t * t * t;
	return  t - tsquared + tCube - (tsquared * tsquared) + (tCube*tCube)- tquadrapule + (tquadrapule * tquadrapule);
}
