#pragma once
#include "Engine/EngineData.hpp"
#include "Engine/Renderer/RenderResources/RenderBuffers.hpp"
#include "Engine/Renderer/RendererD12.hpp"

class ResourceManager
{
	//------------- Constructor/Destructor -----------
public:
	ResourceManager();
	~ResourceManager();

	//------------- resource creation -----------
public:
	void CreateVertexBuffer();
	void CreateIndexBuffer();

	//------------- member variables -----------
public:
	VertexBufferHandle<VertexNormalTangentArray>		m_mainVertexBuffer;
	IndexBufferHandle									m_mainIndexBuffer;
};