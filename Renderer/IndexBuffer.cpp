#include <d3d11.h>
#include "IndexBuffer.hpp"
#include "Engine/Renderer/Renderer.hpp"

IndexBuffer::IndexBuffer(size_t size)
{
	m_size = size;
}

IndexBuffer::~IndexBuffer()
{
	DX_SAFE_RELEASE(m_buffer);
}

unsigned int IndexBuffer::GetStride() const
{
	return sizeof(Vertex_PCU);
}
