#include "Engine/Math/IntVec2.hpp"
#include "Engine/Renderer/Texture.hpp"

class Renderer;
class Texture;
struct ID3D11Texture2D;
struct ID3D11View;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
#include <string>
#pragma once

struct TextureViewInfo
{
	eTextureBindFlagBit type;
	eTextureFormat texFormat;
	bool operator == (TextureViewInfo& viewInfo) 
	{
		return type == viewInfo.type;
	}
};
class TextureView
{
	friend Texture;
	friend Renderer;
private:
	TextureView();
	~TextureView();
	TextureView (TextureView const& ) = delete;
public:

public:
	Texture* m_source = nullptr;
	TextureViewInfo m_info;
	ID3D11Texture2D* m_sourceHandle = nullptr;
	union
	{
		ID3D11View* m_handle;
		ID3D11ShaderResourceView* m_srv;
		ID3D11RenderTargetView* m_rtv;
		ID3D11DepthStencilView* m_dsv;
	};
};