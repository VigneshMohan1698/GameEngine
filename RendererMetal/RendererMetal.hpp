//
//  RendererMetal.hpp
//  Albus
//
//  Created by Vignesh Mohan on 2/24/24.
//
#pragma once
#ifndef RendererMetal_hpp
#define RendererMetal_hpp

class RendererMetal;

struct RendererConfig
{
    CGRect renderFrameDimensions;
    
};

class MetalViewDelegate : public MTK::ViewDelegate
{
    public:
        MetalViewDelegate(RendererMetal* renderer);
        virtual ~MetalViewDelegate() override;
        virtual void drawInMTKView( MTK::View* pView ) override;
    private:
        RendererMetal* m_rendererMetal = nullptr;
};

class RendererMetal
{

    public:
        RendererMetal(const RendererConfig& config);
        ~RendererMetal();
    
        void Startup();
        void Shutdown();
        void Draw(MTK::View* view);
        void SetClearScreenColor(const Rgba8& color);
        MTK::View* GetView();
        RendererConfig m_config;
    private:
        MTL::Device*       m_device;
        MTL::CommandQueue* m_commandQueue;
        MTK::View*         m_view;
        Rgba8              m_clearScreenColor = {} ;
        MetalViewDelegate* m_viewDelegate = nullptr;
};



#endif /* RendererMetal_hpp */

