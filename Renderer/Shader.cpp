#include <d3d11.h>
#include <d3d12.h>

#include "Engine/Renderer/Shader.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/RendererD12.hpp"
#include <dxcapi.h>

extern RendererD12* g_theRenderer;
Shader::Shader(const ShaderConfig& config)
{
	m_config = config;

}

Shader::~Shader()
{
	DX_SAFE_RELEASE(m_pixelShader);
	DX_SAFE_RELEASE(m_vertexShader);
	DX_SAFE_RELEASE(m_inputLayoutForVertex_PCU);
	DX_SAFE_RELEASE(m_inputLayoutForVertex_PNCU);
}

const std::string& Shader::GetName() const
{
	return m_config.m_name;
}

