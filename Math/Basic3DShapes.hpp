#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"

struct Sphere3D
{
public:
	Vec3	m_center;
	float	m_radius = 0.0f;

public:
	Sphere3D() {};
	Sphere3D(Vec3 center, float radius);
	~Sphere3D() {};

};

struct Cube
{
public:
	Vec3 m_center;
	float m_sideLength;

public:
	Cube() {};
	Cube(Vec3 center, float length) : m_center(center), m_sideLength(length) {}

	~Cube() {};

};

