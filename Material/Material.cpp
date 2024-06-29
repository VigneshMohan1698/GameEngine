#include "Material.hpp"
#include <Engine\Renderer\RendererD12.hpp>
#include "MaterialIncludes.hpp"
#include <Engine\Renderer\ShaderD12.hpp>

extern RendererD12* g_theRenderer;

Material::Material(const MaterialType& type)
{
	m_type = type;

	if (type == MaterialType::Basic2D)
	{
		m_shader = g_theRenderer->CreateOrGetShader("Default", Defaul2DShaderFilePath);
	} 
	else if (type == MaterialType::Basic3D)
	{
		m_shader = g_theRenderer->CreateOrGetShader("Default3D", Defaul3DShaderFilePath);
	}
	else if (type == MaterialType::PBR3D)
	{
		m_shader = g_theRenderer->CreateOrGetShader("PBR3D", DefaulPBRShaderFilePath);
	}
}

Material::Material(ShaderD12* customShader)
{
	m_shader = customShader;
	m_type = MaterialType::Custom;
}

Material::~Material()
{
	m_shader = nullptr;
}
