#include "LineSegment2.hpp"
#include "Engine/Core/EngineCommon.hpp"

LineSegment2::LineSegment2()
{
}

LineSegment2::LineSegment2(Vec2 start, Vec2 end)
{
	m_start = start;
	m_end = end;
}

void LineSegment2::Translate(Vec2 const& translation)
{
	UNUSED((void)translation);
}

void LineSegment2::SetCenter(Vec2 const& newCenter)
{
	UNUSED((void)newCenter);
}

void LineSegment2::RotateAboutCenter(Vec2 const& floatRotationDegrees)
{
	UNUSED((void)floatRotationDegrees);
}
