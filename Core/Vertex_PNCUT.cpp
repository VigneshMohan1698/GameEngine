#include "Vertex_PNCUT.hpp"


Vertex_PNCUT::Vertex_PNCUT(Vertex_PNCU pncu)
{
	m_position = pncu.m_position;;
	m_normal = pncu.m_normal;
	m_tangent = Vec3();
	m_color = Vec4();
	m_uvTexCoords = pncu.m_uvTexCoords;
}
