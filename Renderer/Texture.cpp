
#include "Texture.hpp"
#include <d3d11.h>
#include "Engine/Renderer/Renderer.hpp"

#include "Engine/Renderer/TextureView.hpp"
#include <Engine/Core/ErrorWarningAssert.hpp>
Texture::~Texture()
{
	if (m_texture)
	{
		DX_SAFE_RELEASE(m_texture);
	}

	for (int i = 0; i < m_views.size(); i++)
	{
		delete m_views[i];
	}

	m_views.clear();
}

TextureView* Texture::GetShaderResourceView()
{
	TextureViewInfo info;
	info.type = TEXTURE_BIND_SHADER_RESOURCE_BIT;
	info.texFormat = m_textureFormat;
	return FindOrCreateView(info);
}

TextureView* Texture::GetDepthStencilView()
{
	TextureViewInfo info;
	info.type = TEXTURE_BIND_DEPTH_STENCIL_BIT;
	info.texFormat = eTextureFormat::R24G8_TYPELESS;
	return FindOrCreateView(info);
}

TextureView* Texture::GetRenderTargetView()
{
	TextureViewInfo info;
	info.type = TEXTURE_BIND_RENDER_TARGET_BIT;
	return FindOrCreateView(info);
}

TextureView* Texture::FindOrCreateView(TextureViewInfo& info)
{
	for (int i = 0; i < m_views.size(); i++)
	{
		if (m_views[i]->m_info == info)
			return m_views[i];
	}

	TextureView* view = new TextureView();
	view->m_handle = nullptr;
	view->m_source = this;

	m_texture->AddRef();
	view->m_sourceHandle = m_texture;
	view->m_info = info;

	ID3D11Device* device = m_renderer->m_device;

	switch (info.type)
	{
	case TEXTURE_BIND_SHADER_RESOURCE_BIT:
	{
		//FOR DEPTH BUFFER TO SRV
		if (info.texFormat == eTextureFormat::R24G8_TYPELESS)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = 1;
			device->CreateShaderResourceView(m_texture, &srvDesc, &(view->m_srv));
			break;
		}
		//REGULAR SRV
		else
		{
			/*srvDesc.Format = */
			device->CreateShaderResourceView(m_texture, nullptr, &(view->m_srv));
			break;
		}

	}
	case TEXTURE_BIND_RENDER_TARGET_BIT:
	{
		device->CreateRenderTargetView(m_texture, nullptr, &(view->m_rtv));
		break;
	}
	case TEXTURE_BIND_DEPTH_STENCIL_BIT:
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		//desc.Format = Renderer::LocalToD3D11(eTextureFormat);
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Flags = 0;
		desc.Texture2D.MipSlice = 0;
		device->CreateDepthStencilView(m_texture, &desc, &(view->m_dsv));
		break;
	}
	default:
	{
		ERROR_AND_DIE("Unhandled view type");
	}
	}

	m_views.push_back(view);
	return view;
}
