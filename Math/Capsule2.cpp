#include "Capsule2.hpp"
Capsule2::Capsule2()
{
}
Capsule2::Capsule2(Vec2 start, Vec2 end, float radius)
{
	m_start = start;
	m_end = end;
	this->radius = radius;
}