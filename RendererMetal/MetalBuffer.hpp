//
//  MetalBuffer
//  Albus
//
//  Created by Vignesh Mohan on 2/24/24.
//

#pragma once

#ifndef MetalBuffer_hpp
#define MetalBuffer_hpp

#include <stddef.h>
#include "../Core/EngineIncludeHeaders.hpp"

class RendererMetal;


enum class BufferType
{
    ArgumentBuffer,
    VertexBuffer
};

class MetalBuffer
{
    friend class RendererMetal;
public:
    MetalBuffer() {};
    MetalBuffer(RendererMetal* renderer, const size_t bufferSize, BufferType type);
    ~MetalBuffer();
    
    void                UpdateBuffer(const void* data);
    MTL::Buffer*        GetBufferObject();
    void                ReleaseBuffer();
    
    
private:
  MTL::Buffer *m_buffer;
  RendererMetal *g_renderer;
  size_t m_size;
  BufferType m_type;
};

#endif
