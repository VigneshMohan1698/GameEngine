#pragma once
#include "Engine/Math/Vec2.hpp"
#include <vector>

class CubicHermiteCurve2D;

enum StepModes
{
	SmoothStarts2,
	SmoothStarts3,
	SmoothStarts4,
	SmoothStarts5,
	SmoothStarts6,
	SmoothStops2,
	SmoothStops3,
	SmoothStops4,
	SmoothStops5,
	SmoothStops6,
	SmoothSteps3,
	SmoothSteps5,
	Hesitates3,
	Hesitates5,
	Identitys,
	Custom,
	TotalSteps
};
class Spline
{
};
class CubicBezierCurve2D
{
public:
	CubicBezierCurve2D();
	CubicBezierCurve2D(Vec2 startPos, Vec2 guidePos1, Vec2 guidePos2, Vec2 endPos);
	CubicBezierCurve2D(Vec2 a, Vec2 b, Vec2 c, float t, float parameticeZeroToOne);
	explicit CubicBezierCurve2D(const CubicHermiteCurve2D& bezierCurve);
	float GetApproximateLength(int numSubdivisions);
	Vec2 EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubdivisions);
	Vec2 EvaluateAtParametric(const float& value) const;
public:
	Vec2 m_a;
	Vec2 m_b;
	Vec2 m_c;
	Vec2 m_d;

	Vec2 ab;
	Vec2 bc;
	Vec2 cd;
	Vec2 abcd;
};
class CubicHermiteCurve2D
{
public:
	CubicHermiteCurve2D() {};
	CubicHermiteCurve2D(Vec2 startPoint, Vec2 startVelocity, Vec2 endPoint, Vec2 endVelocity);
	explicit CubicHermiteCurve2D(const CubicBezierCurve2D& bezierCurve);
	float GetApproximateLength(int numSubdivisions = 64);
	Vec2 EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubdivisions = 64);
public:
	Vec2 m_startPoint;
	Vec2 m_endPoint;
	Vec2 m_startVelocity;
	Vec2 m_endVelocity;
};
class CatmullRomSpline2D
{
public:
	CatmullRomSpline2D() {};
	CatmullRomSpline2D(std::vector<Vec2> const& position);
	std::vector<Vec2> m_positions;
	std::vector<Vec2> m_velocities;
protected:


};