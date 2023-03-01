#include <d3d11.h>
#include "VertexBuffer.hpp"
#include "Engine/Renderer/Renderer.hpp"

VertexBuffer::VertexBuffer(size_t size)
{
	m_size = size;
	m_stride = (unsigned int)size;
}

VertexBuffer::VertexBuffer(size_t size, unsigned int stride)
{
	m_size = size;
	m_stride = stride;
}

VertexBuffer::~VertexBuffer()
{
	DX_SAFE_RELEASE(m_buffer);
}

unsigned int VertexBuffer::GetStride() const
{
	return m_stride;
}
