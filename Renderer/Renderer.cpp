
#define WIN32_LEAN_AND_MEAN	
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Image.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/EngineBuildPreferences.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Renderer/VertexBuffer.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Renderer/ConstantBuffer.hpp"
#include "Engine/Renderer/IndexBuffer.hpp"
#include "Engine/Renderer/DefaultShader.hpp"
#include "Engine/Core/Clock.hpp"

#ifdef ENGINE_DEBUG_RENDER
#include <dxgidebug.h>
#pragma comment( lib, "dxguid.lib" )
#endif
#include <Engine/Renderer/DebugRenderer.hpp>
#include <Engine/Renderer/TextureView.hpp>
#include "Engine/Renderer/Texture.hpp"

#pragma comment( lib, "d3d11" )	
#pragma comment( lib, "dxgi" )	
#pragma comment( lib, "d3dcompiler" )	
#pragma comment( lib, "dxguid.lib")


HWND windowhandle;
Renderer::Renderer()
{

}
Renderer::Renderer(RendererConfig const& rendererConfig)
{
	m_redererConfig = rendererConfig;
}

void Renderer::Startup()
{
	//----------------------------OPEN GL------------------------------------------------
	//CreateRendererContext();
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
	//----------------------------DIRECTX---------------------------------------
	ReportingSetup();
	CreateDeviceSwapChain();
	CreateBackBuffer();
	CreateViewPort();
	CreateOrGetShader(defaultShaderName);
	m_immediateVBO = new VertexBuffer(sizeof(Vertex_PCU));
	m_immediateVBO_PNCU = new VertexBuffer(sizeof(Vertex_PNCU), sizeof(Vertex_PNCU));
	CreateDepthStencilTextureAndView();
	BindShaderByName(defaultShaderName);
	CreateVertexBuffer(sizeof(Vertex_PCU));
	m_cameraCBO = new ConstantBuffer(sizeof(CameraConstants));
	m_modalCBO = new ConstantBuffer(sizeof(ModelConstants));
	m_lightCBO = new ConstantBuffer(sizeof(LightConstants));
	m_pointLightCBO = new ConstantBuffer(sizeof(PointLights));
	m_gameConstantCBO = new ConstantBuffer(sizeof(MinecraftGameConstants));
	m_fireNoiseCBO = new ConstantBuffer(sizeof(NoiseConstants));
	m_fireDistortionCBO = new ConstantBuffer(sizeof(DistortionConstants));

 	CreateConstantBuffer(sizeof(CameraConstants), m_cameraCBO);
	CreateConstantBuffer(sizeof(ModelConstants), m_modalCBO);
	CreateConstantBuffer(sizeof(LightConstants), m_lightCBO);
	CreateConstantBuffer(sizeof(PointLights), m_pointLightCBO);
	CreateConstantBuffer(sizeof(MinecraftGameConstants), m_gameConstantCBO);
	CreateConstantBuffer(sizeof(NoiseConstants), m_fireNoiseCBO);
	CreateConstantBuffer(sizeof(DistortionConstants), m_fireDistortionCBO);

	m_immediateIBO = new IndexBuffer(sizeof(unsigned int));
	CreateIndexBuffer(sizeof(unsigned int), m_immediateIBO);
	SetBlendMode(BlendMode::ALPHA);
	Image* image = new Image(IntVec2(1, 1), Rgba8(255, 255, 255, 255));
	m_defaultTexture = CreateTextureFromImage(*image);
	//BindTexture(m_defaultTexture);
	SetDepthStencilState(DepthTest::ALWAYS, false);
	SetRasterizerState(CullMode::NONE, FillMode::SOLID, WindingOrder::COUNTERCLOCKWISE);
	SetSamplerMode(SamplerMode::POINTCLAMP);
	//BindSampler();

	//------------------------------DEBUG RENDERER STARTUP--------------------------------------

	DebugRenderConfig debugRenderConfig;
	debugRenderConfig.m_renderer = this;
	debugRenderConfig.m_startHidden = false;
	BitmapFont* font = CreateOrGetBitmapFont("Data/Images/SquirrelFixedFont");
	debugRenderConfig.m_bitmapFont = font;
	DebugRenderStartup(debugRenderConfig);

}
void Renderer::BeginFrame()
{
	DebugRenderBeginFrame();
}
void Renderer::RenderFrame() const
{
	
}
void Renderer::EndFrame()
{
	/*HDC displayDeviceContext = ::GetDC(windowhandle);
	::SwapBuffers(displayDeviceContext);*/
 	CopyTexture(m_activeColorTarget[m_activeColorTargetIndex], m_backBuffer);
	m_swapChain->Present(0, 0);

	BindTexture(nullptr);
	DebugRenderEndFrame();
}
void Renderer::ReleaseConstantBuffer(ConstantBuffer* buffer)
{
	DX_SAFE_RELEASE(buffer->m_buffer);
}
void Renderer::ReleaseIndexBuffer(IndexBuffer* buffer)
{
	DX_SAFE_RELEASE(buffer->m_buffer);
}
void Renderer::ReleaseVertexBuffer(VertexBuffer* buffer)
{
	DX_SAFE_RELEASE(buffer->m_buffer);
}
void Renderer::ReleaseSampler(ID3D11SamplerState* sampler)
{
	DX_SAFE_RELEASE(sampler);
}
RendererConfig	Renderer::GetRenderConfig()
{
	return m_redererConfig;
}
void Renderer::DestroyTexture(Texture* texture)
{
	if (texture != nullptr)
	{
		delete texture;
		texture = nullptr;
	}
}
Texture* Renderer::GetCurrentColorTarget()
{
	if (m_currentCamera == nullptr)
	{
		return m_activeColorTarget[m_activeColorTargetIndex];
	}
	Texture* color = m_currentCamera->GetColorTarget();
	return (color == nullptr ) ?  m_activeColorTarget[m_activeColorTargetIndex] : color;
}
Texture* Renderer::GetCurrentDepthTarget()
{
	if (m_currentCamera == nullptr)
	{
		return m_depthBuffer;
	}
	Texture* color = m_currentCamera->GetDepthTarget();
	return (color == nullptr) ? m_depthBuffer : color;
}
void Renderer::ShutDown()
{
	DebugRenderShutDown();
	DX_SAFE_RELEASE(m_blendState);
	DX_SAFE_RELEASE(m_swapChain);
	DX_SAFE_RELEASE(m_deviceContext);
	DX_SAFE_RELEASE(m_rasterizerState);
	DX_SAFE_RELEASE(m_cameraCBO->m_buffer);
	DX_SAFE_RELEASE(m_modalCBO->m_buffer);
	DX_SAFE_RELEASE(m_lightCBO->m_buffer);
	DX_SAFE_RELEASE(m_pointLightCBO->m_buffer);
	DX_SAFE_RELEASE(m_gameConstantCBO->m_buffer);
	if (m_fireDistortionCBO)
	{
		DX_SAFE_RELEASE(m_fireDistortionCBO->m_buffer);
	}
	if (m_fireNoiseCBO)
	{
		DX_SAFE_RELEASE(m_fireNoiseCBO->m_buffer);
	}
	if (m_raytracingWorldObjectsCBO)
	{
		DX_SAFE_RELEASE(m_raytracingWorldObjectsCBO->m_buffer);
	}
	if (m_worldPlanesCBO)
	{
		DX_SAFE_RELEASE(m_worldPlanesCBO->m_buffer);
	}
	if (m_worldSpheresCBO)
	{
		DX_SAFE_RELEASE(m_worldSpheresCBO->m_buffer);
	}
	if (m_worldCubesCBO)
	{
		DX_SAFE_RELEASE(m_worldCubesCBO->m_buffer);
	}
	DestroyTexture(m_backBuffer);
	DestroyTexture(GetCurrentDepthTarget());
	DestroyTexture(m_defaultTexture);
	DestroyTexture(m_activeColorTarget[0]);
	DestroyTexture(m_activeColorTarget[1]);
	//DestroyTexture(m_backupColorTarget);
	m_depthBuffer = nullptr;
	m_activeColorTarget[m_activeColorTargetIndex] = nullptr;
	m_backBuffer = nullptr;
	DX_SAFE_RELEASE(m_immediateIBO->m_buffer);
	DX_SAFE_RELEASE(m_immediateVBO->m_buffer);
	DX_SAFE_RELEASE(m_immediateVBO_PNCU->m_buffer);
	DX_SAFE_RELEASE(m_samplerState);
	DX_SAFE_RELEASE(m_samplerState2);
	DX_SAFE_RELEASE(m_depthStencilState);
	DX_SAFE_RELEASE(m_depthStencilTexture);
	DX_SAFE_RELEASE(m_device);
	m_cameraCBO = nullptr;
	for (int i = 0; i < m_loadedShaders.size(); i++)
	{
		if (m_loadedShaders[i] != nullptr)
		{
			delete m_loadedShaders[i];
			m_loadedShaders[i] = nullptr;
		}
	}
	for (int i = 0; i < m_loadedMeshes.size(); i++)
	{
		if (m_loadedMeshes[i] != nullptr)
		{
			delete m_loadedMeshes[i];
			m_loadedMeshes[i] = nullptr;
		}
	}

	for (int i = 0; i < m_loadedTextures.size(); i++)
	{
		if (m_loadedTextures[i] != nullptr)
		{
			delete m_loadedTextures[i];
			m_loadedTextures[i] = nullptr;
		}
	}
	delete m_immediateVBO;
	m_immediateVBO = nullptr;
	delete m_immediateVBO_PNCU;
	m_immediateVBO_PNCU = nullptr;
	delete m_immediateIBO;
	m_immediateIBO = nullptr;
	delete m_lightCBO;
	m_lightCBO = nullptr;
	delete m_pointLightCBO;
	m_pointLightCBO = nullptr;
	delete m_gameConstantCBO;
	m_gameConstantCBO = nullptr;
	delete m_fireNoiseCBO;
	m_fireNoiseCBO = nullptr;
	delete m_fireDistortionCBO;
	m_fireDistortionCBO = nullptr;

	delete m_raytracingWorldObjectsCBO;
	m_raytracingWorldObjectsCBO = nullptr;
	/*ReportLiveObjects();*/

#ifdef ENGINE_DEBUG_RENDER
	((IDXGIDebug*)m_dxgiDebug)->ReportLiveObjects(DXGI_DEBUG_ALL, (DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
	((IDXGIDebug*)m_dxgiDebug)->Release();
	m_dxgiDebug = nullptr;
	::FreeLibrary((HMODULE)m_dxgiDebugModule);
	m_dxgiDebugModule = nullptr;
#endif 
}

//------------------------------DIRECTX BASE FUNCTIONS FUNCTIONS ------------------------
void Renderer::CreateRendererContext()
{
/*	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	memset(&pixelFormatDescriptor, 0, sizeof(pixelFormatDescriptor));
	pixelFormatDescriptor.nSize = sizeof(pixelFormatDescriptor);
	pixelFormatDescriptor.nVersion = 1;
	pixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDescriptor.cColorBits = 24;
	pixelFormatDescriptor.cDepthBits = 24;
	pixelFormatDescriptor.cAccumBits = 0;
	pixelFormatDescriptor.cStencilBits = 8;

		windowhandle = ::GetActiveWindow();
	HDC displayDeviceContext = ::GetDC(windowhandle);
	int pixelFormatCode = ChoosePixelFormat(displayDeviceContext, &pixelFormatDescriptor);
	SetPixelFormat(displayDeviceContext, pixelFormatCode, &pixelFormatDescriptor);
	HGLRC g_openGLRenderingContext = wglCreateContext(displayDeviceContext);
	wglMakeCurrent(displayDeviceContext, g_openGLRenderingContext);*/
}
void Renderer::CreateDeviceSwapChain()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferDesc.Width = m_redererConfig.m_window->GetClientDimensions().x;
	swapChainDesc.BufferDesc.Height = m_redererConfig.m_window->GetClientDimensions().y;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = (HWND)m_redererConfig.m_window->GetHwnd();
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	UINT flags = 0;
#if defined( ENGINE_DEBUG_RENDER )
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
	UINT SDK_version = D3D11_SDK_VERSION;
	HRESULT result = D3D11CreateDeviceAndSwapChain(
		NULL,
		driverType,
		NULL,
		flags,
		NULL,
		0,
		SDK_version,
		&swapChainDesc,
		&m_swapChain,
		&m_device,
		0,
		&m_deviceContext
	);

	if (!SUCCEEDED(result))
	{
		ERROR_AND_DIE("Error while creating swap chain buffer");
	}

}
void Renderer::SetDebugName(ID3D11DeviceChild* object, char const* name)
{
	object->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(name), name);
}
void Renderer::DrawTriangle(Vec2 point1, Vec2 point2, Vec2 point3, Rgba8 color)
{
	UNUSED((void)point1);
	UNUSED((void)point2);
	UNUSED((void)point3);
	UNUSED((void)color);
	/*
	glBegin(GL_TRIANGLES);
	{
		glColor4ub(color.r, color.g, color.b, color.a);
		glTexCoord2f(0.f, 0.f);
		glVertex2f(point1.x, point1.y);

		glColor4ub(color.r, color.g, color.b, color.a);
		glTexCoord2f(0.f, 0.f);
		glVertex2f(point2.x, point2.y);

		glColor4ub(color.r, color.g, color.b, color.a);
		glTexCoord2f(0.f, 0.f);
		glVertex2f(point3.x, point3.y);
	}
	glEnd();*/
}
void Renderer::CreateViewPort()
{
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;
	viewport.Width = static_cast<float>(m_redererConfig.m_window->GetClientDimensions().x);
	viewport.Height = static_cast<float>(m_redererConfig.m_window->GetClientDimensions().y);
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	m_deviceContext->RSSetViewports(1, &viewport);
}
void Renderer::SetViewPortForCamera(const Camera&  camera)
{
	AABB2 cameraBounds = camera.GetCameraBounds();
	AABB2 cameraViewPortBounds = camera.m_viewport;
	float windowDimensionsX = static_cast<float>(m_redererConfig.m_window->GetClientDimensions().x);
	float windowDimensionsY = static_cast<float>(m_redererConfig.m_window->GetClientDimensions().y);
	/*float topleftX = RangeMap()*/
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = windowDimensionsX * cameraViewPortBounds.m_mins.x;
	viewport.TopLeftY = windowDimensionsY * cameraViewPortBounds.m_mins.y;
	viewport.Width = windowDimensionsX * cameraViewPortBounds.m_maxs.x;
	viewport.Height = windowDimensionsY * cameraViewPortBounds.m_maxs.y;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	m_deviceContext->RSSetViewports(1, &viewport);
}
void Renderer::ReportLiveObjects()
{
#ifdef  ENGINE_DEBUG_RENDER
	ID3D11Debug* debugDevice = nullptr;

	HRESULT result = m_device->QueryInterface(__uuidof(ID3D11Debug), (void**)&debugDevice);
	result = debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	DX_SAFE_RELEASE(debugDevice);
#endif //  ENGINE_DEBUG_RENDER

}

