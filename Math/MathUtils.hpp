#pragma once
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/Capsule2.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/LineSegment2.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Core/RaycastResult2D.hpp"
#include "Engine/Core/RaycastResult3D.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include <ThirdParty/Mikkt/mikktspace.h>
#include "Engine/Math/ConvexShapes.hpp"
//------------------------------------------
// Forward type  declarations

struct Vec2;
struct Vec3;
class FloatRange;
typedef unsigned int uint;

struct mikkt_data_t
{
	Vec3* pos;
	uint pos_stride;

	Vec3* normal;
	uint normal_stride;

	Vec2* uv;
	uint uv_stride;

	Vec4* tangent;
	uint tangent_stride;

	uint vertex_count;
};

class MikktSpace : public SMikkTSpaceInterface
{ 
	public:
	static int MikkTGetNumFaces(SMikkTSpaceContext const* pcontext)
	{
		mikkt_data_t const* data = (mikkt_data_t const*)pcontext->m_pUserData;
		return data->vertex_count / 3;
	}

	//------------------------------------------------------------------------
	static int MikkTGetVerticesOfFace(SMikkTSpaceContext const*, int)
	{
		return 3;
	}

	//------------------------------------------------------------------------
	static void MikkTGetPosition(SMikkTSpaceContext const* pcontext,
		float out_pos[],
		int const iface,
		int const ivert)
	{
		mikkt_data_t const* data = (mikkt_data_t const*)pcontext->m_pUserData;
		uint offset = (iface * 3) + ivert;
		Vec3* pos = data->pos +  data->pos_stride * offset;

		out_pos[0] = pos->x;
		out_pos[1] = pos->y;
		out_pos[2] = pos->z;
	}

	//------------------------------------------------------------------------
	static void MikkTGetNormal(SMikkTSpaceContext const* pcontext,
		float out_normal[],
		int const iface,
		int const ivert)
	{
		mikkt_data_t const* data = (mikkt_data_t const*)pcontext->m_pUserData;
		uint offset = (iface * 3) + ivert;
		Vec3* normal = data->normal +  data->normal_stride * offset;

		out_normal[0] = normal->x;
		out_normal[1] = normal->y;
		out_normal[2] = normal->z;
	}

	//------------------------------------------------------------------------
	static void MikkTGetUV(SMikkTSpaceContext const* pcontext,
		float out_uv[],
		int const iface,
		int const ivert)
	{
		mikkt_data_t const* data = (mikkt_data_t const*)pcontext->m_pUserData;
		uint offset = (iface * 3) + ivert;
		Vec2* uv = data->uv +  data->uv_stride * offset;

		out_uv[0] = uv->x;
		out_uv[1] = uv->y;
	}

	//------------------------------------------------------------------------
	static void MikkTSetTSpaceBasic(SMikkTSpaceContext const* pcontext,
		float const tangent[],
		float const fsign,
		const int iface,
		const int ivert)
	{
		mikkt_data_t const* data = (mikkt_data_t const*)pcontext->m_pUserData;
		uint offset = (iface * 3) + ivert;
		Vec4* out_tan = data->tangent +  data->tangent_stride * offset;

		*out_tan = Vec4(tangent[0], tangent[1], tangent[2], fsign);
	}

};

struct Transform 
{
	Vec3 position;
	EulerAngles orientation;
};
//-----------------------------SIMPLE MATH--------------------------------
float				GetSmallest(const float& a, const float& b);
float				GetSmallest(const float& a, const float& b, const float& c);
float				GetLargest(const float& a, const float& b);
float				GetLargest(const float& a, const float& b, const float& c);
int					GetSmallest(const int& a, const int& b);
int					GetLargest(const int& a, const int& b);
int					GetLargest(const int& a, const int& b, const int& c, const int& d);
int					GetLargest(const int& a, const int& b, const int& c, const int& d, const int& e, const int& f);

//------------------------------ANGLE UTILITIES ---------------------------
float				ConvertDegreesToRadians(float degrees);
float				ConvertRadiansToDegrees(float Radians);
float				CosDegrees(float degrees);
float				SinDegrees(float degrees);
float				Atan2Degrees(float y, float x);

