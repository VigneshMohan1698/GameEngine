#pragma once
#include <Engine\Math\Vec3.hpp>
#include <Engine\Math\EulerAngles.hpp>
#include <Engine\Core\Model.hpp>
#include <Engine\Material\Material.hpp>

struct TransformComponent
{
	Vec3 m_position;
	EulerAngles m_oritentationDegrees;
	float m_scale;
};

struct MeshComponent
{
	Mesh			m_mesh;
	Material		m_material;
};