//------------------------------SHADER FUNCTIONS AND INUPUT LAYOUT------------------------
Shader* Renderer::CreateOrGetShader(const char* shaderName)
{
	for (int i = 0; i < m_loadedShaders.size(); i++)
	{
		if (m_loadedShaders[i]->GetName() == shaderName)
		{
			return m_loadedShaders[i];
		}
	}
	
	if (shaderName == defaultShaderName)
	{

		m_defaultShader = CreateShader(shaderName, shaderSources);
		return m_defaultShader;
	}
	Shader* shader = CreateShaderFromFile(shaderName);
	return shader;
}
void Renderer::BindShaderByName(const char* shaderName)
{
	for (int i = 0; i < m_loadedShaders.size(); i++)
	{
		if (m_loadedShaders[i]->GetName() == shaderName)
		{
			BindShader(m_loadedShaders[i]);
		}
	} 
	return;
}
void Renderer::BindShader(Shader* shader)
{
	if (shader == nullptr)
	{
		m_currentShader = m_defaultShader;
		m_deviceContext->VSSetShader(m_defaultShader->m_vertexShader, NULL, NULL);
		m_deviceContext->PSSetShader(m_defaultShader->m_pixelShader, NULL, NULL);
		return;
	}
	m_currentShader = shader;
	m_deviceContext->VSSetShader(shader->m_vertexShader, NULL, NULL);
	m_deviceContext->PSSetShader(shader->m_pixelShader, NULL, NULL);
}
Shader* Renderer::GetShaderForName(const char* shaderName)
{
	for (int i = 0; i < m_loadedShaders.size(); i++)
	{
		if (m_loadedShaders[i]->GetName() == shaderName)
		{
			return m_loadedShaders[i];
		}
	}
	return nullptr;
}
Shader* Renderer::CreateShader(char const* shaderName, char const* shaderSrc)
{
	std::vector<unsigned char> vertexByte = {};
	std::vector<unsigned char> pixelByte = {};

	ShaderConfig shaderConfig;
	shaderConfig.m_name = shaderName;
	Shader* shader = new Shader(shaderConfig);
	CompileShaderToByteCode(vertexByte, shaderName, shaderSrc, "VertexMain", "vs_5_0");
	CreateVertexShader(shader, shaderSources, vertexByte);
	CreateInputLayout(shader, vertexByte);
	CompileShaderToByteCode(pixelByte, shaderName, shaderSrc, "PixelMain", "ps_5_0");
	CreatePixelShader(shader, shaderSrc, pixelByte);
	m_loadedShaders.push_back(shader);
	return shader;
}
bool Renderer::CompileShaderToByteCode(std::vector<unsigned char>& outByteCode, char const* name, char const* source, char const* entryPoint, char const* target)
{
	UNUSED((void)name);
	ID3DBlob* errorBlobs = nullptr;
	ID3DBlob* shaderBlobs = nullptr;
	UINT flags = NULL;
	#if defined( ENGINE_DEBUG_RENDER )
	flags |= D3DCOMPILE_DEBUG;
	flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif
	int Size = 0;
	while (source[Size] != '\0')
	{
		Size++;
	}
	HRESULT result = D3DCompile(source, Size, NULL, NULL, NULL, entryPoint, target, flags, NULL, &shaderBlobs, &errorBlobs);
	if (FAILED(result))
	{
		DebuggerPrintf(static_cast<const char*>(errorBlobs->GetBufferPointer()));
		errorBlobs->Release();
		ERROR_AND_DIE("Failed while compiling shader");
	}
	outByteCode.resize(shaderBlobs->GetBufferSize());
	std::memcpy(outByteCode.data(), shaderBlobs->GetBufferPointer(), shaderBlobs->GetBufferSize());
	if (errorBlobs != nullptr)
	{
		errorBlobs->Release();
	}
	shaderBlobs->Release();
	return true;
}
void Renderer::CreateVertexShader(Shader* shader, const char* shaderSrc, std::vector<uint8_t> vertexByte)
{
	UNUSED((void)shaderSrc);
	HRESULT result2 = m_device->CreateVertexShader(vertexByte.data(), vertexByte.size(), NULL, &shader->m_vertexShader);
	if (FAILED(result2))
	{
		ERROR_AND_DIE("Failed to create vertex shader");
	}
}
void Renderer::CreatePixelShader(Shader* shader, char const* shaderSrc, std::vector<uint8_t> pixelByte)
{
	UNUSED((void)shaderSrc);
	HRESULT result2 = m_device->CreatePixelShader(pixelByte.data(), pixelByte.size(), NULL, &shader->m_pixelShader);
	if (FAILED(result2))
	{
		ERROR_AND_DIE("Failed to create pixel shader");
	}
}

void Renderer::CreateInputLayout(Shader* shader, std::vector<unsigned char> vertexByte)
{
	if (shader->GetName().find("Lit") != std::string::npos)
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT elements = 4;
		HRESULT result = m_device->CreateInputLayout(inputElementDesc, elements, vertexByte.data(), vertexByte.size(), &shader->m_inputLayoutForVertex_PNCU);

		if (FAILED(result))
		{
			ERROR_AND_DIE("Failed to create input layout");
		}
	}
	else
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT elements = 3;
		HRESULT result = m_device->CreateInputLayout(inputElementDesc, elements, vertexByte.data(), vertexByte.size(), &shader->m_inputLayoutForVertex_PCU);

		if (FAILED(result))
		{
			ERROR_AND_DIE("Failed to create input layout");
		}
	}
}
Shader* Renderer::CreateShaderFromFile(const char* shaderName)
{
	std::string shaderNameWithExtension = std::string(shaderName) + ".hlsl";
	std::string shaderValues;
	FileReadToString(shaderValues, shaderNameWithExtension);
	Shader* shad = nullptr;
	shad = CreateShader(shaderName, shaderValues.c_str());
	return shad;
}
Shader* Renderer::GetDefaultShader()
{
	for (int i = 0; i < m_loadedShaders.size(); i++)
	{
		if (m_loadedShaders[i]->GetName() == defaultShaderName)
		{
			return m_loadedShaders[i];
		}
	}
	return nullptr;
}