//---------------------------BASIC 2D AND 3D UTILS -------------------------
float				GetDistance2D(const Vec2& posA, const Vec2& posB);
float				GetDistanceSquared2D(const Vec2& posA, const Vec2& posB);
float				GetDistance3D(const Vec3& posA, const Vec3& posB);
float				GetDistanceSquared3D(const Vec3& posA, const Vec3& posB);
float				GetDistanceXY3D(const Vec3& posA, const Vec3& posB);
float				GetDistanceXYSquared3D(const Vec3& posA, const Vec3& posB);
float				DotProduct2D(Vec2 const& a, Vec2 const& b);
float				DotProduct3D(Vec3 const& a, Vec3 const& b);
float				DotProduct4D(Vec4 const& a, Vec4 const& b);
float				CrossProduct2D(Vec2 const& a, Vec2 const& b);
Vec3				CrossProduct3D(Vec3 const& a, Vec3 const& b);
float				NormalizeByte(unsigned char byteValue);
unsigned char		DenormalizeByte(float zeroToOne);
float				GetAngleDegreesBetweenVectors2D(Vec2 const& a, Vec2 const& b);
int					GetTaxicabDistance2D(IntVec2 const& a, IntVec2 const& b);


//--------------------------GEOMETRY QUERY ABILITIES-----------------------
bool DoDiscsOverlap(const Vec2& centerA, float radiusA, const Vec2& centerB, float radiusB);
bool DoSpheresOverlap(const Vec3& centerA, float radiusA, const Vec3& centerB, float  radiusB);
bool DoSpheresOverlap(const Vec2& centerA, float radiusA, const Vec2& centerB, float radiusB);
void TransformPosition2D(Vec2& posToTransform, float uniformScale, float rotationDegrees, const Vec2& translatePos);
void TransformPositionXY3D(Vec3& posToTransform, float uniformScale, float rotationDegrees, const Vec2& translatePos);


//------------PUSH FUNCTIONS-------------------------------------
bool PushDiscOutOfPoint2D(Vec2& MobileDiscCenter, float radius, const Vec2& fixedPoint);
bool PushDiscOutOfDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2 const& fixedDiscCenter, float fixedDiscRadius);
bool PushDiscOutOfAABB2D(Vec2& Disc, float radius, const AABB2& box);
bool PushDiscsOutOfEachOther2D(Vec2& discA, float radiusA,Vec2& discB, float radiusB);
Vec3 PushAABB3DoutOfAABB3D(AABB3& mobileAABB3, AABB3 fixedAABB3, float tolerance);
	 
//-- ---------BOUNCE FUNCTIONS---------------------------------------
bool BounceDiscOffEachOther(Vec2& discA, const float& radiusA, Vec2& velocityA,Vec2& discB, const float& radiusB, Vec2& velocityB, const float& elasticity);
bool BounceDiscOffDisc(Vec2& mobileDisc, const float& mobileDiscRadius, Vec2& mobileDiscVelocity, const Vec2& fixedDisc, const float& fixedDiscRadius, Vec2& fixedDiscVelocity, float const& elasticity);
bool BounceDiscOffOBB2D(Vec2& mobileDisc, const float& mobileDiscRadius, Vec2& mobileDiscVelocity, const OBB2& obb, const float& elasticity);
bool BounceDiscOffCapsule2D(Vec2& mobileDisc, const float& mobileDiscRadius, Vec2& mobileDiscVelocity, const Capsule2& capsule, const float& elasticity);
bool BounceDiscOffPoint(Vec2& mobileDisc, const float& mobileDiscRadius, Vec2& mobileDiscVelocity, const Vec2& point, const float& elasticity);

//-------------------------PROJECTED LENGTH --------------------------------------
float				 GetProjectedLength2D(Vec2 const& project, Vec2 const& onto);
Vec2				const GetProjectedOnto2D(Vec2 const& project, Vec2 const& onto);
float				 GetProjectedLength3D(Vec3 const& project, Vec3 const& onto);
Vec3				const GetProjectedOnto3D(Vec3 const& project, Vec3 const& onto);

