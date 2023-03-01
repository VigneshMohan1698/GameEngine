#pragma once
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Core/Rgba8.hpp"

#include <Engine/Core/Vertex_PNCU.hpp>

//------------------------------------------------------------------------------------------------
struct Vertex_PNCUT
{
	Vertex_PNCUT() = default;
	Vertex_PNCUT(Vec3 const& position, Vec3 const& normal,Vec3 const& tangent, Vec4 const& color, Vec2 const& uvTexCoords)
		: m_position(position)
		, m_normal(normal)
		, m_tangent(tangent)
		, m_color(color)
		, m_uvTexCoords(uvTexCoords)
	{}

	Vertex_PNCUT(float px, float py, float pz, float nx, float ny, float nz, unsigned char r, unsigned char g, unsigned char b, unsigned char a, float u, float v)
		: m_position(px, py, pz)
		, m_normal(nx, ny, nz)
		, m_color(r, g, b, a)
		, m_uvTexCoords(u, v)
	{}
	Vertex_PNCUT (Vertex_PNCU pncu);
	Vec3	m_position;
	Vec3	m_normal;
	Vec3	m_tangent;
	Vec4	m_color;
	Vec2	m_uvTexCoords;
};