MeshBuilder* Renderer::CreateOrGetMesh(const char* filePath)
{
	for (int i = 0; i < m_loadedMeshes.size(); i++)
	{
		if (m_loadedMeshes[i]->GetFilePath() == filePath)
		{
			return m_loadedMeshes[i];
		}
	}

	if (!FileExists(filePath))
	{
		ERROR_AND_DIE("Mesh File Does not exist");
	}

	MeshBuilder* mesh = CreateMesh(filePath);
	return mesh;
}
MeshBuilder* Renderer::CreateMeshFromSavedFile(const char* filePath)
{
	for (int i = 0; i < m_loadedMeshes.size(); i++)
	{
		if (m_loadedMeshes[i]->GetFilePath() == filePath)
		{
			m_loadedMeshes[i]->Load(filePath);
			return m_loadedMeshes[i];
		}
	}

	if (!FileExists(filePath))
	{
		ERROR_AND_DIE("Mesh File Does not exist");
	}

	MeshBuilder* mesh = new MeshBuilder();
	mesh->Load(filePath);
	mesh->m_filePath = filePath;
	m_loadedMeshes.push_back(mesh);
	return mesh;
}
MeshBuilder* Renderer::GetMeshByName(const char* Name)
{
	UNUSED((void) Name);
	//for (int i = 0; i < m_loadedMeshes.size(); i++)
	//{
	//	if (m_loadedMeshes[i]->m_modelName == Name)
	//	{
	//		return m_loadedMeshes[i];
	//	}
	//}
	return nullptr;
}
MeshBuilder* Renderer::GetMesh(const char* filePath)
{
	for (int i = 0; i < m_loadedMeshes.size(); i++)
	{
		if (m_loadedMeshes[i]->GetFilePath() == filePath)
		{
			return m_loadedMeshes[i];
		}
	}
	return nullptr;
}

MeshBuilder* Renderer::CreateMesh(const char* filePath)
{
	MeshBuilder* mesh = new MeshBuilder();
	MeshImportOptions options;
	mesh->ImportFromOBJFile(filePath, options);
	m_loadedMeshes.push_back(mesh);
	return mesh;
}

MeshBuilder* Renderer::CreateMesh(const char* filePath, const char* name)
{
	UNUSED((void) name);
	MeshBuilder* mesh = new MeshBuilder();
	MeshImportOptions options;
	mesh->ImportFromOBJFile(filePath, options);
	//mesh->m_modelName = name;
	m_loadedMeshes.push_back(mesh);

	return mesh;
}
MeshBuilder* Renderer::BuildMesh(const char* filePath)
{
	MeshBuilder* mesh = GetMesh(filePath);
	mesh->m_gpuMesh = new GPUMesh();
	size_t size = sizeof(Vertex_PNCU) * mesh->m_cpuMesh->m_vertices.size();
	if (size != 0)
	{
		mesh->m_gpuMesh->m_vertexBuffer = new VertexBuffer(size, sizeof(Vertex_PNCU));
		mesh->m_gpuMesh->m_vertexBuffer = CreateVertexBuffer(size, mesh->m_gpuMesh->m_vertexBuffer);
		CopyCPUToGPU(mesh->m_cpuMesh->m_vertices.data(), size, mesh->m_gpuMesh->m_vertexBuffer);

		/*size = sizeof(unsigned int) * mesh->m_cpuMesh->m_indices.size();
		mesh->m_gpuMesh->m_indexBuffer = new IndexBuffer(size);
		mesh->m_gpuMesh->m_indexBuffer = CreateIndexBuffer(size, mesh->m_gpuMesh->m_indexBuffer);
		CopyCPUToGPU(mesh->m_cpuMesh->m_indices.data(), size, mesh->m_gpuMesh->m_indexBuffer);*/
	}

	return mesh;
}
//------------------------------VERTEX BUFFER FUNCTIONS ------------------------
VertexBuffer* Renderer::CreateVertexBuffer(const size_t size)
{
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)size;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT result = m_device->CreateBuffer(
		&bufferDesc,
		NULL,
		&m_immediateVBO->m_buffer
	);
	if (FAILED(result))
	{
		ERROR_AND_DIE("Failed to create Vertex buffer");
	}
	m_immediateVBO->m_size = size;
	return m_immediateVBO;
}
VertexBuffer* Renderer::CreateVertexBuffer(const size_t size, VertexBuffer* vbo)
{
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)size;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT result = m_device->CreateBuffer(
		&bufferDesc,
		NULL,
		&vbo->m_buffer
	);
	if (FAILED(result))
	{
		//HRESULT res = m_device->GetDeviceRemovedReason();
		ERROR_AND_DIE("Failed to create Vertex buffer");
	}
	vbo->m_size = size;
	return vbo;
}
void Renderer::CopyCPUToGPU(const void* data, size_t size, VertexBuffer* vbo)
{
	if (size > vbo->m_size)
	{
		DX_SAFE_RELEASE(vbo->m_buffer);
		CreateVertexBuffer(size, vbo);
	}
	D3D11_MAPPED_SUBRESOURCE gpuResource = { 0 };
	HRESULT result2 = m_deviceContext->Map(vbo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuResource);
	if (!SUCCEEDED(result2))
	{
		ERROR_AND_DIE("Failed while copying resource to GPU");
	}
	std::memcpy(gpuResource.pData, data, size);
	m_deviceContext->Unmap(vbo->m_buffer, 0);
}
void Renderer::BindVertexBuffer(VertexBuffer* vbo)
{
	UINT stride = vbo->GetStride();
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &vbo->m_buffer, &stride, &offset);
	if (m_currentShader->GetName().find("Lit") != std::string::npos)
	{
		m_deviceContext->IASetInputLayout(m_currentShader->m_inputLayoutForVertex_PNCU);
	}
	else
	{
 		m_deviceContext->IASetInputLayout(m_currentShader->m_inputLayoutForVertex_PCU);
	}
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
void Renderer::DrawVertexBuffer(VertexBuffer* vbo, int vertexCount, int vertexOffset)
{
	BindVertexBuffer(vbo);
	m_deviceContext->Draw(vertexCount, vertexOffset);
}
void Renderer::DrawIndexedVertexBuffer(VertexBuffer* vbo,IndexBuffer* ibo, int vertexCount, int vertexOffset, int indexCount, int indexOffset)
{
	//BindVertexBuffer(vbo);
	UNUSED((void)vertexCount);
	UNUSED((void)ibo);
	UNUSED((void)vbo);
	m_deviceContext->DrawIndexed(indexCount, indexOffset, vertexOffset);
}

//------------------------------CONSTANT BUFFER FUNCTIONS ------------------------
ConstantBuffer* Renderer::CreateConstantBuffer(const size_t size, ConstantBuffer* cbo)
{
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)size;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT result = m_device->CreateBuffer(
		&bufferDesc,
		NULL,
		&cbo->m_buffer
	);
	if (FAILED(result))
	{
		ERROR_AND_DIE("Failed to create Constant buffer");
	}
	cbo->m_size = size;
	return cbo;
}
void Renderer::CopyCPUToGPU(const void* data, size_t size, ConstantBuffer* cbo)
{
	if (size > cbo->m_size)
	{
		DX_SAFE_RELEASE(cbo->m_buffer);
		CreateConstantBuffer(size, cbo);
	}
	D3D11_MAPPED_SUBRESOURCE gpuResource = { 0 };
	HRESULT result2 = m_deviceContext->Map(cbo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &gpuResource);
	if (!SUCCEEDED(result2))
	{
		ERROR_AND_DIE("Failed while copying resource to GPU");
	}
	std::memcpy(gpuResource.pData, data, size);
	m_deviceContext->Unmap(cbo->m_buffer, NULL);
}
void Renderer::BindConstantBuffer(int slot, ConstantBuffer* cbo)
{
	m_deviceContext->VSSetConstantBuffers(slot, 1, &cbo->m_buffer);
	m_deviceContext->PSSetConstantBuffers(slot, 1, &cbo->m_buffer);
}
IndexBuffer* Renderer::CreateIndexBuffer(const size_t size, IndexBuffer* ibo)
{
	// Fill in a buffer description.
	D3D11_BUFFER_DESC bufferDesc = {0};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)size;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// Define the resource data.

	// Create the buffer with the device.
	HRESULT hr = m_device->CreateBuffer(&bufferDesc,NULL, &ibo->m_buffer);
	if (FAILED(hr))
	{
		ERROR_AND_DIE("Error while creating index buffer");
	}
	return ibo;
}

//------------------------------INDEX BUFFER FUNCTIONS ------------------------
void Renderer::CopyCPUToGPU(const void* data, size_t size, IndexBuffer* ibo)
{
	if (size > ibo->m_size)
	{
		DX_SAFE_RELEASE(ibo->m_buffer);
		CreateIndexBuffer(size, ibo);
	}
	D3D11_MAPPED_SUBRESOURCE gpuResource = { 0 };
	HRESULT result2 = m_deviceContext->Map(ibo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &gpuResource);
	if (!SUCCEEDED(result2))
	{
		ERROR_AND_DIE("Failed while copying index buffer resource to GPU");
	}
	std::memcpy(gpuResource.pData, data, size);
	m_deviceContext->Unmap(ibo->m_buffer, NULL);
}
void Renderer::BindIndexBuffer(int slot, IndexBuffer* cbo)
{
	// Set the buffer.
	m_deviceContext->IASetIndexBuffer(cbo->m_buffer, DXGI_FORMAT_R32_UINT, slot);
}
	
//-----------------------------RENDERER AND CAMERA FUNCTIONS-----------------------------
void Renderer::ReportingSetup()
{
#ifdef ENGINE_DEBUG_RENDER
	m_dxgiDebugModule = (void*) ::LoadLibraryA("dxgidebug.dll");
	typedef HRESULT(WINAPI* GetDebugModuleCB)(REFIID, void**);
	((GetDebugModuleCB) ::GetProcAddress((HMODULE)m_dxgiDebugModule, "DXGIGetDebugInterface"))(__uuidof(IDXGIDebug), (void**)&m_dxgiDebug);
#endif // ENGINE_DEBUG_RENDER
}
void Renderer::CreateRenderTarget()
{
	/*ID3D11Texture2D* texture2D = nullptr;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&texture2D);

	HRESULT targetView = m_device->CreateRenderTargetView(texture2D, NULL, &m_renderTargetView);
	if (!SUCCEEDED(targetView))
	{
		ERROR_AND_DIE("Failed to create Target view");
	}
	texture2D->Release();*/
}
void Renderer::ClearScreen(const Rgba8& clearColor)
{
/*	glClearColor(clearColor.r,clearColor.g,clearColor.b,clearColor.a); 
	glClear(GL_COLOR_BUFFER_BIT);*/
	
	float clearScreenColor[4];
	clearColor.GetAsFloats(clearScreenColor);

	TextureView* rtv = GetCurrentColorTarget()->GetRenderTargetView();
	m_deviceContext->ClearRenderTargetView(rtv->m_rtv, clearScreenColor);
	//rtv = m_activeColorTarget[1]->GetRenderTargetView();
	//m_deviceContext->ClearRenderTargetView(rtv->m_rtv, clearScreenColor);
	ClearDepth();
}
void Renderer::BeginCamera(const Camera& camera)
 {
	//------------------------------OPENGL--------------------------------
/*	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();
	Vec2 cameraMinBounds = camera.GetCameraBounds().m_mins;z
	Vec2 cameraMaxBounds = camera.GetCameraBounds().m_maxs;
	glOrtho(cameraMinBounds.x, cameraMaxBounds.x, cameraMinBounds.y,
		cameraMaxBounds.y, 0.f, 1.f);*/
	//-------------------------OPENGL-----------------------------------------
	if (camera.m_cameraView == CameraView::Perspective)
	{
		SetRasterizerState(CullMode::BACK, FillMode::SOLID, WindingOrder::COUNTERCLOCKWISE);
		SetDepthStencilState(DepthTest::LESSEQUAL, true);
		SetSamplerMode(SamplerMode::BILINEARWRAP);


	}
	else if (camera.m_cameraView == CameraView::Orthographic)
	{
		SetDepthStencilState(DepthTest::ALWAYS, false);
		SetRasterizerState(CullMode::NONE, FillMode::SOLID, WindingOrder::COUNTERCLOCKWISE);
		SetSamplerMode(SamplerMode::POINTCLAMP);
		SetBlendMode(BlendMode::ALPHA);
	}
	SetViewPortForCamera(camera);

	m_currentCamera = (Camera*)&camera;
	float modelColor[4];
	Rgba8::WHITE.GetAsFloats(modelColor);
	SetModalMatrix(Mat44());
	SetModalColor(modelColor);

	TextureView* rtv = GetCurrentColorTarget()->GetRenderTargetView();
	if (GetCurrentDepthTarget() != nullptr)
	{
		TextureView* dsv = GetCurrentDepthTarget()->GetDepthStencilView();
		m_deviceContext->OMSetRenderTargets(1, &(rtv->m_rtv), dsv->m_dsv);
	}
	BindCameraConstantsToShader();
}