//-------------------------TRANFORM POSITIONS-------------------------------------
void				TransformPosition2D(Vec2& posToTransform, Vec2 const& i, Vec2 const& j, const Vec2& translatePos);
void				TransformPositionXY3D(Vec3& posToTransform, Vec2 const& i, Vec2 const& j, const Vec2& translatePos);

//------------------------------ROTATE FUNCTIONS ----------------------------------------------
void				RotateDegrees(float degreesToRotate, Vec3& positionToRotate);
Vec2				RotateDegrees(float degreesToRotate, float radius);
void				Rotate90Degrees(Vec3& positionToRotate);

//------------------ LERP AND CLAMP UTILITIES---------------------------------------
float				ClampZeroToOne(float value);
float				Clamp(float value, float min, float max);
float				GetShortestAngularDispDegrees(float fromDegrees, float toDegrees);
float				GetTurnedTowardDegrees(float fromDegrees, float toDegrees, float maxTurnDegrees);
float				GetFractionWithin(float inputValue, float inputStart, float inputEnd);
float				Interpolate(float pointA, float pointB, float fraction);
Vec2				Interpolate(Vec2 pointA, Vec2 pointB, float fraction);
EulerAngles			Interpolate(EulerAngles pointA, EulerAngles pointB, float fraction);
float				RangeMap(float inputValue, float inputStart, float inputEnd, float outputStart, float outputEnd);
Vec3				RangeMap(float inputValue, float inputStart, float inputEnd, Vec3 outputStart, Vec3 outputEnd);
float				RangeMapZeroToOne(float inputValue, float inputStart, float inputEnd);
EulerAngles			RangeMapEuler(float inputValue, float inputStart, float inputEnd, EulerAngles outputStart, EulerAngles outputEnd);
float				RangeMapClamped(float inputValue, float inputStart, float inputEnd, float outputStart, float outputEnd);
int					RoundDownToInt(float value);
bool				OverlapAABB2DVsAABB2D(const AABB3& a, const AABB3& b);

//------------------ IS POINT INSIDE FUNCTIONS -----------------------------------
bool				IsPointInsideDisc(const Vec2& centerA, const Vec2& point, float radius);
bool				IsPointInsideOrientedSector2D(const Vec2& point, const Vec2& center, float orientation, float aperture, float radius);
bool				IsPointInsideDirectedSector2D(const Vec2& point, const Vec2& center, const Vec2& i, float aperture, float radius);
bool				IsPointInsideAABB2D(Vec2 const& point, AABB2 const& box);
bool				IsPointInsideCapsule2D(Vec2 const& point, Capsule2 const& capsule);
bool				IsPointInsideCapsule2D(Vec2 const& point, Vec2 const& boneStart, Vec2 const& boneEnd, float radius);
bool				IsPointInsideOBB2D(Vec2 const& point, OBB2 const& orientedBox);

//------------------- GET NEAREST POINT 2D---------------------------------------
Vec2 const 			GetNearestPointOnDisc2D(Vec2 const& point, Vec2 const& center, float radius);
Vec2 const			GetNearestPointOnInfiniteLine2D(Vec2 const& referencePos, LineSegment2 const& infiniteLine);
Vec2 const			GetNearestPointOnInfiniteLine2D(Vec2 const& referencePos, Vec2 const& pointOnLine, Vec2 const& anotherPointOnLine);
Vec2 const			GetNearestPointOnLineSegment2D(Vec2 const& referencePos, LineSegment2 const& lineSegment);
Vec2 const			GetNearestPointOnLineSegment2D(Vec2 const& referencePos, Vec2 const& lineSegmentStart, Vec2 const& lineSegmentEnd);
Vec2 const			GetNearestPointOnCapsule2D(Vec2 const& referencePos, Capsule2 const& capsule);
Vec2 const			GetNearestPointOnCapsule2D(Vec2 const& referencePos, Vec2 const& boneStart, Vec2 const& boneEnd, float radius);
Vec2 const			GetNearestPointOnAABB2D(Vec2 const& referencePos, AABB2 const& box);
Vec2 const			GetNearestPointOnOBB2D(Vec2 const& referencePos, OBB2 const& orientedBox);

