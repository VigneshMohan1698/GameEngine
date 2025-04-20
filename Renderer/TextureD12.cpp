#include "TextureD12.hpp"
TextureD12::~TextureD12()
{
	m_textureResource.Reset();
	m_uploadResource.Reset();
}