void Renderer::BindCameraConstantsToShader()
{
	Mat44 projectionMatrix, viewMatrix;
	projectionMatrix = m_currentCamera->GetProjectionMatrix();
	viewMatrix = m_currentCamera->GetViewMatrix();
	CameraConstants localCamera{ projectionMatrix ,viewMatrix };
	CopyCPUToGPU(&localCamera, sizeof(localCamera), m_cameraCBO);
	BindConstantBuffer(m_constantBufferSlot, m_cameraCBO);
}
void Renderer::SetModalMatrix(Mat44 const& modelMatrix)
{
	m_modelMatrix = modelMatrix;
}
Mat44 Renderer::GetModelMatrix()
{
	return m_modelMatrix;
}
void Renderer::SetRenderTarget(ID3D11RenderTargetView* view)
{
	m_deviceContext->OMSetRenderTargets(1, &view, nullptr);
}
void Renderer::SetModalColor(float modelColor[4])
{
	m_modelColor[0] = modelColor[0];
	m_modelColor[1] = modelColor[1];
	m_modelColor[2] = modelColor[2];
	m_modelColor[3] = modelColor[3];
}
void Renderer::SetRasterizerState(CullMode cullMode, FillMode fillMode, WindingOrder windingOrder)
{
	DX_SAFE_RELEASE(m_rasterizerState);
	D3D11_RASTERIZER_DESC pRasterizerDesc = {  };
	switch (fillMode)
	{
	case FillMode::SOLID:
		pRasterizerDesc.FillMode = D3D11_FILL_SOLID;
		break;
	case FillMode::WIREFRAME:
		pRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		break;
	default:
		break;
	}
	switch (cullMode)
	{
	case CullMode::NONE:
		pRasterizerDesc.CullMode = D3D11_CULL_NONE;
		break;
	case CullMode::FRONT:
		pRasterizerDesc.CullMode = D3D11_CULL_FRONT;
		break;
	case CullMode::BACK:
		pRasterizerDesc.CullMode = D3D11_CULL_BACK;
		break;
	default:
		break;
	}
	switch (windingOrder)
	{
	case WindingOrder::CLOCKWISE:
		pRasterizerDesc.FrontCounterClockwise = false;
		break;
	case WindingOrder::COUNTERCLOCKWISE:
		pRasterizerDesc.FrontCounterClockwise = true;
		break;
	default:
		break;
	}
	pRasterizerDesc.DepthClipEnable = true;
	pRasterizerDesc.AntialiasedLineEnable = true;

	m_device->CreateRasterizerState(&pRasterizerDesc, &m_rasterizerState);
	m_deviceContext->RSSetState(m_rasterizerState);
}
void Renderer::EndCamera(const Camera& camera)
{
	m_deviceContext->ClearState();
	UNUSED((void)camera);
	//Texture* cameraTexture = m_currentCamera->GetColorTarget();
	//if (cameraTexture != nullptr)
	//{
	//	CopyTexture(cameraTexture, m_activeColorTarget[m_activeColorTargetIndex]);
	//}
	//
	//m_deviceContext->ClearState();
	m_currentCamera = nullptr;
}
Camera Renderer::GetCurrentCamera()
{
	return *m_currentCamera;
}


//------------------------------DRAW FUNCTIONS------------------------------------
void Renderer::DrawVertexArray(int numVertexes, Vertex_PNCU const* vertexes)
{
	ModelConstants modelConstant;
	modelConstant.ModelMatrix = m_modelMatrix;
	modelConstant.color[0] = m_modelColor[0];
	modelConstant.color[1] = m_modelColor[1];
	modelConstant.color[2] = m_modelColor[2];
	modelConstant.color[3] = m_modelColor[3];
	CopyCPUToGPU(&modelConstant, sizeof(modelConstant), m_modalCBO);
	BindConstantBuffer(m_modalconstantBufferSlot, m_modalCBO);

	LightConstants lightConstant{};
	lightConstant.sunDirection[0] = m_sunDirection.x;
	lightConstant.sunDirection[1] = m_sunDirection.y;
	lightConstant.sunDirection[2] = m_sunDirection.z;
	lightConstant.SunIntensity = m_sunIntensity;
	lightConstant.AmbientIntensity = m_ambientIntensity;
	CopyCPUToGPU(&lightConstant, sizeof(LightConstants), m_lightCBO);
	BindConstantBuffer(m_lightConstantBufferSlot, m_lightCBO);

	if (m_currentShader->GetName().find("Lit") != std::string::npos)
	{
		if ((int)m_pointLights.size() != 0)
		{
			CopyCPUToGPU(&m_pointLights[0], sizeof(PointLights), m_pointLightCBO);
			BindConstantBuffer(5, m_pointLightCBO);
		}
	}

	size_t size = sizeof(Vertex_PNCU) * numVertexes;
	CopyCPUToGPU(vertexes, size, m_immediateVBO_PNCU);
	DrawVertexBuffer(m_immediateVBO_PNCU, numVertexes, 0);
}
void Renderer::DrawVertexArray(std::vector<Vertex_PNCU> const& vertexes)
{
	ModelConstants modelConstant;
	modelConstant.ModelMatrix = m_modelMatrix;
	modelConstant.color[0] = m_modelColor[0];
	modelConstant.color[1] = m_modelColor[1];
	modelConstant.color[2] = m_modelColor[2];
	modelConstant.color[3] = m_modelColor[3];
	CopyCPUToGPU(&modelConstant, sizeof(modelConstant), m_modalCBO);
	BindConstantBuffer(m_modalconstantBufferSlot, m_modalCBO);

	LightConstants lightConstant{};
	lightConstant.sunDirection[0] = m_sunDirection.x;
	lightConstant.sunDirection[1] = m_sunDirection.y;
	lightConstant.sunDirection[2] = m_sunDirection.z;
	lightConstant.SunIntensity = m_sunIntensity;
	lightConstant.AmbientIntensity = m_ambientIntensity;
	CopyCPUToGPU(&lightConstant, sizeof(LightConstants), m_lightCBO);
	BindConstantBuffer(m_lightConstantBufferSlot, m_lightCBO);

	size_t size = sizeof(Vertex_PNCU) * vertexes.size();
	CopyCPUToGPU(vertexes.data(), size, m_immediateVBO_PNCU);
	DrawVertexBuffer(m_immediateVBO_PNCU, (int)vertexes.size(), 0);
}
void Renderer::DrawVrVertexArray(int numVertexes, std::vector<Vertex_PCU> vertexes, ID3D11DepthStencilView* depthView, ID3D11RenderTargetView* targetView)
{
	UNUSED((void)depthView);
	UNUSED((void)targetView);
	ModelConstants modelConstant;
	modelConstant.ModelMatrix = m_modelMatrix;
	modelConstant.color[0] = m_modelColor[0];
	modelConstant.color[1] = m_modelColor[1];
	modelConstant.color[2] = m_modelColor[2];
	modelConstant.color[3] = m_modelColor[3];
	CopyCPUToGPU(&modelConstant, sizeof(modelConstant), m_modalCBO);
	BindConstantBuffer(m_modalconstantBufferSlot, m_modalCBO);

	size_t size = sizeof(Vertex_PCU) * numVertexes;
	CopyCPUToGPU(vertexes.data(), size, m_immediateVBO);
	BindVertexBuffer(m_immediateVBO);
	m_deviceContext->Draw((int)size, 0);
}
void Renderer::DrawVertexArray(int numVertexes, Vertex_PCU const* vertexes)
{
	/*glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < numVertexes; ++i)
		{
			glColor4ub(vertexes[i].m_color.r, vertexes[i].m_color.g, vertexes[i].m_color.b, vertexes[i].m_color.a);
			glTexCoord2f(vertexes[i].m_uvTexCoords.x, vertexes[i].m_uvTexCoords.y);
			glVertex3f(vertexes[i].m_position.x, vertexes[i].m_position.y, vertexes[i].m_position.z);
		}
	}
	glEnd();*/

	ModelConstants modelConstant;
	modelConstant.ModelMatrix = m_modelMatrix;
	modelConstant.color[0] = m_modelColor[0];
	modelConstant.color[1] = m_modelColor[1];
	modelConstant.color[2] = m_modelColor[2];
	modelConstant.color[3] = m_modelColor[3];
	CopyCPUToGPU(&modelConstant, sizeof(modelConstant), m_modalCBO);
	BindConstantBuffer(m_modalconstantBufferSlot, m_modalCBO);

	size_t size = sizeof(Vertex_PCU) * numVertexes;
	CopyCPUToGPU(vertexes, size, m_immediateVBO);
	
	DrawVertexBuffer(m_immediateVBO, numVertexes, 0);
}
void Renderer::DrawVertexArray(int numVertexes, std::vector<Vertex_PCU> const& vertexes)
{
	/*glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < numVertexes; ++i)
		{
			glColor4ub(vertexes[i].m_color.r, vertexes[i].m_color.g, vertexes[i].m_color.b, vertexes[i].m_color.a);
			glTexCoord2f(vertexes[i].m_uvTexCoords.x, vertexes[i].m_uvTexCoords.y);
			glVertex3f(vertexes[i].m_position.x, vertexes[i].m_position.y, vertexes[i].m_position.z);
		}
	}
	glEnd();*/

	ModelConstants modelConstant;
	modelConstant.ModelMatrix = m_modelMatrix;
	modelConstant.color[0] = m_modelColor[0];
	modelConstant.color[1] = m_modelColor[1];
	modelConstant.color[2] = m_modelColor[2];
	modelConstant.color[3] = m_modelColor[3];
	CopyCPUToGPU(&modelConstant, sizeof(modelConstant), m_modalCBO);
	BindConstantBuffer(m_modalconstantBufferSlot, m_modalCBO);

	size_t size = sizeof(Vertex_PCU) * numVertexes;
	CopyCPUToGPU(vertexes.data(), size, m_immediateVBO);
	DrawVertexBuffer(m_immediateVBO, numVertexes, 0);
}

