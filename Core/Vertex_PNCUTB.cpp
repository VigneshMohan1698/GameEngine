#include "Vertex_PNCUTB.hpp"


Vertex_PNCUTB::Vertex_PNCUTB(Vertex_PNCU pncu)
{
	m_position = pncu.m_position;;
	m_normal = pncu.m_normal;
	m_tangent = Vec3();
	m_bitangent = Vec3();
	m_color = Vec4();
	m_uvTexCoords = pncu.m_uvTexCoords;
}
