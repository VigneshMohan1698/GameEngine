
#include "Texture.hpp"
#include <d3d11.h>
#include <Engine/Renderer/Renderer.hpp>
#include "TextureView.hpp"


TextureView::TextureView()
{
}

TextureView::~TextureView()
{
	DX_SAFE_RELEASE(m_sourceHandle);
	DX_SAFE_RELEASE(m_handle);
}

