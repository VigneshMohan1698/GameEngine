#pragma once
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Core/Rgba8.hpp"

//------------------------------------------------------------------------------------------------
struct Vertex_PCU
{
public:
	Vec3 m_position;
	Rgba8 m_color;
	Vec2 m_uvTexCoords;
	Vertex_PCU() {};
	explicit Vertex_PCU(Vec3 const& position, Rgba8 const& tint, Vec2 const& uvTexCoords)
	{
		m_position = position;
		m_color = tint;
		m_uvTexCoords = uvTexCoords;
	}
};

//------------------------------------------------------------------------------------------------
struct Vertex_PNCU
{
	Vertex_PNCU() = default;
	Vertex_PNCU(Vec3 const& position, Vec3 const& normal, Rgba8 const& color, Vec2 const& uvTexCoords)
		: m_position(position)
		, m_normal(normal)
		, m_color(color)
		, m_uvTexCoords(uvTexCoords)
	{}

	Vertex_PNCU(float px, float py, float pz, float nx, float ny, float nz, unsigned char r, unsigned char g, unsigned char b, unsigned char a, float u, float v)
		: m_position(px, py, pz)
		, m_normal(nx, ny, nz)
		, m_color(r, g, b, a)
		, m_uvTexCoords(u, v)
	{}

	Vec3	m_position;
	Vec3	m_normal;
	Rgba8	m_color;
	Vec2	m_uvTexCoords;
};


//------------------------------------------------------------------------------------------------
struct Vertex_PNCUT
{
	Vertex_PNCUT() = default;
	Vertex_PNCUT(Vec3 const& position, Vec3 const& normal, Vec3 const& tangent, Vec4 const& color, Vec2 const& uvTexCoords)
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

	Vertex_PNCUT(Vertex_PNCU pncu)
	{
		m_position = pncu.m_position;;
		m_normal = pncu.m_normal;
		m_tangent = Vec3();
		m_color = Vec4();
		m_uvTexCoords = pncu.m_uvTexCoords;
	}

	Vec3	m_position;
	Vec3	m_normal;
	Vec3	m_tangent;
	Vec4	m_color;
	Vec2	m_uvTexCoords;
};


//------------------------------------------------------------------------------------------------
struct Vertex_PNCUTB
{
	Vertex_PNCUTB() = default;
	Vertex_PNCUTB(Vec3 const& position, Vec3 const& normal, Vec3 const& tangent, Vec4 const& color, Vec2 const& uvTexCoords)
		: m_position(position)
		, m_normal(normal)
		, m_tangent(tangent)
		, m_color(color)
		, m_uvTexCoords(uvTexCoords)
	{}

	Vertex_PNCUTB(float px, float py, float pz, float nx, float ny, float nz, unsigned char r, unsigned char g, unsigned char b, unsigned char a, float u, float v)
		: m_position(px, py, pz)
		, m_normal(nx, ny, nz)
		, m_color(r, g, b, a)
		, m_uvTexCoords(u, v)
	{}
	Vertex_PNCUTB(Vertex_PNCU pncu) 
	{
		m_position = pncu.m_position;;
		m_normal = pncu.m_normal;
		m_tangent = Vec3();
		m_bitangent = Vec3();
		m_color = Vec4();
		m_uvTexCoords = pncu.m_uvTexCoords;
	}

public:
	Vec3	m_position;
	Vec3	m_normal;
	Vec4	m_color;
	Vec3	m_tangent;
	Vec3	m_bitangent;
	Vec2	m_uvTexCoords;
};

