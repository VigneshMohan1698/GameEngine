
//  RendererMetal.cpp
//  Albus
//
//  Created by Vignesh Mohan on 2/24/24.


#include "../Core/EngineIncludeHeaders.hpp"
#include "RendererMetal.hpp"
#include "../Core/Rgba8.hpp"
#include <simd/simd.h>
#include "Core/FileUtils.hpp"
#include "fstream"
#include "sstream"
#include "iostream"

//---------------------------MAIN FUNCTIONS---------------------------------------

RendererMetal::RendererMetal(const RendererConfig& config)
{
    m_config = config;
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
    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer* m_commandBuffer = m_commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* pRpd = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* pEnc = m_commandBuffer->renderCommandEncoder( pRpd );
    
    pEnc->setRenderPipelineState(m_PSO);
    pEnc->setVertexBuffer(m_VPositionsBuffer.GetBufferObject(), 0, 0);
    pEnc->setVertexBuffer(m_VColorBuffer.GetBufferObject(), 0, 1);
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
    MTL::Library* libraryMetal = m_device->newLibrary( NS::String::string(shaderSrc, UTF8StringEncoding), nullptr, &pError );
    if ( !libraryMetal )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert( false );
    }
    
    MTL::Function* vertexFunction = libraryMetal->newFunction( NS::String::string("vertexMain", UTF8StringEncoding) );
    MTL::Function* fragmentFunction = libraryMetal->newFunction( NS::String::string("fragmentMain", UTF8StringEncoding) );

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
    libraryMetal->release();
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
    
    m_VPositionsBuffer.UpdateBuffer(positions);
    m_VColorBuffer.UpdateBuffer(colors);
}
