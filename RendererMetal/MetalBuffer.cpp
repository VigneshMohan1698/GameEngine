
//  MetalBuffer.cpp
//  Albus
//
//  Created by Vignesh Mohan on 2/24/24.

#include "MetalBuffer.hpp"
#include "../Core/EngineIncludeHeaders.hpp"
#include "RendererMetal.hpp"
#include "iostream"

MTL::Buffer* MetalBuffer::GetBufferObject()
{
    return m_buffer;
}

MetalBuffer::MetalBuffer(RendererMetal* renderer, const size_t bufferSize, BufferType type)
{
    g_renderer = renderer;
    m_buffer = g_renderer->m_device->newBuffer( bufferSize, MTL::ResourceStorageModeManaged );
    m_size = bufferSize;
    m_type = type;
}

void MetalBuffer::UpdateBuffer(const void* data)
{
    memcpy( m_buffer->contents(), data, m_size );
    m_buffer->didModifyRange( NS::Range::Make( 0, m_buffer->length()));
}

void MetalBuffer::ReleaseBuffer()
{
    m_buffer->release();
}

MetalBuffer::~MetalBuffer()
{
}


