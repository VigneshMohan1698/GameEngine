#include "Engine/Math/IntVec2.hpp"
#include <string>
#include <vector>

#pragma once
class TextureView;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct TextureViewInfo;

enum eTextureBindFlagBit
{
	TEXTURE_BIND_NONE = 0,
	TEXTURE_BIND_SHADER_RESOURCE_BIT = (1 << 0),
	TEXTURE_BIND_RENDER_TARGET_BIT = (1 << 1),
	TEXTURE_BIND_DEPTH_STENCIL_BIT = (1 << 2)
};
typedef unsigned int eTextureBindFlags;
enum class eTextureFormat :int
{
	R8G8B8A8_UNORM,
	D24_UNORM_S8_UINT,
	R24G8_TYPELESS,

	//R24G8_TYPELESS 
	//R24_UNORM_X8
};

enum class eMemoryHint : int
{
	STATIC,
	GPU,
	DYNAMIC,
	STAGING
};

struct TextureCreateInfo
{
	std::string name;
	IntVec2 dimensions;
	void const* initialData = nullptr;
	size_t intitialDataBySize = 0;
	eTextureFormat format;
	eTextureBindFlags bindFlags;
	eMemoryHint memory = eMemoryHint::STATIC;
	ID3D11Texture2D* handle = nullptr;
};

class Texture
{
	friend class Renderer; // Only the Renderer can create new Texture objects!
	friend class RendererD12; // Only the Renderer can create new Texture objects!

private:
	Texture() = default; // can't instantiate directly; must ask Renderer to do it for you
	Texture(Texture const& copy) = delete; // No copying allowed!  This represents GPU memory.
	~Texture();

public:
	IntVec2				GetDimensions() const { return m_dimensions; }
	std::string const&  GetImageFilePath() const { return m_name; }
	TextureView*		GetShaderResourceView();
	TextureView*		GetDepthStencilView();
	TextureView*		GetRenderTargetView();
	TextureView*		FindOrCreateView(TextureViewInfo& vi);
protected:
	std::string							m_name;
	eTextureFormat						m_textureFormat;
	IntVec2								m_dimensions;
	std::vector<TextureView*>			m_views;
	eTextureBindFlags				    m_allowedBinds = TEXTURE_BIND_NONE;
	// #ToDo: generalize/replace this for D3D11 support!
	//unsigned int				m_openglTextureID = 0xFFFFFFFF;
	/*unsigned int				m_textureID = 0xFFFFFFFF;*/
	Renderer*							m_renderer = nullptr;
	ID3D11Texture2D*					m_texture = nullptr;

};

