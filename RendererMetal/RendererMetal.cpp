
//  RendererMetal.cpp
//  Albus
//
//  Created by Vignesh Mohan on 2/24/24.

#include "RendererMetal.hpp"
#include "../Core/EngineIncludeHeaders.hpp"
#include "../Core/Rgba8.hpp"
#include "Core/FileUtils.hpp"
#include "fstream"
#include "iostream"
#include "sstream"
#include <cassert>
#include <simd/simd.h>


//---------------------------MAIN FUNCTIONS---------------------------------------

RendererMetal::RendererMetal(const RendererConfig& config, WindowMac* windowToRender)
{
    m_config = config;
    m_window = windowToRender;
}

void RendererMetal::Startup()
{
    m_device = MTL::CreateSystemDefaultDevice();
    m_commandQueue= m_device->newCommandQueue();
    m_view = MTK::View::alloc()->init( m_config.renderFrameDimensions, m_device );
    m_view->setColorPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB );
    SetClearScreenColor(Rgba8(1,0,0,1));
    
    m_viewDelegate = new MetalViewDelegate(this);
    m_view->setDelegate(m_viewDelegate);
}

void RendererMetal::Draw(MTK::View* view)
{
    m_window->Update();
    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer* m_commandBuffer = m_commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* pRpd = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* pEnc = m_commandBuffer->renderCommandEncoder( pRpd );
    
    pEnc->setRenderPipelineState(m_PSO);
    pEnc->setVertexBuffer(m_argumentBuffer.GetBufferObject(), 0, 0);
    pEnc->useResource(m_VPositionsBuffer.GetBufferObject(),
                      MTL::ResourceUsageRead);
    pEnc->useResource(m_VColorBuffer.GetBufferObject(), MTL::ResourceUsageRead);
    pEnc->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));
    pEnc->endEncoding();
    
    m_commandBuffer->presentDrawable( view->currentDrawable() );
    m_commandBuffer->commit();
    pPool->release();
}

void RendererMetal::Shutdown()
{
    m_commandQueue->release();
    m_device->release();
    m_view->release();
    m_VPositionsBuffer.ReleaseBuffer();
    m_VColorBuffer.ReleaseBuffer();
    m_argumentBuffer.ReleaseBuffer();
    m_shaderLibrary->release();
}

//---------------------------NOT SO MAIN FUNCTIONS---------------------------------------

void RendererMetal::SetClearScreenColor(const Rgba8& color)
{
    m_clearScreenColor = color;
    m_view->setClearColor( MTL::ClearColor::Make( color.r, color.g,color.b,color.a ) );
}

MTK::View* RendererMetal::GetView()
{
    return m_view;
}

//---------------------------MAIN FUNCTIONS---------------------------------------

MetalViewDelegate::MetalViewDelegate( RendererMetal* renderer ) : MTK::ViewDelegate()
{
    m_rendererMetal = renderer;
}

void MetalViewDelegate::drawInMTKView(MTK::View* view )
{
    m_rendererMetal->Draw(view);
}

MetalViewDelegate::~MetalViewDelegate()
{
}

void RendererMetal::BuildShader(const std::string& shaderFileName)
{
    using NS::StringEncoding::UTF8StringEncoding;
    
    std::string shaderFile = ReadMetalFile(shaderFileName);
    const char* shaderSrc = shaderFile.c_str();
    
    NS::Error* pError = nullptr;
    m_shaderLibrary = m_device->newLibrary(
        NS::String::string(shaderSrc, UTF8StringEncoding), nullptr, &pError);
    if (!m_shaderLibrary) {
      __builtin_printf("%s", pError->localizedDescription()->utf8String());
      assert(false);
    }

    MTL::Function *vertexFunction = m_shaderLibrary->newFunction(
        NS::String::string("vertexMain", UTF8StringEncoding));
    MTL::Function *fragmentFunction = m_shaderLibrary->newFunction(
        NS::String::string("fragmentMain", UTF8StringEncoding));

    if(!vertexFunction) 
    {
        std::cout << "Error building vertex function\n";
    }
    if(!fragmentFunction)
    {
        std::cout << "Error building fragment function\n";
    }
    
    MTL::RenderPipelineDescriptor* psoDesc = MTL::RenderPipelineDescriptor::alloc()->init();
    psoDesc->setVertexFunction( vertexFunction );
    psoDesc->setFragmentFunction( fragmentFunction );
    psoDesc->colorAttachments()->object(0)->setPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB );

    m_PSO = m_device->newRenderPipelineState( psoDesc, &pError );
    if ( !m_PSO )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert( false );
    }

    vertexFunction->release();
    fragmentFunction->release();
    psoDesc->release();
}

void RendererMetal::BuildBasicBuffer()
{
    const size_t NumVertices = 3;

    simd::float3 positions[NumVertices] =
    {
        { -0.8f,  -0.8f, 0.0f },
        {  0.0f, 0.8f, 0.0f },
        { +0.8f,  -0.8f, 0.0f }
    };

    simd::float3 colors[NumVertices] =
    {
        {  1.0, 0.3f, 0.2f },
        {  0.8f, 1.0, 0.0f },
        {  0.8f, 0.0f, 1.0 }
    };

    const size_t positionsDataSize = NumVertices * sizeof( simd::float3 );
    const size_t colorDataSize = NumVertices * sizeof( simd::float3 );

    m_VPositionsBuffer = MetalBuffer(this, positionsDataSize, BufferType::VertexBuffer);
    m_VColorBuffer     = MetalBuffer(this, colorDataSize, BufferType::VertexBuffer);

    assert(m_shaderLibrary);

    MTL::Function *vertexFunction = m_shaderLibrary->newFunction(
        NS::String::string("vertexMain", NS::UTF8StringEncoding));
    MTL::ArgumentEncoder *argEncoder = vertexFunction->newArgumentEncoder(0);

    m_argumentBuffer = MetalBuffer(this, argEncoder->encodedLength(),
                                   BufferType::ArgumentBuffer);
    argEncoder->setArgumentBuffer(m_argumentBuffer.GetBufferObject(), 0);

    m_VPositionsBuffer.UpdateBuffer(positions);
    m_VColorBuffer.UpdateBuffer(colors);

    argEncoder->setBuffer(m_VPositionsBuffer.GetBufferObject(), 0, 0);
    argEncoder->setBuffer(m_VColorBuffer.GetBufferObject(), 0, 1);
    m_argumentBuffer.GetBufferObject()->didModifyRange(
        NS::Range::Make(0, m_argumentBuffer.GetBufferObject()->length()));

    vertexFunction->release();
    argEncoder->release();
}
