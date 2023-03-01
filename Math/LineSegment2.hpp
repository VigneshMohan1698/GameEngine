#pragma once
#include "Engine/Math/Vec2.hpp"
class LineSegment2
{
public:
	Vec2 m_start;
	Vec2 m_end;
	float radius = 0.0f;
	LineSegment2();
	LineSegment2(Vec2 start, Vec2 end);

	void			Translate(Vec2 const& translation);
	void			SetCenter(Vec2 const& newCenter);
	void			RotateAboutCenter(Vec2 const& floatRotationDegrees);
};

