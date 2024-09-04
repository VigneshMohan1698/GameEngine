#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/AABB3.hpp"

struct Sphere3D
{
public:
	Vec3	m_center;
	float	m_radius = 0.0f;

public:
	Sphere3D() {}
	Sphere3D(Vec3 center, float radius);
	~Sphere3D() {};

};

struct Cube
{
public:
	AABB3 m_bounds = AABB3();

public:
	Cube() {};
	Cube(AABB3 bounds) : m_bounds(bounds) {}

	~Cube() {};

};

//struct Plane
//{
//	public:
//	// Mostly used to create a simple plane mesh
//	Plane3D m_plane;
//	Vec3 m_dimensions;
//	Vec3 m_perpendicularVector;
//
//	Plane() {};
//	Plane(Plane3D plane, Vec2 dimensions, Vec3 perpendicularVector) : m_plane(plane), m_dimensions(dimensions), m_perpendicularVector(perpendicularVector) {}
//
//	~Plane() {};
//
//};
