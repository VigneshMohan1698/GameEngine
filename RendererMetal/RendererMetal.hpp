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
    
        void BuildBasicShader();
        void BuildBasicBuffer();
        MTK::View* GetView();
        RendererConfig m_config;
    private:
        MTL::Device*              m_device;
        MTL::CommandQueue*        m_commandQueue;
        MTK::View*                m_view;
        MetalViewDelegate*        m_viewDelegate = nullptr;
        MTL::RenderPipelineState* m_PSO = nullptr;
    
        //--------------------BUFFERS------------------------
        MTL::Buffer*              m_vertexPositions;
        MTL::Buffer*              m_vertexColor;
    
        //--------------------OTHER DATA--------------------
        Rgba8                     m_clearScreenColor = {} ;
};



#endif /* RendererMetal_hpp */

