//
//  RendererMetal.hpp
//  Albus
//
//  Created by Vignesh Mohan on 2/24/24.
//
#pragma once
#ifndef RendererMetal_hpp
#define RendererMetal_hpp
#include "MetalBuffer.hpp"

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
        
        //---------------------STARTUP AND MAIN----------------
        void Startup();
        void Shutdown();
        void Draw(MTK::View* view);
        void SetClearScreenColor(const Rgba8& color);
    
        //----------------------SHADERS-------------------------
        void BuildShader(const std::string& shaderFileName);
    
    
        //----------------------BUFFERS---------------------
        void BuildBasicBuffer();
    
        MTK::View* GetView();
        RendererConfig m_config;
    public:
        MTL::Device*              m_device;
    
    private:
        MTL::CommandQueue*        m_commandQueue;
        MTK::View*                m_view;
        MetalViewDelegate*        m_viewDelegate = nullptr;
        MTL::RenderPipelineState* m_PSO = nullptr;
    
        //--------------------BUFFERS------------------------
        MetalBuffer               m_VPositionsBuffer = {};
        MetalBuffer               m_VColorBuffer = {};
    
        //--------------------OTHER DATA--------------------
        Rgba8                     m_clearScreenColor = {} ;
};



#endif /* RendererMetal_hpp */

