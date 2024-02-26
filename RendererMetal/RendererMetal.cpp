//
//  RendererMetal.cpp
//  Albus
//
//  Created by Vignesh Mohan on 2/24/24.
//

#include "../Core/EngineIncludeHeaders.hpp"
#include "RendererMetal.hpp"

//---------------------------MAIN FUNCTIONS---------------------------------------

RendererMetal::RendererMetal(const RendererConfig& config)
{
    m_config = config;
}

void RendererMetal::Startup()
{
    m_device = MTL::CreateSystemDefaultDevice();
    
    m_view = MTK::View::alloc()->init( m_config.renderFrameDimensions, m_device );
    m_view->setColorPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB );
    //SetClearScreenColor(Rgba8(1,0,0,1));
    
    m_viewDelegate = new MetalViewDelegate(this);
    m_view->setDelegate(m_viewDelegate);
}

void RendererMetal::Draw(MTK::View* view)
{
    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer* m_commandBuffer = m_commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* pRpd = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* pEnc = m_commandBuffer->renderCommandEncoder( pRpd );
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
