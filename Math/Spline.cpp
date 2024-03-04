#include "Spline.hpp"
#include "Engine/Math/MathUtils.hpp"

//-------------------------CUBIC BEZIER------------------------
CubicBezierCurve2D::CubicBezierCurve2D()
{

}
CubicBezierCurve2D::CubicBezierCurve2D(Vec2 a, Vec2 b, Vec2 c, float t, float parameticeZeroToOne)
{
	//UNUSED((void)parameticeZeroToOne);
	m_a = a;
	m_b = b;
	m_c = c;
	ab = Interpolate(m_a, m_b, t);
	bc = Interpolate(m_b, m_c, t);
}
CubicBezierCurve2D::CubicBezierCurve2D(Vec2 startPos, Vec2 guidePos1, Vec2 guidePos2, Vec2 endPos)
{
	m_a = startPos;
	m_b = guidePos1;
	m_c = guidePos2;
	m_d = endPos;


}
CubicBezierCurve2D::CubicBezierCurve2D(const CubicHermiteCurve2D& hermitCurve)
{
	m_a = hermitCurve.m_startPoint;
	m_b = (hermitCurve.m_startPoint + (hermitCurve.m_startVelocity / 3.0f));
	m_c = (hermitCurve.m_endPoint - (hermitCurve.m_endVelocity / 3.0f));
	m_d = hermitCurve.m_endPoint;
	
}
float CubicBezierCurve2D::GetApproximateLength(int numSubdivisions)
{
	float tperStep = 1.f / float(numSubdivisions);
	float curveLength = 0.0f;
	Vec2 endPos;
	Vec2 startPos = m_a;
	for (int i = 1; i <= numSubdivisions; i++)
	{
		endPos = EvaluateAtParametric(tperStep * float(i));
		curveLength += GetDistance2D(endPos, startPos);
		startPos = endPos;
	}
	return curveLength;
}

Vec2 CubicBezierCurve2D::EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubdivisions)
{
	float tperStep = 1.f / float(numSubdivisions);
	Vec2 startPos = m_a;
	Vec2 endPos = m_a;
	float distanceTravelled = 0.0f;
	float subdivLength = 0.0f;
	for (int i = 1; i <=numSubdivisions; i++)
	{
		endPos = EvaluateAtParametric(tperStep * (float)i);
		subdivLength = GetDistance2D(endPos, startPos);
		float lengthFraction = (distanceAlongCurve - distanceTravelled) / subdivLength;
		if (distanceTravelled + subdivLength > distanceAlongCurve)
		{
			return Interpolate(startPos, endPos, lengthFraction);
		}
		startPos = endPos;
		distanceTravelled += subdivLength;
	}
	return Vec2();
}

Vec2 CubicBezierCurve2D::EvaluateAtParametric(const float& value) const
{
	Vec2 AB, BC, CD, ABC, BCD,ABCD;
	AB = Interpolate(m_a, m_b, value);
	BC = Interpolate(m_b, m_c, value);
	CD = Interpolate(m_c, m_d, value);

	ABC = Interpolate(AB, BC, value);
	BCD = Interpolate(BC, CD, value);

	ABCD = Interpolate(ABC, BCD, value);
	return ABCD;
}

//-------------------------CUBIC HERMITE------------------------

CubicHermiteCurve2D::CubicHermiteCurve2D(Vec2 startPoint, Vec2 startVelocity, Vec2 endPoint, Vec2 endVelocity)
{
	m_startPoint = startPoint;
	m_startVelocity = startVelocity;
	m_endPoint = endPoint;
	m_endVelocity = endVelocity;
}

CubicHermiteCurve2D::CubicHermiteCurve2D(const CubicBezierCurve2D& bezierCurve)
{ 
	m_startPoint = bezierCurve.m_a;
	m_startVelocity = (bezierCurve.m_b - bezierCurve.m_a) * 3;
	m_endPoint = bezierCurve.m_d;
	m_endVelocity = (bezierCurve.m_d- bezierCurve.m_c) * 3;
}

float CubicHermiteCurve2D::GetApproximateLength(int numSubdivisions)
{
	//UNUSED((void)numSubdivisions);
	return 0.0f;
}

Vec2 CubicHermiteCurve2D::EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubdivisions)
{
//	UNUSED((void)numSubdivisions);
//	UNUSED((void)distanceAlongCurve);
	return Vec2();
}

//--------------------SPLINE-----------------------------
CatmullRomSpline2D::CatmullRomSpline2D(std::vector<Vec2> const& position)
{
	m_positions = position;
	for (int i = 0; i < (int)position.size(); i++)
	{

		m_velocities.push_back(Vec2::ZERO);
	}
	Vec2 velocity;
	for (int i = 0; i < (int)position.size(); i++)
	{
		if (i == 0 || i == (int)position.size() - 1)
		{
			m_velocities[i] = velocity;
			continue;
		}
		velocity = ((position[i + 1] - position[i - 1]) / 2.0f);
		m_velocities[i] = velocity;
	}

	
}