//------------------- GET NEAREST POINT 3D---------------------------------------
Vec3 const			GetNearestPointOnAABB3(Vec3 const& referencePoint, AABB3 const& bounds);
Vec3 const			GetNearestPointOnSphere3D(Vec3 const& referencePoint, Vec3 const& sphereCenter, float const& sphereRadius);
Vec3 const			GetNearestPointOnZCylinder(Vec3 const& referencePoint, Vec2 const& cylinderXY, float const& cylinderRadius, FloatRange const& zRange);

//--------------------- RAYCAST FUNCTIONS -----------------------------------------
RaycastResult2D		RaycastVsDisc2D(Vec2 startingPoint,Vec2 endingPoint, Vec2 discCenter, float DiscRadius, float maxDistance, float stepsToTake);
RaycastResult2D     RaycastVsDisc2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, Vec2 discCenter, float discRadius);
RaycastResult2D     RaycasVsConvexHull2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, ConvexHull2D hull);
PlaneRaycastResult2D     RaycasVsPlane2D(const Vec2& startPos, const Vec2& fwdNormal, const float& maxDist, Plane2D plane);
PlaneRaycastResult3D RaycastVsPlane3D(const Vec3& startPos, const Vec3& fwdNormal, const float& maxDist, Plane3D plane);
RaycastResult2D     RaycastVsAABB2(Vec2 startPos, Vec2 fwdNormal, float maxDist,AABB2 aabbBounds);
RaycastResult2D		RaycasVsOBB2(Vec2 startPos, Vec2 fwdNormal, float maxDist, OBB2 obbBounds);
RaycastResult2D		RaycastVsLineSegment2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, LineSegment2 line);
RaycastResult3D		RaycastVsAABB3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, AABB3 aabbBounds);
RaycastResult3D		RaycastVsZCylinder(Vec3 startPos, Vec3 fwdNormal, float maxDist, Vec3 cylinderPosition, FloatRange zRange, float cylinderRadius);
RaycastResult3D		RaycastVsSphere3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, Vec3 spherePosition, float const& sphereRadius);

//-------------------- OBJECT OVERLAP 3D FUNCTIONS------------------------------------------
bool				OverlapAABB3DVsAABB3D(const AABB3& a, const AABB3& b);
bool				OverlapAABB3DVsSphere(const AABB3& a, const Vec3& sphereCenter, const float& sphereRadius);
bool				OverlapZClyinderVsSphere(Vec2 const& cylinderXY, float const& cylinderRadius, FloatRange const& zRange, const Vec3& sphereCenter, const float& sphereRadius);
bool				OverlapZCylinderVsZCylinder(Vec2 const& cylinderXYA, float const& cylinderRadiusA, FloatRange const& zRangeA, Vec2 const& cylinderXYB, float const& cylinderRadiusB, FloatRange const& zRangeB);
bool				OverlapZCylinderVsAABB3D(Vec2 const& cylinderXY, float const& cylinderRadius, FloatRange const& zRange, const AABB3& bounds);

//---------------------CURVES----------------------------
 float				ComputeCubicBezier1D(float A, float B, float C, float D, float t);
 float				ComputeQuinticBezier1D(float A, float B, float C, float D, float E, float F, float t);
 float				Identity(float t);
 float				SmoothStart2(float t);
 float				SmoothStart3(float t);
 float				SmoothStart4(float t);
 float				SmoothStart5(float t);
 float				SmoothStart6(float t);

 float				SmoothStop2(float t);
 float				SmoothStop3(float t);
 float				SmoothStop4(float t);
 float				SmoothStop5(float t);
 float				SmoothStop6(float t);

 float				SmoothStep3(float t);
 float				SmoothStep5(float t);
 float				Hesitate3(float t);
 float				Hesitate5(float t);
 float				CustomFunky(float t);