void Renderer::DrawIndexedVertexArray(int numVertexes, Vertex_PCU const* vertexes, int indeces, const unsigned int* indexes, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer)
{
	UNUSED((void)indexes);
	UNUSED((void)vertexes);
	UNUSED((void)vertexBuffer);
	UNUSED((void)indexBuffer);
	UNUSED((void)numVertexes);
	ModelConstants modelConstant;
	modelConstant.ModelMatrix = m_modelMatrix;
	modelConstant.color[0] = m_modelColor[0];
	modelConstant.color[1] = m_modelColor[1];
	modelConstant.color[2] = m_modelColor[2];
	modelConstant.color[3] = m_modelColor[3];
	CopyCPUToGPU(&modelConstant, sizeof(modelConstant), m_modalCBO);
	BindConstantBuffer(m_modalconstantBufferSlot, m_modalCBO);

	LightConstants lightConstant{};
	lightConstant.sunDirection[0] = m_sunDirection.x;
	lightConstant.sunDirection[1] = m_sunDirection.y;
	lightConstant.sunDirection[2] = m_sunDirection.z;
	lightConstant.SunIntensity = m_sunIntensity;
	lightConstant.AmbientIntensity = m_ambientIntensity;
	CopyCPUToGPU(&lightConstant, sizeof(LightConstants), m_lightCBO);
	BindConstantBuffer(m_lightConstantBufferSlot, m_lightCBO);

	if (m_currentShader->GetName().find("Lit") != std::string::npos)
	{
		if ((int)m_pointLights.size() != 0)
		{
			CopyCPUToGPU(&m_pointLights[0], sizeof(PointLights), m_pointLightCBO);
			BindConstantBuffer(5, m_pointLightCBO);
		}
	}

	if (m_currentShader->GetName().find("Minecraft") != std::string::npos)
	{
		CopyCPUToGPU(&m_GameConstants, sizeof(MinecraftGameConstants), m_gameConstantCBO);
		BindConstantBuffer(m_minecraftBufferSlot, m_gameConstantCBO);
	}
	m_deviceContext->DrawIndexed(indeces, 0, 0);
	//DrawIndexedVertexBuffer(vertexBuffer, indexBuffer, numVertexes, 0, indeces, 0);
}
void Renderer::DrawIndexedVertexArray( int indeces )
{
	ModelConstants modelConstant;
	modelConstant.ModelMatrix = m_modelMatrix;
	modelConstant.color[0] = m_modelColor[0];
	modelConstant.color[1] = m_modelColor[1];
	modelConstant.color[2] = m_modelColor[2];
	modelConstant.color[3] = m_modelColor[3];
	CopyCPUToGPU(&modelConstant, sizeof(modelConstant), m_modalCBO);
	BindConstantBuffer(m_modalconstantBufferSlot, m_modalCBO);

	if (m_currentShader->GetName().find("Lit") != std::string::npos)
	{
		//LightConstants lightConstant{};
		//lightConstant.sunDirection[0] = m_sunDirection.x;
		//lightConstant.sunDirection[1] = m_sunDirection.y;
		//lightConstant.sunDirection[2] = m_sunDirection.z;
		//lightConstant.SunIntensity = m_sunIntensity;
		//lightConstant.AmbientIntensity = m_ambientIntensity;
		//CopyCPUToGPU(&lightConstant, sizeof(LightConstants), m_lightCBO);
		//BindConstantBuffer(m_lightConstantBufferSlot, m_lightCBO);

		if ((int)m_pointLights.size() != 0)
		{
			CopyCPUToGPU(&m_pointLights[0], sizeof(PointLights), m_pointLightCBO);
			BindConstantBuffer(5, m_pointLightCBO);
		}
	}
	if (m_currentShader->GetName().find("Minecraft") != std::string::npos)
	{
		CopyCPUToGPU(&m_GameConstants, sizeof(MinecraftGameConstants), m_gameConstantCBO);
		BindConstantBuffer(m_minecraftBufferSlot, m_gameConstantCBO);
	}
	m_deviceContext->DrawIndexed(indeces, 0, 0);
}

void Renderer::DrawIndexedVertexArray(int numVertexes, Vertex_PNCU const* vertexes, int indeces, const unsigned int* indexes, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer)
{
	UNUSED((void)indexes);
	UNUSED((void)vertexes);
	ModelConstants modelConstant;
	modelConstant.ModelMatrix = m_modelMatrix;
	modelConstant.color[0] = m_modelColor[0];
	modelConstant.color[1] = m_modelColor[1];
	modelConstant.color[2] = m_modelColor[2];
	modelConstant.color[3] = m_modelColor[3];
	CopyCPUToGPU(&modelConstant, sizeof(modelConstant), m_modalCBO);
	BindConstantBuffer(m_modalconstantBufferSlot, m_modalCBO);

	LightConstants lightConstant{};
	lightConstant.sunDirection[0] = m_sunDirection.x;
	lightConstant.sunDirection[1] = m_sunDirection.y;
	lightConstant.sunDirection[2] = m_sunDirection.z;
	lightConstant.SunIntensity = m_sunIntensity;
	lightConstant.AmbientIntensity = m_ambientIntensity;
	CopyCPUToGPU(&lightConstant, sizeof(LightConstants), m_lightCBO);
	BindConstantBuffer(m_lightConstantBufferSlot, m_lightCBO);

	if (m_currentShader->GetName().find("Lit") != std::string::npos)
	{
		if ((int)m_pointLights.size() != 0)
		{
			CopyCPUToGPU(&m_pointLights[0], sizeof(PointLights), m_pointLightCBO);
			BindConstantBuffer(5, m_pointLightCBO);
		}
	}

	//size_t size = sizeof(Vertex_PNCU) * numVertexes;
	//CopyCPUToGPU(vertexes, size, vertexBuffer);

	//size = sizeof(unsigned int) * indeces;
	//CopyCPUToGPU(indexes, size, indexBuffer);
	DrawIndexedVertexBuffer(vertexBuffer, indexBuffer, numVertexes, 0, indeces, 0);
}
void Renderer::DrawVertexArray(int numVertexes, std::vector<Vertex_PCU>& vertexes)
{
	ModelConstants modelConstant;
	modelConstant.ModelMatrix = m_modelMatrix;
	modelConstant.color[0] = m_modelColor[0];
	modelConstant.color[1] = m_modelColor[1];
	modelConstant.color[2] = m_modelColor[2];
	modelConstant.color[3] = m_modelColor[3];
	CopyCPUToGPU(&modelConstant, sizeof(modelConstant), m_modalCBO);
	BindConstantBuffer(m_modalconstantBufferSlot, m_modalCBO);

	if (m_currentShader->GetName().find("Fire") != std::string::npos)
	{
		DistortionConstants fireDistortion;
		fireDistortion.distortion1[0] = 0.1f;
		fireDistortion.distortion1[1] = 0.2f;
		fireDistortion.distortion2[0] = 0.1f;
		fireDistortion.distortion2[1] = 0.3f;
		fireDistortion.distortion3[0] = 0.1f;
		fireDistortion.distortion3[1] = 0.1f;
		fireDistortion.distortionScale = 0.8f;
		fireDistortion.distortionBias = 0.5f;

		CopyCPUToGPU(&fireDistortion, sizeof(fireDistortion), m_fireDistortionCBO);
		BindConstantBuffer(m_fireDistortionConstantBufferSlot, m_fireDistortionCBO);

		NoiseConstants fireNoise;
		fireNoise.frameTime = m_frameTime;
		fireNoise.scrollSpeeds[0] = -1.3f;
		fireNoise.scrollSpeeds[1] = -2.1f;
		fireNoise.scrollSpeeds[2] = -2.3f;
		fireNoise.scales[0] = 1.0f;
		fireNoise.scales[1] = 2.0f;
		fireNoise.scales[2] = 3.0f;

		CopyCPUToGPU(&fireNoise, sizeof(fireNoise), m_fireNoiseCBO);
		BindConstantBuffer(m_fireNoiseConstantBufferSlot, m_fireNoiseCBO);
	}
	if (m_currentShader->GetName().find("Lit") != std::string::npos)
	{
		if ((int)m_pointLights.size() != 0)
		{
			CopyCPUToGPU(&m_pointLights[0], sizeof(PointLights), m_pointLightCBO);
			BindConstantBuffer(5, m_pointLightCBO);
		}
	}
	size_t size = sizeof(Vertex_PCU) * numVertexes;
	CopyCPUToGPU(vertexes.data(), size, m_immediateVBO);
	DrawVertexBuffer(m_immediateVBO, numVertexes, 0);
	/*glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < numVertexes; ++i)
		{
			glColor4ub(vertexes[i].m_color.r, vertexes[i].m_color.g, vertexes[i].m_color.b, vertexes[i].m_color.a);
			glTexCoord2f(vertexes[i].m_uvTexCoords.x, vertexes[i].m_uvTexCoords.y);
			glVertex3f(vertexes[i].m_position.x, vertexes[i].m_position.y, vertexes[i].m_position.z);
		}
	}
	glEnd();*/
}
void Renderer::DrawVertexArray(std::vector<Vertex_PCU>& vertexes)
{
	int numVertexes = (int)vertexes.size();
	ModelConstants modelConstant;
	modelConstant.ModelMatrix = m_modelMatrix;
	modelConstant.color[0] = m_modelColor[0];
	modelConstant.color[1] = m_modelColor[1];
	modelConstant.color[2] = m_modelColor[2];
	modelConstant.color[3] = m_modelColor[3];
	CopyCPUToGPU(&modelConstant, sizeof(modelConstant), m_modalCBO);
	BindConstantBuffer(m_modalconstantBufferSlot, m_modalCBO);

	if (m_currentShader->GetName().find("Fire") != std::string::npos)
	{
		DistortionConstants fireDistortion;
		fireDistortion.distortion1[0] = 0.1f;
		fireDistortion.distortion1[1] = 0.2f;
		fireDistortion.distortion2[0] = 0.1f;
		fireDistortion.distortion2[1] = 0.3f;
		fireDistortion.distortion3[0] = 0.1f;
		fireDistortion.distortion3[1] = 0.1f;
		fireDistortion.distortionScale = 0.8f;
		fireDistortion.distortionBias = 0.5f;

		CopyCPUToGPU(&fireDistortion, sizeof(fireDistortion), m_fireDistortionCBO);
		BindConstantBuffer(m_fireDistortionConstantBufferSlot, m_fireDistortionCBO);

		NoiseConstants fireNoise;
		fireNoise.frameTime = m_frameTime;
		fireNoise.scrollSpeeds[0] = -1.3f;
		fireNoise.scrollSpeeds[1] = -2.1f;
		fireNoise.scrollSpeeds[2] = -2.3f;
		fireNoise.scales[0] = 1.0f;
		fireNoise.scales[1] = 2.0f;
		fireNoise.scales[2] = 3.0f;

		CopyCPUToGPU(&fireNoise, sizeof(fireNoise), m_fireNoiseCBO);
		BindConstantBuffer(m_fireNoiseConstantBufferSlot, m_fireNoiseCBO);
	}
	if (m_currentShader->GetName().find("Lit") != std::string::npos)
	{
		if ((int)m_pointLights.size() != 0)
		{
			CopyCPUToGPU(&m_pointLights[0], sizeof(PointLights), m_pointLightCBO);
			BindConstantBuffer(5, m_pointLightCBO);
		}
	}
	size_t size = sizeof(Vertex_PCU) * numVertexes;
	CopyCPUToGPU(vertexes.data(), size, m_immediateVBO);
	DrawVertexBuffer(m_immediateVBO, numVertexes, 0);
	/*glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < numVertexes; ++i)
		{
			glColor4ub(vertexes[i].m_color.r, vertexes[i].m_color.g, vertexes[i].m_color.b, vertexes[i].m_color.a);
			glTexCoord2f(vertexes[i].m_uvTexCoords.x, vertexes[i].m_uvTexCoords.y);
			glVertex3f(vertexes[i].m_position.x, vertexes[i].m_position.y, vertexes[i].m_position.z);
		}
	}
	glEnd();*/
}
void Renderer::DrawVertexArray(int numVertexes, std::vector<Vertex_PNCU> const& vertexes)
{
	ModelConstants modelConstant;
	modelConstant.ModelMatrix = m_modelMatrix;
	modelConstant.color[0] = m_modelColor[0];
	modelConstant.color[1] = m_modelColor[1];
	modelConstant.color[2] = m_modelColor[2];
	modelConstant.color[3] = m_modelColor[3];
	CopyCPUToGPU(&modelConstant, sizeof(modelConstant), m_modalCBO);
	BindConstantBuffer(m_modalconstantBufferSlot, m_modalCBO);

	if (m_currentShader->GetName().find("Lit") != std::string::npos)
	{
		LightConstants lightConstant{};
		lightConstant.sunDirection[0] = m_sunDirection.x;
		lightConstant.sunDirection[1] = m_sunDirection.y;
		lightConstant.sunDirection[2] = m_sunDirection.z;
		lightConstant.SunIntensity = m_sunIntensity;
		lightConstant.AmbientIntensity = m_ambientIntensity;
		CopyCPUToGPU(&lightConstant, sizeof(LightConstants), m_lightCBO);
		BindConstantBuffer(m_lightConstantBufferSlot, m_lightCBO);

		if ((int)m_pointLights.size() != 0)
		{
 			CopyCPUToGPU(&m_pointLights[0], sizeof(PointLights), m_pointLightCBO);
			BindConstantBuffer(5, m_pointLightCBO);
		}
	}
	if (m_currentShader->GetName().find("Raytracing") != std::string::npos)
	{
 		Mat44 mat = Mat44::CreateTranslation3D(m_currentCamera->m_position);
		Mat44 rotationMatrix = m_currentCamera->m_orientation.GetAsMatrix_XFwd_YLeft_ZUp();
		mat.Append(rotationMatrix);

		m_worldSpheres.CameraPosition[0] = mat.m_values[12];
		m_worldSpheres.CameraPosition[1] = mat.m_values[13];
		m_worldSpheres.CameraPosition[2] = mat.m_values[14];

		m_worldSpheres.CameraDirection[0] = mat.m_values[0];
		m_worldSpheres.CameraDirection[1] = mat.m_values[1];
		m_worldSpheres.CameraDirection[2] = mat.m_values[2];

		CopyCPUToGPU(&m_worldSpheres, sizeof(m_worldSpheres), m_worldSpheresCBO);
		BindConstantBuffer(m_sphereBufferSlot, m_worldSpheresCBO);

		CopyCPUToGPU(&m_worldCubes, sizeof(m_worldCubes), m_worldCubesCBO);
		BindConstantBuffer(m_cubeBufferSlot, m_worldCubesCBO);

		CopyCPUToGPU(&m_worldPlanes, sizeof(m_worldPlanes), m_worldPlanesCBO);
		BindConstantBuffer(m_planeBufferSlot, m_worldPlanesCBO);
	}

	size_t size = sizeof(Vertex_PNCU) * numVertexes;
	CopyCPUToGPU(vertexes.data(), size, m_immediateVBO_PNCU);
	DrawVertexBuffer(m_immediateVBO_PNCU, (int)vertexes.size(), 0);
}
//-------------------------------TEXTURE FUNCTIONS-------------------------

