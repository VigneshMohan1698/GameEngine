#pragma once
#include "Engine/Math/Vec2.hpp"
class Capsule2
{
public:		
	Capsule2();
	Capsule2(Vec2 start, Vec2 end, float radius);
	Vec2 m_start;
	Vec2 m_end;
	float radius;

};