static DXGI_FORMAT LocalToD3D11(eTextureFormat format)
{
	switch (format)
	{
	case eTextureFormat::R8G8B8A8_UNORM:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case eTextureFormat::D24_UNORM_S8_UINT:
		return DXGI_FORMAT_D24_UNORM_S8_UINT;
		break;
	case eTextureFormat::R24G8_TYPELESS:
		return DXGI_FORMAT_R24G8_TYPELESS;
		break;
	default: 
		ERROR_AND_DIE("Unsupported format");
	}
}

static D3D11_USAGE LocalToD3D11(eMemoryHint hint)
{
	switch (hint)
	{
	case eMemoryHint::STATIC:
		return D3D11_USAGE_IMMUTABLE;
		break;
	case eMemoryHint::GPU:
		return D3D11_USAGE_DEFAULT;
		break;
	case eMemoryHint::DYNAMIC:
		return D3D11_USAGE_DYNAMIC;
		break;
	default:
		ERROR_AND_DIE("Unsupported format");
		break;
	}
}


static D3D11_USAGE LocalToD3D11(eTextureBindFlagBit flag)
{
	UINT result = 0;
	if (flag & TEXTURE_BIND_SHADER_RESOURCE_BIT)
	{
		result |= D3D11_BIND_SHADER_RESOURCE;
	}
	if (flag & TEXTURE_BIND_DEPTH_STENCIL_BIT)
	{
		result |= D3D11_BIND_DEPTH_STENCIL;
	}
	if (flag & TEXTURE_BIND_RENDER_TARGET_BIT)
	{
		result |= D3D11_BIND_RENDER_TARGET;
	}
	return D3D11_USAGE(result);
}

static UINT LocalToD3D11BindFlags(const eTextureBindFlags bindFlags)
{
	UINT result = 0;

	if (bindFlags & TEXTURE_BIND_SHADER_RESOURCE_BIT)
	{
		result |= D3D11_BIND_SHADER_RESOURCE;
	}
	if (bindFlags & TEXTURE_BIND_RENDER_TARGET_BIT)
	{
		result |= D3D11_BIND_RENDER_TARGET;
	}
	if (bindFlags & TEXTURE_BIND_DEPTH_STENCIL_BIT)
	{
		result |= D3D11_BIND_DEPTH_STENCIL;
	}

	return result;
}

Texture* Renderer::GetTextureForFileName(char const* imageFilePath)
{
	for (int i = 0; i < m_loadedTextures.size(); i++)
	{
		if (m_loadedTextures[i]->GetImageFilePath() == imageFilePath)
		{
			return m_loadedTextures[i];
		}
	}
	return nullptr;
}
Texture* Renderer::CreateTextureFromImage(const Image& image)
{
	TextureCreateInfo ci{};
	ci.name = image.GetImageFilePath();
	ci.dimensions = image.GetDimensions();
	ci.initialData = image.GetRawData();
	ci.bindFlags = TEXTURE_BIND_SHADER_RESOURCE_BIT;
	ci.intitialDataBySize = ci.dimensions.x * ci.dimensions.y * sizeof(Rgba8);

	Texture* newTexture = CreateTextureFromInfo(ci);
	return newTexture;
}

Texture* Renderer::CreateTextureFromInfo(const TextureCreateInfo& createInfo)
{
	ID3D11Texture2D* handle = createInfo.handle;

	if (handle == nullptr)
	{
		D3D11_TEXTURE2D_DESC pTextureDesc = { 0 };
		pTextureDesc.Width = createInfo.dimensions.x;
		pTextureDesc.Height = createInfo.dimensions.y;
		pTextureDesc.MipLevels = 1;
		pTextureDesc.ArraySize = 1;
		pTextureDesc.Format = LocalToD3D11(createInfo.format);
		pTextureDesc.SampleDesc.Count = 1;
		pTextureDesc.Usage = LocalToD3D11(createInfo.memory);
		pTextureDesc.BindFlags = LocalToD3D11BindFlags(createInfo.bindFlags);

		D3D11_SUBRESOURCE_DATA pIntialData = { 0 };
		D3D11_SUBRESOURCE_DATA* pInitialDataPtr = nullptr;
		if (createInfo.initialData != nullptr)
		{
			pIntialData.pSysMem = createInfo.initialData;
			pIntialData.SysMemPitch = (UINT)createInfo.intitialDataBySize / (UINT)createInfo.dimensions.y;
			pInitialDataPtr = &pIntialData;
		}

		HRESULT textureResult = m_device->CreateTexture2D(&pTextureDesc, pInitialDataPtr, &handle);
		if (!SUCCEEDED(textureResult))
		{
			std::string errorString = "Error while creating 2d texture: ";
			errorString.append(createInfo.name);
			ERROR_AND_DIE(errorString);
		}
	}
	else
	{
		handle->AddRef();
	}

	Texture* tex = new Texture();
	tex->m_renderer = this;
	tex->m_textureFormat = createInfo.format;
	tex->m_name = createInfo.name;
	tex->m_dimensions = createInfo.dimensions;
	tex->m_texture = handle;
	tex->m_allowedBinds = createInfo.bindFlags;

	return tex;
}

Texture* Renderer::CreateOrGetTextureFromFile(char const* imageFilePath)
{
		//See if we already have this texture previously loaded
	Texture* existingTexture = GetTextureForFileName(imageFilePath);
	if (existingTexture)
	{
		return existingTexture;
	}

	// Never seen this texture before!  Let's load it.
	Texture* newTexture = CreateTextureFromFile(imageFilePath);
	return newTexture;
}
Texture* Renderer::CreateTextureFromFile(char const* imageFilePath)
{
	// -----------------OPEN GL------------------------------
	//IntVec2 dimensions = IntVec2::ZERO;		// This will be filled in for us to indicate image width & height
	//int bytesPerTexel = 0; // This will be filled in for us to indicate how many color components the image had (e.g. 3=RGB=24bit, 4=RGBA=32bit)
	//int numComponentsRequested = 0; // don't care; we support 3 (24-bit RGB) or 4 (32-bit RGBA)

	//								// Load (and decompress) the image RGB(A) bytes from a file on disk into a memory buffer (array of bytes)
	//stbi_set_flip_vertically_on_load(1); // We prefer uvTexCoords has origin (0,0) at BOTTOM LEFT
	//unsigned char* texelData = stbi_load(imageFilePath, &dimensions.x, &dimensions.y, &bytesPerTexel, numComponentsRequested);

	//// Check if the load was successful
	//GUARANTEE_OR_DIE( texelData, Stringf( "Failed to load image %s\n", imageFilePath ) );

	//Texture* newTexture = CreateTextureFromData(imageFilePath, dimensions, bytesPerTexel, texelData);

	//// Free the raw image texel data now that we've sent a copy of it down to the GPU to be stored in video memory
	//stbi_image_free(texelData);

	//m_loadedTextures.push_back(newTexture);
	//return newTexture;

	//------------DIRECTX -------------------------
	Image* textureImage = new Image(imageFilePath);
	Texture* texture = CreateTextureFromImage(*textureImage);
	SetDebugName(texture->m_texture, imageFilePath);
	m_loadedTextures.push_back(texture);
	return texture;
}
Texture* Renderer::CreateTextureFromData(char const* name, IntVec2 dimensions, int bytesPerTexel, uint8_t* texelData)
{
	UNUSED((void)name);
	UNUSED((void)dimensions);
	UNUSED((void)bytesPerTexel);
	UNUSED((void)texelData);
	//// Check if the load was successful
	//GUARANTEE_OR_DIE(texelData, Stringf("CreateTextureFromData failed for %s\n - texelData was null!", name));
	//const std::string errorString = Stringf("CreateTextureFromData failed for \"%s\" - unsupported BPP=%i (must be 3 or 4)", name, bytesPerTexel);
	//GUARANTEE_OR_DIE(bytesPerTexel >= 3 && bytesPerTexel <= 4, errorString);
	//const std::string errorString2 = Stringf("CreateTextureFromData failed for \"%s\" - illegal texture dimensions (%i x %i)", name, dimensions.x, dimensions.y);
	//GUARANTEE_OR_DIE(dimensions.x > 0 && dimensions.y > 0, errorString2);

	//Texture* newTexture = new Texture();
	//newTexture->m_name = name;
	//newTexture->m_dimensions = dimensions;

	//// Enable OpenGL texturing
	//glEnable(GL_TEXTURE_2D);

	//// Tell OpenGL that our pixel data is single-byte aligned
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//// Ask OpenGL for an unused texName (ID number) to use for this texture
	//glGenTextures(1, (GLuint*)&newTexture->m_textureID);

	//// Tell OpenGL to bind (set) this as the currently active texture
	//glBindTexture(GL_TEXTURE_2D, newTexture->m_textureID);

	//// Set texture clamp vs. wrap (repeat) default settings
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_CLAMP or GL_REPEAT
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // GL_CLAMP or GL_REPEAT

	//																// Set magnification (texel > pixel) and minification (texel < pixel) filters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // one of: GL_NEAREST, GL_LINEAR
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // one of: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR

	//																	 // Pick the appropriate OpenGL format (RGB or RGBA) for this texel data
	//GLenum bufferFormat = GL_RGBA; // the format our source pixel data is in; any of: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, ...
	//if (bytesPerTexel == 3)
	//{
	//	bufferFormat = GL_RGB;
	//}
	//GLenum internalFormat = bufferFormat; // the format we want the texture to be on the card; technically allows us to translate into a different texture format as we upload to OpenGL

	//									  // Upload the image texel data (raw pixels bytes) to OpenGL under this textureID
	//glTexImage2D(			// Upload this pixel data to our new OpenGL texture
	//	GL_TEXTURE_2D,		// Creating this as a 2d texture
	//	0,					// Which mipmap level to use as the "root" (0 = the highest-quality, full-res image), if mipmaps are enabled
	//	internalFormat,		// Type of texel format we want OpenGL to use for this texture internally on the video card
	//	dimensions.x,		// Texel-width of image; for maximum compatibility, use 2^N + 2^B, where N is some integer in the range [3,11], and B is the border thickness [0,1]
	//	dimensions.y,		// Texel-height of image; for maximum compatibility, use 2^M + 2^B, where M is some integer in the range [3,11], and B is the border thickness [0,1]
	//	0,					// Border size, in texels (must be 0 or 1, recommend 0)
	//	bufferFormat,		// Pixel format describing the composition of the pixel data in buffer
	//	GL_UNSIGNED_BYTE,	// Pixel color components are unsigned bytes (one byte per color channel/component)
	//	texelData);		// Address of the actual pixel data bytes/buffer in system memory

	//m_loadedTextures.push_back(newTexture);
	//return newTexture;
	return nullptr;
}
void Renderer::BindTexture(const Texture* texture)
{
	//----------------OPENGL------------------------------
/*	if (texture)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture->m_textureID);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}*/

	//-------------DIRECTX--------------------
	if (texture != nullptr)
	{
		int slots = 0;
		int numViews = 1;
		Texture* tex = const_cast<Texture*>(texture);
		TextureView* view = tex->GetShaderResourceView();
		m_deviceContext->PSSetShaderResources(slots, numViews, &(view->m_srv));
	}
	else BindTexture(m_defaultTexture);
	return;
}
void Renderer::BindTexture(const Texture* texture, int slot)
{
	//-------------DIRECTX--------------------
	if (texture != nullptr)
	{
		int numViews = 1;
		Texture* tex = const_cast<Texture*>(texture);
		TextureView* view = tex->GetShaderResourceView();
		m_deviceContext->PSSetShaderResources(slot, numViews, &(view->m_srv));
	}
	else BindTexture(m_defaultTexture);
	return;
}
void Renderer::BindTexture(std::vector<Texture*>  textures, std::vector<int> slots)
{
	int numViews = 1;
	//-------------DIRECTX--------------------
	for (int i = 0; i < (int)textures.size(); i++)
	{
		if (textures[i] != nullptr)
		{
			Texture* tex = const_cast<Texture*>(textures[i]);
			TextureView* view = tex->GetShaderResourceView();
			m_deviceContext->PSSetShaderResources(slots[i], numViews, &(view->m_srv));
		}
		else BindTexture(m_defaultTexture);
	}
	return;
}
//-----------------------------FONT FUNCTIONS----------------------------------
BitmapFont* Renderer::CreateOrGetBitmapFont(const char* bitmapFontFilePathWithNoExtension)
{
	//See if we already have this texture previously loaded
	BitmapFont* existingFont = GetFontForFileName(bitmapFontFilePathWithNoExtension);
	if (existingFont)
	{
		return existingFont;
	}

	// Never seen this texture before!  Let's load it.
	BitmapFont* newFont = CreateFontFromFile(bitmapFontFilePathWithNoExtension);
	return newFont;
}
BitmapFont* Renderer::CreateFontFromFile(char const* fontFilePath)
{
	std::string fontFilePathWithExtension = std::string(fontFilePath) + ".png";
	Texture* bitmapFontTexture = CreateTextureFromFile(fontFilePathWithExtension.c_str());
	BitmapFont* newFont = new BitmapFont(fontFilePath, *bitmapFontTexture);

	m_loadedFonts.push_back(newFont);
	return newFont;
}
BitmapFont* Renderer::GetFontForFileName(char const* fontFilePath)
{
	for (int i = 0; i < m_loadedFonts.size(); i++)
	{
		if (m_loadedFonts[i]->m_fontFilePathNameWithNoExtension == fontFilePath)
		{
			return m_loadedFonts[i];
		}
	}
	return nullptr;
}


//---------------------------STENCIL DEPTH AND SAMPLER FUNCTIONS--------------------------------------------
void Renderer::SetBlendMode(BlendMode blendMode)
{
	//------------------------DIRECTX--------------------------------
	DX_SAFE_RELEASE(m_blendState);
	D3D11_BLEND_DESC blendDesc = { 0 };
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	if (blendMode == BlendMode::ALPHA)
	{
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		
	}
	else if (blendMode == BlendMode::ADDITIVE)
	{
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	}
	else if(blendMode == BlendMode::Opaque)
	{
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	}
	m_device->CreateBlendState(&blendDesc, &m_blendState);
	float arr[4] = { 0,0,0,0 };
	m_deviceContext->OMSetBlendState(m_blendState, arr, 0xffffffff);

	//----------------------OPENGL-------------------------
/*	if (blendMode == BlendMode::ALPHA)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else if (blendMode == BlendMode::ADDITIVE)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	else
	{
		ERROR_AND_DIE(Stringf("Unknown / unsupported blend mode #%i", blendMode));
	}*/
}

void Renderer::CopyTexture(Texture* src, Texture* dest)
{
	m_deviceContext->CopyResource(dest->m_texture, src->m_texture);
}
void Renderer::PrepareRendererForNextPass()
{
	Texture* src = GetCurrentColorTarget();
	TextureView* rtv = src->GetRenderTargetView();
	m_deviceContext->OMSetRenderTargets(1, &rtv->m_rtv, nullptr);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
void Renderer::ApplySpecialEffects(Shader* shader)
{
	BindShader(shader);
	Texture* src = GetCurrentColorTarget();
	Texture* dst = m_activeColorTargetIndex == 0 ? m_activeColorTarget[1] : m_activeColorTarget[0];
	Texture* depth = GetCurrentDepthTarget();

	TextureView* rtv = dst->GetRenderTargetView();
	m_deviceContext->OMSetRenderTargets(1, &rtv->m_rtv, nullptr);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	float windowDimensionsX = static_cast<float>(src->m_dimensions.x);
	float windowDimensionsY = static_cast<float>(src->m_dimensions.y);
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = windowDimensionsX;
	viewport.Height = windowDimensionsY;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	m_deviceContext->RSSetViewports(1, &viewport);

	BindTexture(src, 0);
	BindTexture(depth, 1);
	BindSampler();
	m_deviceContext->Draw(3, 0);
	m_activeColorTargetIndex = (m_activeColorTargetIndex + 1) % 2;
	m_deviceContext->ClearState();
}


void Renderer::ApplySpecialEffects(Shader* shader,  ConstantBuffer* constantBuffer,EffectsBuffer& data)
{
	BindShader(shader);
	Texture* src = GetCurrentColorTarget();
	Texture* dst = m_activeColorTargetIndex == 0 ? m_activeColorTarget[1]  : m_activeColorTarget[0];
	Texture* depth =  GetCurrentDepthTarget();

	TextureView* rtv = dst->GetRenderTargetView();
	m_deviceContext->OMSetRenderTargets(1, &rtv->m_rtv, nullptr);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	float windowDimensionsX = static_cast<float>(src->m_dimensions.x);
	float windowDimensionsY = static_cast<float>(src->m_dimensions.y);
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0 ;
	viewport.TopLeftY = 0 ;
	viewport.Width = windowDimensionsX ;
	viewport.Height = windowDimensionsY ;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	m_deviceContext->RSSetViewports(1, &viewport);

	CopyCPUToGPU(&data, sizeof(EffectsBuffer), constantBuffer);
	BindConstantBuffer(1, constantBuffer);
	BindTexture(src, 0);
	BindTexture(depth, 1);
	BindSampler();
	m_deviceContext->Draw(3, 0);
	m_activeColorTargetIndex = (m_activeColorTargetIndex + 1) % 2;
	m_deviceContext->ClearState();
}

void Renderer::CreateDepthStencilTextureAndView()
{
	TextureCreateInfo info;
	info.dimensions = IntVec2(m_redererConfig.m_window->GetClientDimensions());

	info.format = eTextureFormat::R24G8_TYPELESS;
	info.bindFlags = eTextureBindFlagBit::TEXTURE_BIND_DEPTH_STENCIL_BIT;
	info.bindFlags |= eTextureBindFlagBit::TEXTURE_BIND_SHADER_RESOURCE_BIT;
	info.memory = eMemoryHint::GPU;

	m_depthBuffer = CreateTextureFromInfo(info);
}
void Renderer::CreateBackBuffer()
{
	ID3D11Texture2D* texture2D = nullptr;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&texture2D));
	D3D11_TEXTURE2D_DESC desc;
	texture2D->GetDesc(&desc);

	TextureCreateInfo info;
	info.name = "DefaultColor";
	info.dimensions = IntVec2(desc.Width, desc.Height);
	ASSERT_OR_DIE(desc.Format == DXGI_FORMAT_R8G8B8A8_UNORM, "Swapchain isn't R8G8B8A - add conversion to local format");
	info.format = eTextureFormat::R8G8B8A8_UNORM;
	info.bindFlags = TEXTURE_BIND_RENDER_TARGET_BIT;
	info.memory = eMemoryHint::GPU;
	info.handle = texture2D;

	m_backBuffer = CreateTextureFromInfo(info);

	info.name = "DefaultColorTarget";
	info.handle = nullptr;
	info.bindFlags |= TEXTURE_BIND_SHADER_RESOURCE_BIT;

	m_activeColorTarget[0] = CreateTextureFromInfo(info);

	info.name = "SpecialEffectsOrBackupColorTarget";
	m_activeColorTarget[1] = CreateTextureFromInfo(info);
	DX_SAFE_RELEASE(texture2D);
}
void Renderer::ClearDepth(float value)
{
	TextureView* dsv = GetCurrentDepthTarget()->GetDepthStencilView();
	m_deviceContext->ClearDepthStencilView(dsv->m_dsv, D3D11_CLEAR_DEPTH, value, 0);
	/*if (m_depthStencilView)
	{
		m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, value, NULL);
	}*/
}
void Renderer::SetDepthStencilState(DepthTest depthTest, bool writeDepth)
{
	D3D11_DEPTH_STENCIL_DESC depthDesc;
	depthDesc.DepthEnable = true;
	if (writeDepth)
	{
		depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	}
	else
	{
		depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	}
	switch (depthTest)
	{
	case DepthTest::ALWAYS:
		depthDesc.DepthFunc  = D3D11_COMPARISON_ALWAYS;
		depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		break;
	case DepthTest::NEVER:
		depthDesc.DepthFunc = D3D11_COMPARISON_NEVER;
		depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER;
		depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
		break;
	case DepthTest::EQUAL:
		depthDesc.DepthFunc = D3D11_COMPARISON_EQUAL;
		depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		break;
	case DepthTest::NOTEQUAL:
		depthDesc.DepthFunc = D3D11_COMPARISON_NOT_EQUAL;
		depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		break;
	case DepthTest::LESS:
		depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS;
		depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_LESS;
		break;
	case DepthTest::LESSEQUAL:
		depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
		break;
	case DepthTest::GREATER:
		depthDesc.DepthFunc = D3D11_COMPARISON_GREATER;
		depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER;
		depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_GREATER;
		break;
	case DepthTest::GREATEREQUAL:
		depthDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;
		depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;
		break;
	default:
		break;
	}
	// Stencil operations if pixel is front-facing
	depthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;


	// Stencil operations if pixel is back-facing
	depthDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	DX_SAFE_RELEASE(m_depthStencilState);
	HRESULT result = m_device->CreateDepthStencilState(&depthDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		ERROR_AND_DIE("Error while creating depth stencil state");
	}
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, NULL);
}
void Renderer::SetSunDirection(const Vec3& direction)
{
	m_sunDirection = direction;
}
void Renderer::SetSunIntensity(float intensity)
{
	m_sunIntensity = intensity;
}
void Renderer::SetAmbientIntensity(float intensity)
{
	m_ambientIntensity = intensity;
}
void Renderer::SetSunColor(float sunColor[3])
{
	m_sunColor[0] = sunColor[0];
	m_sunColor[1] = sunColor[1];
	m_sunColor[2] = sunColor[2];
}

void Renderer::OpenXRRenderFrame()
{
}

bool Renderer::ToggleDebugRendererVisibility()
{
	return false;
}
void Renderer::BindSampler()
{
	int slot = 0;
	int numSamplers = 1;
	m_deviceContext->PSSetSamplers(slot, numSamplers, &m_samplerState);
	slot = 1;
	m_deviceContext->PSSetSamplers(slot, numSamplers, &m_samplerState2);
}
void Renderer::SetSamplerMode(SamplerMode samplerMode)
{
	DX_SAFE_RELEASE(m_samplerState);
	DX_SAFE_RELEASE(m_samplerState2);
	D3D11_SAMPLER_DESC samplerDesc = {  };

	switch (samplerMode)
	{
	case SamplerMode::POINTCLAMP:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		break;
	case SamplerMode::POINTWRAP:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	case SamplerMode::BILINEARCLAMP:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		break;
	case SamplerMode::BILINEARWRAP:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	default:
		break;
	}
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT result = m_device->CreateSamplerState(&samplerDesc, &m_samplerState);
	if (FAILED(result))
	{
		ERROR_AND_DIE("Error while creating create sampler state");
	}

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	HRESULT result2 = m_device->CreateSamplerState(&samplerDesc, &m_samplerState2);
	if (FAILED(result2))
	{
		ERROR_AND_DIE("Error while creating create sampler state number 2");
	}
	BindSampler();
}
void Renderer::BindSampler(int samplerSlot , ID3D11SamplerState* samplerState)
{
	samplerSlot = samplerSlot;
	int numSamplers = 1;
	m_deviceContext->PSSetSamplers(samplerSlot, numSamplers, &samplerState);
}
void Renderer::SetSamplerMode(SamplerMode samplerMode, int slot, ID3D11SamplerState* samplerState)
{
	DX_SAFE_RELEASE(samplerState);
	D3D11_SAMPLER_DESC samplerDesc = {  };

	switch (samplerMode)
	{
	case SamplerMode::POINTCLAMP:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		break;
	case SamplerMode::POINTWRAP:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	case SamplerMode::BILINEARCLAMP:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		break;
	case SamplerMode::BILINEARWRAP:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	default:
		break;
	}
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT result = m_device->CreateSamplerState(&samplerDesc, &samplerState);
	if (FAILED(result))
	{
		ERROR_AND_DIE("Error while creating create sampler state");
	}
	BindSampler(slot, samplerState);
}
//--------------------------------------------LIGHTING-------------------------------------------
void Renderer::AddPointLight(Vec3 Position, float intensity, Rgba8 color, float radius)
{
	PointLights light; 
	light.PointLightPosition[0] = Position.x;
	light.PointLightPosition[1] = Position.y;
	light.PointLightPosition[2] = Position.z;
	light.PointLightIntensity = intensity;
	light.PointLightColor[0] = color.r;
	light.PointLightColor[1] = color.g;
	light.PointLightColor[2] = color.b;
	light.PointLightColor[3] = color.a;
	light.PointLightRadius = radius;
	m_pointLights.push_back(light);

}
Transform Renderer::GetLightData(int index)
{
	Transform transform;
	transform.position = Vec3(m_pointLights[index].PointLightPosition[0], m_pointLights[index].PointLightPosition[1],
		m_pointLights[index].PointLightPosition[2]);
	transform.orientation = EulerAngles(185.0f,42.0f,0.0f);
	return transform;
}
void Renderer::AddWorldObjectSphere(const Vec3& spherePosition, const float sphereRadius, Rgba8 color, int objectNumber)
{
	m_worldSpheres.SpherePosition[objectNumber - 1][0] = spherePosition.x;
	m_worldSpheres.SpherePosition[objectNumber - 1][1] = spherePosition.y;
	m_worldSpheres.SpherePosition[objectNumber - 1][2] = spherePosition.z;
	m_worldSpheres.SpherePosition[objectNumber - 1][3] = 1.0f;
	m_worldSpheres.SphereRadius[objectNumber - 1][0] = sphereRadius;
	color.GetAsFloats(m_worldSpheres.SphereColors[objectNumber - 1]);
	m_worldSpheres.TotalSphereObjects = (float)objectNumber;
}

void Renderer::AddWorldObjectCube(const Vec3& cubePosition, const float cubeDimension, Rgba8 color, int objectNumber)
{
	m_worldCubes.CubePosition[objectNumber - 1][0] = cubePosition.x;
	m_worldCubes.CubePosition[objectNumber - 1][1] = cubePosition.y;
	m_worldCubes.CubePosition[objectNumber - 1][2] = cubePosition.z;
	m_worldCubes.CubePosition[objectNumber - 1][3] = 1.0f;
	m_worldCubes.CubeDimensions[objectNumber - 1] = cubeDimension;
	color.GetAsFloats(m_worldCubes.CubeColors[objectNumber - 1]);
	m_worldCubes.TotalCubeObjects = (float)objectNumber;
}

void Renderer::AddWorldObjectPlane(const Vec3& planePosition,const Vec3& planeNormal, const Vec3& planeDimension, Rgba8 color, int objectNumber)
{
	UNUSED((void)planeNormal);
	m_worldPlanes.PlanePosition[objectNumber - 1][0] = planePosition.x;
	m_worldPlanes.PlanePosition[objectNumber - 1][1] = planePosition.y;
	m_worldPlanes.PlanePosition[objectNumber - 1][2] = planePosition.z;
	m_worldPlanes.PlanePosition[objectNumber - 1][3] = 1.0f;

	m_worldPlanes.PlaneDimensions[objectNumber - 1][0] = planeDimension.x;
	m_worldPlanes.PlaneDimensions[objectNumber - 1][1] = planeDimension.y;
	m_worldPlanes.PlaneDimensions[objectNumber - 1][2] = planeDimension.z;
	m_worldPlanes.PlaneDimensions[objectNumber - 1][3] = 1.0f;
	color.GetAsFloats(m_worldPlanes.PlaneColor[objectNumber - 1]);
	m_worldPlanes.TotalPlaneObject= (float)objectNumber;
}