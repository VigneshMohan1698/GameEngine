
#define WIN32_LEAN_AND_MEAN	
#include <windows.h>
#include "Engine/Renderer/RendererD12.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Image.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/EngineBuildPreferences.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/Clock.hpp"
#include "ThirdParty/D3D12DXR/d3dx12.h"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/RaytracingHelpers.hpp"
#include "Engine/Renderer/ResourceManager.hpp"
#include "Engine/Renderer/ShaderD12.hpp"
#include <dxgidebug.h>
#include <combaseapi.h>
#include <dxgi1_6.h>
#/*include <dxcapi.h>*/

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <minwindef.h>
#include <Engine/Math/VertexUtils.hpp>
#include "Engine/Renderer/ShadowMap.hpp"

#define SizeOfInUint32(obj) ((sizeof(obj) - 1) / sizeof(UINT32) + 1)

#ifdef ENGINE_DEBUG_RENDER
#pragma comment( lib, "ThirdParty/D3D12DXR/DirectXTK12_debug.lib" )
#else
#pragma comment( lib, "ThirdParty/D3D12DXR/DirectXTK12_release.lib" )
#endif // ENGINE_DEBUG_RENDER

#pragma comment( lib, "dxguid.lib" )
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxcompiler.lib")
//
//const unsigned int  RendererD12::MINECRAFTCHUNKS = 10;


namespace
{
	inline DXGI_FORMAT NoSRGB(DXGI_FORMAT fmt)
	{
		switch (fmt)
		{
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:   return DXGI_FORMAT_R8G8B8A8_UNORM;
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:   return DXGI_FORMAT_B8G8R8A8_UNORM;
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:   return DXGI_FORMAT_B8G8R8X8_UNORM;
		default:                                return fmt;

		}
	}
};


//-----------------------------CONSTRUCTORS--------------------------
RendererD12::RendererD12()
{
}
RendererD12::RendererD12(RendererD12Config const& rendererConfig)
{
	m_renderConfig = rendererConfig;
}

//-------------------------------MAIN FUNCTIONS-------------------------
void RendererD12::Startup()
{
#ifdef ENGINE_DEBUG_RENDER
	m_dxgiDebugModule = (void*) ::LoadLibraryA("dxgidebug.dll");
	typedef HRESULT(WINAPI* GetDebugModuleCB)(REFIID, void**);
	((GetDebugModuleCB) ::GetProcAddress((HMODULE)m_dxgiDebugModule, "DXGIGetDebugInterface"))(__uuidof(IDXGIDebug), (void**)&m_dxgiDebug);
#endif // ENGINE_DEBUG_RENDER

	D3D12InterfaceInitialization();
	if (m_renderingPipeline == RenderingPipeline::Rasterization)
	{
		InitializeRasterization();
	}
	Prepare();
}
void RendererD12::BeginFrame()
{
	
}
void RendererD12::ShutDown()
{
	WaitForGpu();

	for (int i = 0; i < m_loadedShaders.size(); i++)
	{
		delete m_loadedShaders[i];
		m_loadedShaders[i] = nullptr;
	}
	m_loadedShaders.clear();

	for (int i = 0; i < m_loadedTextures.size(); i++)
	{
		delete m_loadedTextures[i];
		m_loadedTextures[i] = nullptr;
	}
	m_loadedTextures.clear();

	m_fence.Reset();
	for (int i = 0; i < m_backBufferCount; i++)
	{
		m_RcommandAllocator[i]->Reset();
		m_backBufferRenderTarget[i].Reset();
	}

	delete m_resourceManager;
	m_resourceManager = nullptr;

	m_rtvDescriptorHeap.Reset();
	m_dsvDescriptorHeap.Reset();
	m_imguiDescriptorHeap.Reset();
	m_cameraCB.ResetResources();
	m_modelConstantsCB.ResetResources();
	m_descriptorHeap.Reset();
	m_descriptorsAllocated = 0;
	m_imguiDescriptorsAllocated = 0;
	m_adapter.Reset();
	m_dxgiFactory.Reset();

	m_depthStencilHandle.m_gpuBuffer->ResetResource();
	m_RcommandList.Reset();
	m_RswapChain.Reset();
	m_Rdevice.Reset();
	m_RcommandQueue.Reset();

#ifdef ENGINE_DEBUG_RENDER
	((IDXGIDebug*)m_dxgiDebug)->ReportLiveObjects(DXGI_DEBUG_ALL, (DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
	((IDXGIDebug*)m_dxgiDebug)->Release();
	m_dxgiDebug = nullptr;
	::FreeLibrary((HMODULE)m_dxgiDebugModule);
	m_dxgiDebugModule = nullptr;
#endif 
}
void RendererD12::EndFrame()
{
	FinishUpGPUWork();
	Present();
	MoveToNextFrame();
}


//----------------------------CAMERA FUNCTIONS-----------------------------
void RendererD12::BeginCamera(const Camera& camera)
{
	//m_currentCamera = camera;

	//Mat44 game = camera.GetViewToRenderMatrix();
	//Mat44 model = Mat44();
	//Mat44 proj = camera.GetProjectionMatrix();
	//Mat44 view = camera.GetViewMatrix();
	//Mat44 viewOrigin = camera.GetViewMatrixAtOrigin();
	//Mat44 invProj, invView, invViewOrigin;
	//if (!m_isFirstFrame)
	//{
	//	m_sceneCB->_cameraPosition = m_sceneCB->cameraPosition;
	//	m_sceneCB->_viewMatrix = m_sceneCB->viewMatrix;
	//}
	////------------GETTING INVERSE TO CONVERT FROM NDC TO WORLD--------------------
	//invProj = proj.GetNonOrthonormalInverse();
	//invView = view.GetNonOrthonormalInverse();
	//invViewOrigin = viewOrigin.GetNonOrthonormalInverse();
	//Vec3 ibasis, jbasis, kbasis;
	//m_sceneCB->inversedProjectionMatrix = invProj;
	//m_sceneCB->projectionMatrix = proj;
	//m_sceneCB->viewMatrix = view;
	//m_sceneCB->inversedViewMatrix = invView;
	//m_sceneCB->inversedViewMatrixOrigin = invViewOrigin;
	//m_sceneCB->cameraPosition = Vec4(camera.m_position, 1.0f);
	//m_sceneCB->lightPosition = m_lightPosition;
	//m_sceneCB->samplingData = m_gameValues.samplingData;
	//m_sceneCB->GIColor = m_gameValues.GIColor;
	//m_sceneCB->lightBools = m_gameValues.lightBools;
	//m_sceneCB->lightfallOff_AmbientIntensity_CosineSampling_DayNight = m_gameValues.lightfallOff_AmbientIntensity_CosineSampling_DayNight;
	//m_sceneCB->textureMappings = m_gameValues.textureMappings;
	//m_isFirstFrame = false;

}
void RendererD12::BeginRasterizerCamera(const Camera& camera, ShadowMap* shadowMap)
{
	m_currentCamera = camera;
	auto renderTarget = GetBackBufferCPUHandle();
	auto depthTarget = &m_depthStencilHandle.m_cpuDescriptorHandle;
	m_RcommandList->SetDescriptorHeaps(1, m_descriptorHeap.GetAddressOf());
	m_RcommandList->OMSetRenderTargets(1, renderTarget, FALSE, depthTarget);
	m_RcommandList->RSSetViewports(1, &m_screenViewport);
	m_RcommandList->RSSetScissorRects(1, &m_scissorRect);

	m_cameraCB->projectionMatrix = m_currentCamera.GetProjectionMatrix();
	m_cameraCB->viewMatrix = m_currentCamera.GetViewMatrix();
	Mat44 lightViewMatrix = Mat44();
	Mat44 lightProjectionMatrix = Mat44();

	if(shadowMap) 
	{
		lightViewMatrix = shadowMap->m_shadowCamera.GetViewMatrix();
		lightProjectionMatrix = shadowMap->m_shadowCamera.GetProjectionMatrix();
	}

	m_cameraCB->lightViewMatrix = lightViewMatrix;
	m_cameraCB->lightProjMatrix = lightProjectionMatrix;

	//m_cameraCB->cameraPosition = Vec4(m_currentCamera.m_position, 0.0f);
	m_gameDataCB->ViewX_GIOnY_ShadowPassZ_FrameTime.z = 0;
	m_cameraCB.CopyCputoGpu(m_frameIndex);
	m_gameDataCB.CopyCputoGpu(m_frameIndex);
}

void RendererD12::BeginShadowMapRender(ShadowMap* shadowMap)
{
	GpuBuffer* shadowBuffer = &shadowMap->m_shadowBuffer;
	auto shadowDepthDSVCPUDescriptorHandle = &shadowMap->m_shadowDepthStencilHandle.m_cpuDescriptorHandle;
	m_RcommandList->OMSetRenderTargets(0, nullptr, FALSE, shadowDepthDSVCPUDescriptorHandle);

	m_currentCamera = shadowMap->m_shadowCamera;
	m_currentCamera.SetTransform(shadowMap->m_shadowCamera.m_position, shadowMap->m_shadowCamera.m_orientation);
	m_RcommandList->RSSetViewports(1, shadowMap->GetViewport());
	m_RcommandList->RSSetScissorRects(1, shadowMap->GetScissorRect());

	m_cameraCB->projectionMatrix = m_currentCamera.GetProjectionMatrix();
	m_cameraCB->viewMatrix = m_currentCamera.GetViewMatrix();
	m_gameDataCB->ViewX_GIOnY_ShadowPassZ_FrameTime.z = 1;

	m_cameraCB.CopyCputoGpu(m_frameIndex);
	m_gameDataCB.CopyCputoGpu(m_frameIndex);

	m_fenceValues[m_frameIndex] = m_fenceValues[m_frameIndex] + 1;
}
void RendererD12::EndShadowMapRender(ShadowMap* shadowMap)
{
	FinishUpGPUWork();
	GpuBuffer* shadowBuffer = &shadowMap->m_shadowBuffer;

	//Transition shadow map from Depth write to Shader resource so we can sample it
	CD3DX12_RESOURCE_BARRIER depthTransitionBarrier = CD3DX12_RESOURCE_BARRIER::Transition(shadowBuffer->GetResource(),
		D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	m_RcommandList->ResourceBarrier(1, &depthTransitionBarrier);

	//Setting depth stencil back as the rendertarget
	auto depthTarget = &m_depthStencilHandle.m_cpuDescriptorHandle;
	m_RcommandList->OMSetRenderTargets(0, nullptr, FALSE, depthTarget);

}
void RendererD12::SetModelConstantData(Mat44 modelMatrix, Vec4 color, int index)
{
	m_modelConstantsCB->modelMatrix = modelMatrix;
	m_modelConstantsCB->color = Vec4(1.0f,0.0f,0.0f,0.0f);
	m_modelConstantsCB.CopyCputoGpu(index);
}

void RendererD12::EndCamera(const Camera& camera)
{
	UNUSED((void)camera);
}
void RendererD12::D3D12InterfaceInitialization()
{
	m_windowDimensions = m_renderConfig.m_window->GetClientDimensions();
	m_dimensions = m_renderConfig.m_window->GetClientDimensions();	
	//m_dimensions = IntVec2(1280, 720);
	//m_renderConfig.m_window->SetRenderTextureDimensions(m_dimensions);
	//m_dimensions = IntVec2(1920, 1080);
	InitializeAdapterAndCheckRaytracingSupport();

	//------------------CREATING THE DEVICE----------------------
	ThrowIfFailed(D3D12CreateDevice(m_adapter.Get(), m_d3dMinFeatureLevel, IID_PPV_ARGS(&m_Rdevice)), "Failed while Creating Device");

	//---------------COMMAND QUEUE-----------------
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	ThrowIfFailed(m_Rdevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_RcommandQueue)), "Failed While Creating Command Queue");


	//--------------RTV AND DSV DESCRIPTOR HEAPS----------------------------
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc = {};
	rtvDescriptorHeapDesc.NumDescriptors = m_backBufferCount;
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	ThrowIfFailed(m_Rdevice->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&m_rtvDescriptorHeap)), "Failed while creating descriptor heap");
	m_RrtvDescriptorSize = m_Rdevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC dsvDescriptorHeapDesc = {};
	dsvDescriptorHeapDesc.NumDescriptors = 10;
	dsvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	ThrowIfFailed(m_Rdevice->CreateDescriptorHeap(&dsvDescriptorHeapDesc, IID_PPV_ARGS(&m_dsvDescriptorHeap)), "Failed while creating DSV HEAP");


	//----------------CREATING COMMAND ALLOCATOR--------------
	for (UINT n = 0; n < m_backBufferCount; n++)
	{
		ThrowIfFailed(m_Rdevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_RcommandAllocator[n])), "Failed while creating command allocator");
	}


	//----------------CREATING COMMAND LIST----------------------
	ThrowIfFailed(m_Rdevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_RcommandAllocator[0].Get(), nullptr, IID_PPV_ARGS(&m_RcommandList)), "Failed while creating command list" );
	ThrowIfFailed(m_Rdevice->CreateFence(m_fenceValues[m_frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)), "Failed while creating fence");
	m_fenceValues[m_frameIndex]++;

	m_fenceEvent.Attach(CreateEvent(nullptr, FALSE, FALSE, nullptr));
	if (!m_fenceEvent.IsValid())
	{
		ThrowIfFailed(E_FAIL, "Failed while creating fence");
	}

	//// Wait until all previous GPU work is complete.
	/*WaitForGpu();*/

	for (UINT n = 0; n < m_backBufferCount; n++)
	{
		m_backBufferRenderTarget[n].Reset();
		m_fenceValues[n] = m_fenceValues[m_frameIndex];
	}
	DXGI_FORMAT backBufferFormat = NoSRGB(m_backBufferFormat);

	//----------------CREATING SWAP CHAIN-----------------------
	ComPtr<IDXGISwapChain1> swapChain;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = m_dimensions.x;
	swapChainDesc.Height = m_dimensions.y;
	swapChainDesc.Format = backBufferFormat;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = m_backBufferCount;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
	fsSwapChainDesc.Windowed = TRUE;
	ThrowIfFailed(m_dxgiFactory->CreateSwapChainForHwnd(
		m_RcommandQueue.Get(), (HWND)GetRenderConfig().m_window->GetHwnd(), &swapChainDesc, &fsSwapChainDesc,
		nullptr, &swapChain), "Failed While creating swap chain");

	ThrowIfFailed(swapChain.As(&m_RswapChain), "Failed While Creating Swap Chain");

	m_RswapChain->ResizeBuffers(
	m_backBufferCount,
	m_dimensions.x,
	m_dimensions.y,
	backBufferFormat,
	0
	);
	// This sample does not support fullscreen transitions.
	ThrowIfFailed(m_dxgiFactory->MakeWindowAssociation(HWND(GetRenderConfig().m_window->GetHwnd()), DXGI_MWA_NO_ALT_ENTER), "Failed While Making Full Screen Association");


	//-------------CREATING RENDER TARGETS----------------------
	for (UINT n = 0; n < m_backBufferCount; n++)
	{
		ThrowIfFailed(m_RswapChain->GetBuffer(n, IID_PPV_ARGS(&m_backBufferRenderTarget[n])), "Failed while getting render targets");

		wchar_t name[25] = {};
		swprintf_s(name, L"Render target %u", n);
		m_backBufferRenderTarget[n]->SetName(name);

		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = m_backBufferFormat;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;


		m_backBufferHeapCPUHandle[n] = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), n, m_RrtvDescriptorSize);
		m_rtvDescriptorsAllocated++;
		m_Rdevice->CreateRenderTargetView(m_backBufferRenderTarget[n].Get(), &rtvDesc, m_backBufferHeapCPUHandle[n]);
		float colorfloats[4];
		Rgba8::BLACK.GetAsFloats(colorfloats);
		//m_RcommandList->ClearRenderTargetView(m_backBufferHeapCPUHandle[n], colorfloats, 0, NULL);
	}

	//---------------CREATING DEPTH STENCIL BUFFER AND VIEW-----------------------
	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0 ;
	depthStencilDesc.Width = m_dimensions.x;
	depthStencilDesc.Height = m_dimensions.y;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_CLEAR_VALUE depthClear;
	depthClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthClear.DepthStencil.Depth = 1;
	depthClear.DepthStencil.Stencil = 0;

	m_depthStencilHandle.m_gpuBuffer = new GpuBuffer();
	CD3DX12_HEAP_PROPERTIES resource = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	ThrowIfFailed(m_Rdevice->CreateCommittedResource(&resource, D3D12_HEAP_FLAG_NONE, &depthStencilDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClear,
		IID_PPV_ARGS(&m_depthStencilHandle.m_gpuBuffer->m_resource)
	), "Failed while getting render targets");


	D3D12_DEPTH_STENCIL_VIEW_DESC depthDesc = {};
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthDesc.Flags = D3D12_DSV_FLAG_NONE;
	m_depthStencilHandle.m_usageState = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	UINT heapIndex = UINT_MAX;
	AllocateDepthDescriptor(&m_depthStencilHandle.m_cpuDescriptorHandle, heapIndex);
	m_Rdevice->CreateDepthStencilView(m_depthStencilHandle.m_gpuBuffer->GetResource(), &depthDesc, m_depthStencilHandle.m_cpuDescriptorHandle);


	//------------------TRANSITION RESOURCE TO BE USED AS DEPTH BUFFER---------------
	CD3DX12_RESOURCE_BARRIER depthTransitionBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_depthStencilHandle.m_gpuBuffer->GetResource(),
	D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	m_RcommandList->ResourceBarrier(1,&depthTransitionBarrier);

	//----------------SETTING THE VIEWPORT-----------------------

	m_screenViewport.TopLeftX = 0.0f;
	m_screenViewport.TopLeftY = 0.0f;
	m_screenViewport.Width = static_cast<float>(m_dimensions.x);
	m_screenViewport.Height = static_cast<float>(m_dimensions.y);
	m_screenViewport.MinDepth = 0.0f;
	m_screenViewport.MaxDepth = 1.0f;
	m_RcommandList->RSSetViewports(1, &m_screenViewport);
	m_scissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(m_dimensions.x), static_cast<LONG>(m_dimensions.y));

	//Closing the command list here after some initial commands
	ThrowIfFailed(m_RcommandList->Close(), "Failed while closing command list");

	m_frameIndex = m_RswapChain->GetCurrentBackBufferIndex();
	m_shaderCompiler = new ShaderCompiler();


	D3D12_DESCRIPTOR_HEAP_DESC heapdesc = {};
	heapdesc.NumDescriptors = 2000;
	heapdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapdesc.NodeMask = 0;
	ThrowIfFailed(m_Rdevice->CreateDescriptorHeap(&heapdesc, IID_PPV_ARGS(&m_imguiDescriptorHeap)), "Failed While Creating Descriptor Heap");
	heapdesc.NumDescriptors = 100;
	ThrowIfFailed(m_Rdevice->CreateDescriptorHeap(&heapdesc, IID_PPV_ARGS(&m_descriptorHeap)), "Failed While Creating Descriptor Heap");

	m_descriptorSize = m_Rdevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	m_depthDescriptorSize = m_Rdevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);


	//----------------CREATING RESOURCE MANAGER---------------------
	m_resourceManager = new ResourceManager();
}
void RendererD12::InitializeRasterization()
{
	//----------Creating camera Constant buffer---------------
	m_cameraCB = ConstantBufferHandle<CameraConstantBuffer>(m_backBufferCount);
	m_gameDataCB = ConstantBufferHandle<EngineDataBuffer>(m_backBufferCount);
	m_modelConstantsCB = ConstantBufferHandle<ModelConstantBuffer>(20);
}
void RendererD12::InitializeAdapterAndCheckRaytracingSupport()
{
	bool debugDXGI = false;

#if defined(ENGINE_DEBUG_RENDER)
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	// NOTE: Enabling the debug layer after device creation will invalidate the active device.
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
		}
		else
		{
			OutputDebugStringA("WARNING: Direct3D Debug Device is not available\n");
		}
		ComPtr<IDXGIInfoQueue>			 m_dxgiDebug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&m_dxgiDebug))))
		{
			debugDXGI = true;

			ThrowIfFailed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&m_dxgiFactory)), "Failed while creating factory");

			m_dxgiDebug->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
			m_dxgiDebug->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
		}
	}
#endif

	if (!debugDXGI)
	{
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory)), "Failed while trying to create factory");
	}
	IDXGIAdapter1** ppAdapter = &m_adapter;
	*ppAdapter = nullptr;

	ComPtr<IDXGIAdapter1> adapter;
	ComPtr<IDXGIFactory6> factory6;
	HRESULT hr = m_dxgiFactory.As(&factory6);

	if (FAILED(hr))
	{
		ERROR_AND_DIE("1.6 not supported");
	}
	for (UINT adapterID = 0; DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(adapterID, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)); ++adapterID)
	{
		if (m_adapterIDoverride != UINT_MAX && adapterID != m_adapterIDoverride)
		{
			continue;
		}

		DXGI_ADAPTER_DESC1 desc;
		ThrowIfFailed(adapter->GetDesc1(&desc), "Failed while getting adapter description");

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Don't select the Basic Render Driver adapter.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), m_d3dMinFeatureLevel, _uuidof(ID3D12Device), nullptr)))
		{
			m_adapterID = adapterID;
			m_adapterDescription = desc.Description;
#ifdef _DEBUG
			wchar_t buff[256] = {};
			swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", adapterID, desc.VendorId, desc.DeviceId, desc.Description);
			OutputDebugStringW(buff);
#endif
			break;
		}
	}

#if !defined(NDEBUG)
	if (!adapter && m_adapterIDoverride == UINT_MAX)
	{
		// Try WARP12 instead
		if (FAILED(m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&adapter))))
		{
			ERROR_AND_DIE("WARP12 not available. Enable the 'Graphics Tools' optional feature");
		}

		OutputDebugStringA("Direct3D Adapter - WARP12\n");
	}
#endif

	if (!adapter)
	{
		if (m_adapterIDoverride != UINT_MAX)
		{
			ERROR_AND_DIE("Unavailable adapter requested.");
		}
		else
		{
			ERROR_AND_DIE("Unavailable adapter.");
		}
	}

	*ppAdapter = adapter.Detach();
	factory6.Reset();
	adapter.Reset();
}
void RendererD12::FinishUpGPUWork()
{
	ExecuteCommandList(m_RcommandList);
	WaitForGpu();
	m_RcommandList->Reset(m_RcommandAllocator[m_frameIndex].Get(), nullptr);
}

void RendererD12::SerializeAndCreateRaytracingRootSignature(D3D12_ROOT_SIGNATURE_DESC& desc, ComPtr<ID3D12RootSignature>* rootSig)
{
	ID3DBlob* blob;
	ID3DBlob* error;
	HRESULT result;
	result = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error);
	m_Rdevice->CreateRootSignature(1, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&(*rootSig)));
}
void RendererD12::SerializeAndCreateRaytracingRootSignature(ID3D12Device5* device, D3D12_ROOT_SIGNATURE_DESC& desc, ComPtr<ID3D12RootSignature>* rootSig)
{
	ID3DBlob* blob;
	ID3DBlob* error;
	HRESULT result;
	result = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error);
	device->CreateRootSignature(1, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&(*rootSig)));
}

void RendererD12::Prepare()
{
	if(m_isRendererPrepared) {
		return;
	}
	m_isRendererPrepared = true;
	ThrowIfFailed(m_RcommandAllocator[m_frameIndex]->Reset(), "Failed while Resetting command allocator");
	if (m_currentShader == nullptr)
	{
		ThrowIfFailed(m_RcommandList->Reset(m_RcommandAllocator[m_frameIndex].Get(), nullptr), "Failed while Resetting command allocator");
	}
	else
	{
		ThrowIfFailed(m_RcommandList->Reset(m_RcommandAllocator[m_frameIndex].Get(), m_currentShader->m_pipelineStateObject.Get()), "Failed while Resetting command allocator");
	}
	
	m_beforeState = D3D12_RESOURCE_STATE_PRESENT;
	// Transition the render target into the correct state to allow for drawing into it.
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_backBufferRenderTarget[m_frameIndex].Get(), m_beforeState, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_RcommandList->ResourceBarrier(1, &barrier);
	//if (m_beforeState != D3D12_RESOURCE_STATE_RENDER_TARGET)
	//{
	//	// Transition the render target into the correct state to allow for drawing into it.
	//	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_backBufferRenderTarget[m_frameIndex].Get(), m_beforeState, D3D12_RESOURCE_STATE_RENDER_TARGET);
	//	m_RcommandList->ResourceBarrier(1, &barrier);
	//}
}
void RendererD12::Present()
{
	auto renderTarget = GetBackBuffer();
	// Transition the render target to the state that allows it to be presented to the display.
	if (m_renderingPipeline == RenderingPipeline::Raytracing)
	{
		D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_PRESENT);
		m_RcommandList->ResourceBarrier(1, &barrier);
	}
	else if (m_renderingPipeline == RenderingPipeline::Rasterization)
	{
		D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_RcommandList->ResourceBarrier(1, &barrier);
	}
	//------------EXECUTING COMMAND LIST------------------
	HRESULT commandListClosed = m_RcommandList->Close();
	if (FAILED(commandListClosed))
	{
		ERROR_AND_DIE("Failed While closing command list");
	}

	ID3D12CommandList* commandLists[] = { m_RcommandList.Get() };
	m_RcommandQueue->ExecuteCommandLists(ARRAYSIZE(commandLists), commandLists);

	HRESULT result;
	result = m_RswapChain->Present(0, 0);
	/*HRESULT result2 = m_Rdevice.Get()->GetDeviceRemovedReason();*/
	if (FAILED(result))
	{
		ERROR_AND_DIE("Failed while presenting");
	}

}
void RendererD12::MoveToNextFrame()
{
	// Schedule a Signal command in the queue.
	m_gpuWaitTime = (float)GetCurrentTimeSeconds();
	const UINT64 currentFenceValue = m_fenceValues[m_frameIndex];
	HRESULT result = m_RcommandQueue->Signal(m_fence.Get(), currentFenceValue);
	if (FAILED(result)) { ERROR_AND_DIE("Failed while moving to next Frame"); }

	// If the next frame is not ready to be rendered yet, wait until it is ready.
	if (m_fence->GetCompletedValue() < m_fenceValues[m_frameIndex])
	{
		result = m_fence->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_fenceEvent.Get());
		if (FAILED(result)) { ERROR_AND_DIE("Failed while moving to next Frame"); }
		WaitForSingleObjectEx(m_fenceEvent.Get(), INFINITE, FALSE);
	}
	// Update the back buffer index.
	m_frameIndex = m_RswapChain->GetCurrentBackBufferIndex();
	// Set the fence value for the next frame.
	m_fenceValues[m_frameIndex] = currentFenceValue + 1;

	//--------------CLEAR DYNAMIC RENDER ITEMS----------------
	for (int i = 0; i < m_dynamicRenderItems.size(); i++)
	{
		//RenderItems& item = m_dynamicRenderItems[i];
		//item.fenceValue = m_fenceValues[m_frameIndex];
		//HRESULT hr = m_Rdevice.Get()->CreateFence(item.fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&item.fence));
		//if (FAILED(hr)) {
		//	ERROR_AND_DIE("Failed while creating the Fence for Vertex buffer");
		//}
		//m_RcommandQueue->Signal(item.fence, item.fenceValue);
		//item.fence->SetEventOnCompletion(item.fenceValue, item.fenceEvent);
		//WaitForSingleObject(item.fenceEvent, INFINITE);

		m_dynamicRenderItems[i].verticesPNCUTB.ResetResources();
		m_dynamicRenderItems[i].verticesPCU.ResetResources();
		m_dynamicRenderItems[i].indices.ResetResources();
		//m_fenceValues[m_frameIndex] = item.fenceValue + 1;
	}
	m_dynamicRenderItems.clear();
	m_gpuWaitTime = (float)GetCurrentTimeSeconds() - m_gpuWaitTime;

	//Resources can't be created if command list and command allocator aren't reset. 
	//So in startup when textures/shaders are created the renderer needs to be prepared.
	//This bool prevents two prepares : startup->prepare, renderFrame->prepare, renderframe->movetonextframe
	m_isRendererPrepared = false;
}

TextureD12* RendererD12::GetTextureForFileNameOrPath(char const* fileName, const char* filePath)
{
	for (int i = 0; i < m_loadedTextures.size(); i++)
	{
		if (m_loadedTextures[i]->GetImageFileName() == fileName || m_loadedTextures[i]->GetImageFilePath() == filePath)
		{
			return m_loadedTextures[i];
		}
	}
	return nullptr;
}
TextureD12* RendererD12::GetTextureForFileName(char const* name)
{
	for (int i = 0; i < m_loadedTextures.size(); i++)
	{
		if (m_loadedTextures[i]->GetImageFileName() == name)
		{
			return m_loadedTextures[i];
		}
	}
	return nullptr;
}
TextureD12* RendererD12::CreateTextureFromImage(const Image& image)
{
	TextureD12* textureToBeCreated = new TextureD12();

	CD3DX12_RESOURCE_DESC textureDesc;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Width = image.GetDimensions().x;
	textureDesc.Height = image.GetDimensions().y;
	textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	textureDesc.Alignment =0;


	auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	HRESULT hr = m_Rdevice->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&textureToBeCreated->m_textureResource));

	if (FAILED(hr))
	{
		ERROR_AND_DIE("Failed to create commited resource.");
	}

	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(textureToBeCreated->m_textureResource.Get(), 0, 1);

	// Create the GPU upload buffer.
	heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);
	hr = m_Rdevice->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&textureToBeCreated->m_uploadResource));

	if (FAILED(hr))
	{
		ERROR_AND_DIE("Failed to create commited resource.");
	}

	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = image.GetRawData();
	textureData.RowPitch = image.GetDimensions().x * sizeof(Rgba8);
	textureData.SlicePitch = textureData.RowPitch * image.GetDimensions().y;

	UpdateSubresources(m_RcommandList.Get(), textureToBeCreated->m_textureResource.Get(), textureToBeCreated->m_uploadResource.Get(), 0, 0, 1, &textureData);
	auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(textureToBeCreated->m_textureResource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	m_RcommandList->ResourceBarrier(1, &resourceBarrier);

	// Get handle for srv desc heap
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	textureToBeCreated->m_heapIndex = AllocateDescriptor(&textureToBeCreated->m_cpuDescriptorHandle, textureToBeCreated->m_heapIndex);
	m_Rdevice->CreateShaderResourceView(textureToBeCreated->m_textureResource.Get(), &srvDesc, textureToBeCreated->m_cpuDescriptorHandle);
	textureToBeCreated->m_gpuDescriptorHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descriptorHeap->GetGPUDescriptorHandleForHeapStart(), textureToBeCreated->m_heapIndex, m_descriptorSize);

	// Debug Name
#ifdef _DEBUG
	textureToBeCreated->m_textureResource->SetName(L"Diffuse Texture 0");
#endif
	return textureToBeCreated;
}
TextureD12* RendererD12::CreateOrGetTextureFromFile(char const* fileName, char const* imageFilePath)
{
	//See if we already have this texture previously loaded
	TextureD12* existingTexture = GetTextureForFileNameOrPath(fileName, imageFilePath );
	if (existingTexture)
	{
		return existingTexture;
	}

	// Never seen this texture before!  Let's load it.
	TextureD12* newTexture = CreateTextureFromFile(imageFilePath);
	newTexture->m_name = fileName;
	return newTexture;
}

void RendererD12::CreateOrGetTextureFromFile(char const* fileName, char const* imageFilePath, int& textureIndex)
{
	//See if we already have this texture previously loaded
	TextureD12* existingTexture = GetTextureForFileNameOrPath(fileName, imageFilePath);
	if (existingTexture)
	{
		textureIndex = GetTextureIndex(existingTexture->m_name.c_str());
	}

	// Never seen this texture before!  Let's load it.
	TextureD12* newTexture = CreateTextureFromFile(imageFilePath);
	newTexture->m_name = fileName;
	textureIndex = (int)m_loadedTextures.size() - 1;
}
TextureD12* RendererD12::CreateTextureFromFile(char const* imageFilePath)
{
	if (!FileExists(imageFilePath))
	{
		std::string erroMessage = "File Does Not Exist !";
		ERROR_AND_DIE(erroMessage);
	}
	Image* textureImage = new Image(imageFilePath);
	TextureD12* texture = CreateTextureFromImage(*textureImage);
	//SetDebugName(texture->m_texture, imageFilePath);
	m_loadedTextures.push_back(texture);
	return texture;
}
TextureD12* RendererD12::GetTextureAtIndex(int index)
{	
	if(index > (int)m_loadedTextures.size() - 1)
	{
		return nullptr;
	}
	return m_loadedTextures[index];
}
int RendererD12::GetTextureIndex(char const* fileName)
{
	for (int i = 0; i < m_loadedTextures.size(); i++)
	{
		if(m_loadedTextures[i]->m_name == fileName)
		{
			return i;
		}
	}
	return -1;
}

void RendererD12::BindTexture(int index, TextureD12* textureToBind)
{
	m_RcommandList->SetGraphicsRootDescriptorTable(index, textureToBind->m_gpuDescriptorHandle);
}
void RendererD12::BindTexture(int bufferIndex, int textureIndex)
{
	TextureD12* textureToBind = GetTextureAtIndex(textureIndex);
	if(!textureToBind)
	{
		ERROR_AND_DIE("Texture that you are trying to bind does not exist");
	}
	m_RcommandList->SetGraphicsRootDescriptorTable(bufferIndex, textureToBind->m_gpuDescriptorHandle);
}
void RendererD12::BindComputeTexture(int index, TextureD12* textureToBind)
{
	m_RcommandList->SetComputeRootDescriptorTable(index, textureToBind->m_gpuDescriptorHandle);
	
}
void RendererD12::BindHandle(int index,D3D12_GPU_DESCRIPTOR_HANDLE& handle)
{
	m_RcommandList->SetGraphicsRootDescriptorTable(index, handle);
}

//----------------------------MAIN RENDER FUNCTIONS----------------------
void RendererD12::ClearScreen(Rgba8 color)
{
	auto renderTarget = GetBackBufferCPUHandle();
	auto depthTarget = &m_depthStencilHandle.m_cpuDescriptorHandle;
	m_RcommandList->SetDescriptorHeaps(1, m_descriptorHeap.GetAddressOf());
	m_RcommandList->OMSetRenderTargets(1, renderTarget, FALSE, depthTarget);
	// Then set the color to clear the window to.
	float colorfloats[4];
	color.GetAsFloats(colorfloats);

	m_RcommandList->ClearRenderTargetView(*renderTarget, colorfloats, 0, NULL);
	D3D12_CLEAR_FLAGS flags = D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL;
	m_RcommandList.Get()->ClearDepthStencilView(m_depthStencilHandle.m_cpuDescriptorHandle, flags, 1,0,0,nullptr);
}
void RendererD12::DrawVertexArray(int numberOfVertices, VertexNormalArray verticesToDraw)
{
	auto device = m_Rdevice.Get();
	auto cmdList = m_RcommandList.Get();
	const UINT vByteSize = sizeof(Vertex_PNCU) * numberOfVertices;
	RenderItems item;

	item.verticesPNCUTB.CreateDefaultBuffer(device, m_RcommandList.Get(), verticesToDraw.data(), vByteSize);


	//--------------CREATING VERTEX BUFFER VIEW------------
	D3D12_VERTEX_BUFFER_VIEW vbv = item.verticesPNCUTB.CreateAndGetVertexBufferView(vByteSize);

	cmdList->IASetVertexBuffers(0,1, &vbv);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->DrawInstanced(numberOfVertices , 1, 0, 0);


	ExecuteCommandList(cmdList);
	WaitForGpu();

	ThrowIfFailed(m_RcommandAllocator[m_frameIndex]->Reset(), "Failed while Resetting command allocator");
	ThrowIfFailed(m_RcommandList->Reset(m_RcommandAllocator[m_frameIndex].Get(), m_currentShader->m_pipelineStateObject.Get()), "Failed while Resetting command allocator");

	item.verticesPNCUTB.ResetResources();
	//m_RcommandList->Reset(m_rCommand);
}
void RendererD12::DrawIndexedVertexArray(int numberOfVertices, VertexNormalTangentArray& verticesToDraw, std::vector<unsigned int>& indexes)
{
	if (numberOfVertices == 0)
	{
		return;
	}
	auto device = m_Rdevice.Get();
	auto cmdList = m_RcommandList.Get();
	const UINT vByteSize = sizeof(Vertex_PNCUTB) * numberOfVertices;
	const UINT iByteSize = (UINT)(sizeof(unsigned int) * indexes.size()) ;
	RenderItems item;
	item.verticesPNCUTB.CreateDefaultBuffer(device, m_RcommandList.Get(), verticesToDraw.data(), vByteSize);
	item.indices.CreateDefaultBuffer(device, m_RcommandList.Get(), indexes.data(), iByteSize);

	//--------------CREATING VERTEX BUFFER VIEW------------
	D3D12_VERTEX_BUFFER_VIEW vbv = item.verticesPNCUTB.CreateAndGetVertexBufferView(vByteSize);
	D3D12_INDEX_BUFFER_VIEW ibv = item.indices.CreateAndGetIndexBufferView(iByteSize);
	cmdList->IASetVertexBuffers(0, 1, &vbv);
	cmdList->IASetIndexBuffer(&ibv);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->DrawIndexedInstanced(UINT(indexes.size()), 1, 0, 0, 0);
	m_dynamicRenderItems.push_back(item);
}
void RendererD12::DrawVertexArray(int numberOfVertices, VertexNormalTangentArray& verticesToDraw)
{
	if (numberOfVertices == 0)
	{
		return;
	}
	auto device = m_Rdevice.Get();
	auto cmdList = m_RcommandList.Get();
	const UINT vByteSize = sizeof(Vertex_PNCUTB) * numberOfVertices;

	RenderItems item;
	item.verticesPNCUTB.CreateDefaultBuffer(device, m_RcommandList.Get(), verticesToDraw.data(), vByteSize);

	//--------------CREATING VERTEX BUFFER VIEW------------
	D3D12_VERTEX_BUFFER_VIEW vbv = item.verticesPNCUTB.CreateAndGetVertexBufferView(vByteSize);
	cmdList->IASetVertexBuffers(0, 1, &vbv);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->DrawInstanced(numberOfVertices, 1, 0, 0);
	m_dynamicRenderItems.push_back(item);
}
void RendererD12::DrawVertexArray(int numberOfVertices, VertexArray& verticesToDraw)
{
	if (numberOfVertices == 0)
	{
		return;
	}
	auto device = m_Rdevice.Get();
	auto cmdList = m_RcommandList.Get();
	const UINT vByteSize = sizeof(Vertex_PCU) * numberOfVertices;

	RenderItems item;
	item.verticesPCU.CreateDefaultBuffer(device, m_RcommandList.Get(), verticesToDraw.data(), vByteSize);

	//--------------CREATING VERTEX BUFFER VIEW------------
	D3D12_VERTEX_BUFFER_VIEW vbv = item.verticesPCU.CreateAndGetVertexBufferView(vByteSize);
	cmdList->IASetVertexBuffers(0, 1, &vbv);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->DrawInstanced(numberOfVertices, 1, 0, 0);
	m_dynamicRenderItems.push_back(item);
}
void RendererD12::SetDepthStencilState(DepthTestD12 depthTest, bool writeDepth)
{
	D3D12_DEPTH_STENCIL_DESC& depthDesc = m_depthStencilHandle.m_depthDesc;
	depthDesc.DepthEnable = true;
	depthDesc.StencilEnable = true;
	if (writeDepth)
	{
		depthDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	}
	else
	{
		depthDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	}
	switch (depthTest)
	{
	case DepthTestD12::ALWAYS:
		depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		depthDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		depthDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		break;
	case DepthTestD12::NEVER:
		depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;
		depthDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
		depthDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
		break;
	case DepthTestD12::EQUAL:
		depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_EQUAL;
		depthDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;
		depthDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;
		break;
	case DepthTestD12::NOTEQUAL:
		depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_NOT_EQUAL;
		depthDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NOT_EQUAL;
		depthDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NOT_EQUAL;
		break;
	case DepthTestD12::LESS:
		depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		depthDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_LESS;
		depthDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_LESS;
		break;
	case DepthTestD12::LESSEQUAL:
		depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		depthDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		depthDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		break;
	case DepthTestD12::GREATER:
		depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
		depthDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_GREATER;
		depthDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_GREATER;
		break;
	case DepthTestD12::GREATEREQUAL:
		depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		depthDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		depthDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		break;
	default:
		break;
	}
	// Stencil operations if pixel is front-facing
	depthDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_INCR;
	depthDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;

	// Stencil operations if pixel is back-facing
	depthDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_DECR;
	depthDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;

	m_RcommandList.Get()->OMSetStencilRef(1);
}
//---------------------------GET FUNCTIONS----------------------
RendererD12Config  RendererD12::GetRenderConfig()
{
	return m_renderConfig;
}
IDXGISwapChain3* RendererD12::GetSwapChain()
{
	return m_RswapChain.Get();
}

void RendererD12::PopulateSimpleCubeVertices()
{	
	//Texture* chunkTexture = new Texture();
	//IntVec2 dimensions = IntVec2(2048,2048);
	//SpriteSheet m_mapSpriteSheet = SpriteSheet(*chunkTexture, dimensions, IntVec2(64, 64));

	//Vec3 bottomLeft, bottomRight, topLeft, topRight;
	//float minx, miny, minz, maxx, maxy, maxz;
	//AABB3 blockbounds;
	//Vec3 m_chunkPosition = Vec3();
	//UINT32 m_indexCount = 0;
	//std::vector<Vertex_PNCU>		m_vertexes;
	//std::vector<unsigned int>	m_indexes;

	//int WorldPosX = 0;
	//int WorldPosY = 0;
	//for (int WorldPosX = 0; WorldPosX < 5; WorldPosX++)
	//{
	//	for (int WorldPosY = 0; WorldPosY < 5; WorldPosY++)
	//	{
	//		for (int z = 0; z < 128; z++)
	//		{
	//			for (int y = 0; y < 16; y++)
	//			{
	//				for (int x = 0; x < 16; x++)
	//				{
	//					m_chunkPosition.x = WorldPosX * 16.0f;
	//					m_chunkPosition.y = WorldPosY * 16.0f;
	//					blockbounds = AABB3(Vec3(m_chunkPosition.x + (float)x , m_chunkPosition.y + (float)y , (float)z), Vec3(m_chunkPosition.x + x + 1.0f, m_chunkPosition.y + y + 1.0f, z + 1.0f));
	//					minx = blockbounds.m_mins.x;
	//					miny = blockbounds.m_mins.y;
	//					minz = blockbounds.m_mins.z;

	//					maxx = blockbounds.m_maxs.x;
	//					maxy = blockbounds.m_maxs.y;
	//					maxz = blockbounds.m_maxs.z;

	//					bottomLeft = Vec3(maxx, maxy, minz);
	//					bottomRight = Vec3(maxx, miny, minz);
	//					topLeft = Vec3(minx, maxy, minz);
	//					topRight = Vec3(minx, miny, minz);

	//					//------------------------------------FLOOR WALL ---------------------------------------
	//					AddVertsForIndexedQuad3D(m_DXRverts, m_DXRindexes, m_indexCount, topLeft, bottomLeft, bottomRight, topRight, Rgba8::WHITE,
	//						m_mapSpriteSheet.GetSpriteDef(Vec2(32, 34), IntVec2(64, 64)).GetUVs());
	//					m_indexCount += 4;

	//					bottomLeft = Vec3(maxx, miny, maxz);
	//					bottomRight = Vec3(maxx, maxy, maxz);
	//					topLeft = Vec3(minx, miny, maxz);
	//					topRight = Vec3(minx, maxy, maxz);

	//					//----------------------------------ROOF WALL-----------------------------------------------
	//					AddVertsForIndexedQuad3D(m_DXRverts, m_DXRindexes, m_indexCount, topLeft, bottomLeft, bottomRight, topRight, Rgba8::WHITE,
	//						m_mapSpriteSheet.GetSpriteDef(Vec2(32, 34), IntVec2(64, 64)).GetUVs());
	//					m_indexCount += 4;

	//					//----------------------LEFT WALL-------------------------------
	//					bottomLeft = Vec3(maxx, maxy, minz);
	//					bottomRight = Vec3(minx, maxy, minz);
	//					topLeft = Vec3(maxx, maxy, maxz);
	//					topRight = Vec3(minx, maxy, maxz);

	//					AddVertsForIndexedQuad3D(m_DXRverts, m_DXRindexes, m_indexCount, topLeft, bottomLeft, bottomRight, topRight, Rgba8::WHITE,
	//						m_mapSpriteSheet.GetSpriteDef(Vec2(33, 34), IntVec2(64, 64)).GetUVs());
	//					m_indexCount += 4;

	//					//-------------------------RIGHT WALL-----------------------------
	//					bottomLeft = Vec3(minx, miny, minz);
	//					bottomRight = Vec3(maxx, miny, minz);
	//					topLeft = Vec3(minx, miny, maxz);
	//					topRight = Vec3(maxx, miny, maxz);

	//					AddVertsForIndexedQuad3D(m_DXRverts, m_DXRindexes, m_indexCount, topLeft, bottomLeft, bottomRight, topRight, Rgba8::WHITE,
	//						m_mapSpriteSheet.GetSpriteDef(Vec2(33, 34), IntVec2(64, 64)).GetUVs());
	//					m_indexCount += 4;

	//					//------------------------FRONT  WALL---------------------------
	//					bottomLeft = Vec3(minx, maxy, minz);
	//					bottomRight = Vec3(minx, miny, minz);
	//					topLeft = Vec3(minx, maxy, maxz);
	//					topRight = Vec3(minx, miny, maxz);

	//					AddVertsForIndexedQuad3D(m_DXRverts, m_DXRindexes, m_indexCount, topLeft, bottomLeft, bottomRight, topRight, Rgba8::WHITE,
	//						m_mapSpriteSheet.GetSpriteDef(Vec2(33, 34), IntVec2(64, 64)).GetUVs());
	//					m_indexCount += 4;

	//					//----------------------BACK WALL------------------------------
	//					bottomLeft = Vec3(maxx, miny, minz);
	//					bottomRight = Vec3(maxx, maxy, minz);
	//					topLeft = Vec3(maxx, miny, maxz);
	//					topRight = Vec3(maxx, maxy, maxz);

	//					AddVertsForIndexedQuad3D(m_DXRverts, m_DXRindexes, m_indexCount, topLeft, bottomLeft, bottomRight, topRight, Rgba8::WHITE,
	//						m_mapSpriteSheet.GetSpriteDef(Vec2(33, 34), IntVec2(64, 64)).GetUVs());
	//					m_indexCount += 4;

	//				}
	//			}
	//		}
	//	}
	//}

	////Vertex vert;
	////VertexNormalArray pncus;
	////Vertex_PNCU pncu;
	////Index ind;
	////for (int i = 0; i < (int)m_vertexes.size(); i++)
	////{
	////	vert.position = m_vertexes[i].m_position;
	////	vert.normal = m_vertexes[i].m_normal;
	////	vert.uv = m_vertexes[i].m_uvTexCoords;
	////	vertices.push_back(vert);
	////}

	////for (int i = 0; i < (int)m_indexes.size(); i++)
	////{
	////	ind = (int)m_indexes[i];
	////	indexes.push_back(ind);
	////}


}
void RendererD12::AllocateUploadBuffer(ID3D12Device* pDevice, void* pData, UINT64 datasize, ID3D12Resource** ppResource, const wchar_t* resourceName = nullptr)
{
	auto uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(datasize);
	ThrowIfFailed(pDevice->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(ppResource)), "Failed while Creating Committed Resource");

	if (resourceName)
	{
		(*ppResource)->SetName(resourceName);
	}
	void* pMappedData;
	(*ppResource)->Map(0, nullptr, &pMappedData);
	memcpy(pMappedData, pData, datasize);
	(*ppResource)->Unmap(0, nullptr);
}
void RendererD12::AllocateUAVBuffer(ID3D12Device* pDevice, UINT64 bufferSize, ID3D12Resource** ppResource, D3D12_RESOURCE_STATES initialResourceState = D3D12_RESOURCE_STATE_COMMON, const wchar_t* resourceName = nullptr)
{
	auto uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	ThrowIfFailed(pDevice->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		initialResourceState,
		nullptr,
		IID_PPV_ARGS(ppResource)), "Failed while Creating UAV Buffer");
	if (resourceName)
	{
		(*ppResource)->SetName(resourceName);
	}
}
void RendererD12::ExecuteCommandList(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	ThrowIfFailed(commandList.Get()->Close(), "Failed While Closing Command List");
	ID3D12CommandList* commandLists[] = { commandList.Get() };
	m_RcommandQueue->ExecuteCommandLists(ARRAYSIZE(commandLists), commandLists);
}
void RendererD12::ExecuteCommandList(int commandsToExecute, ComPtr<ID3D12GraphicsCommandList> commandList)
{
	UNUSED((void )commandsToExecute);
	//ThrowIfFailed(commandList.Get()->Close(), "Failed While Closing Command List");
	ID3D12CommandList* commandLists[] = { commandList.Get() };
	m_RcommandQueue->ExecuteCommandLists(ARRAYSIZE(commandLists), commandLists);
}
void RendererD12::WaitForGpu()
{
	if (m_RcommandQueue && m_fence && m_fenceEvent.IsValid())
	{
		UINT64 fenceValue = m_fenceValues[m_frameIndex];
		if (SUCCEEDED(m_RcommandQueue->Signal(m_fence.Get(), fenceValue)))
		{
			if (SUCCEEDED(m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent.Get())))
			{
				WaitForSingleObjectEx(m_fenceEvent.Get(), INFINITE, FALSE);
				// Increment the fence value for the current frame.
				m_fenceValues[m_frameIndex]++;
			}
		}
	}
}
void  RendererD12::ThrowIfFailed(HRESULT hr, const char* msg)
{
	if (FAILED(hr))
	{
		ERROR_AND_DIE(msg);
	}
}
void  RendererD12::ThrowIfFalse(bool result, const char* msg)
{
	if (result == false)
	{
		ERROR_AND_DIE(msg);
	}
}

//----------------------------SHADERS AND SHADER COMPILATIONS----------------------
//-----------------------Shader compiler functions-------------
ShaderCompiler::ShaderCompiler()
{
	HRESULT hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&m_utils));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&m_compiler));

	hr = m_utils->CreateIncludeHandler(&m_handler);
}
ShaderCompiler::~ShaderCompiler()
{
	m_compiler.Get()->Release();
	m_utils.Get()->Release();
	m_handler.Get()->Release();
}
ComPtr<IDxcBlob> ShaderCompiler::Compile(const char* ShaderFilePath)
{
	wchar_t wtext[40];
	size_t size = strlen(ShaderFilePath) + 1;
	mbstowcs_s(&size, wtext, ShaderFilePath, strlen(ShaderFilePath) + 1);//Plus null
	LPWSTR ptr = wtext;

	uint32_t codePage = CP_UTF8;
	ComPtr<IDxcBlobEncoding> sourceBlob;
	HRESULT hr = m_utils->CreateBlobFromFile(ptr, &codePage, &sourceBlob);
	//if(FAILED(hr)) Handle file loading error...

	ComPtr<IDxcOperationResult> result;
	hr = m_compiler->Compile(
		sourceBlob.Get(), // pSource
		wtext, // pSourceName
		NULL, // pEntryPoint
		L"lib_6_3", // pTargetProfile
		NULL, 0, // pArguments, argCount
		NULL, 0, // pDefines, defineCount
		m_handler.Get(), // pIncludeHandler
		&result); // ppResult
	if (SUCCEEDED(hr))
		result->GetStatus(&hr);

	if (result)
	{
		ComPtr<IDxcBlobEncoding> errorsBlob;
		hr = result->GetErrorBuffer(&errorsBlob);
		if (FAILED(hr) && errorsBlob)
		{
			DebuggerPrintf("Compilation failed with errors:\n%hs\n",
				(const char*)errorsBlob->GetBufferPointer());
		}
		else
		{
			DebuggerPrintf("Shader Compiled successfully");
		}
	}
	// Handle compilation error...
	ComPtr<IDxcBlob> code;
	result->GetResult(&code);
	return code;
}
ComPtr<ID3DBlob> ShaderCompiler::CompileVsPs(const char* filePath, const D3D_SHADER_MACRO* defines, const std::string& entryPoint, const std::string& target)
{
	UINT compileFlags = 0;
#if defined(ENGINE_DEBUG_RENDER)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ComPtr<ID3DBlob> byteCode = nullptr;
	ComPtr<ID3DBlob> errors;

	wchar_t wtext[80];
	size_t size = strlen(filePath) + 1;
	mbstowcs_s(&size, wtext, filePath, strlen(filePath) + 1);//Plus null
	HRESULT hr = D3DCompileFromFile(wtext, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

	if (FAILED(hr))
	{
		DebuggerPrintf("Compilation failed with errors:\n%hs\n",
			(const char*)errors->GetBufferPointer());
	}
	else
	{
		DebuggerPrintf("Shader Compiled successfully");
	}
	//if (errors != nullptr)
	//{
	//	ERROR_AND_DIE("Failed While Compiling Shader D12");
	//}
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Failed While Compiling Shader D12");
	}
	return byteCode;


	//wchar_t entryTex[40];
	//size_t entrySize = strlen(entryPoint.c_str()) + 1;
	//mbstowcs_s(&entrySize, entryTex, entryPoint.c_str(), strlen(entryPoint.c_str()) + 1);//Plus null
	//LPWSTR entry = entryTex;

	//wchar_t targetText[40];
	//size_t targetSize = strlen(target.c_str()) + 1;
	//mbstowcs_s(&targetSize, targetText, target.c_str(), strlen(target.c_str()) + 1);//Plus null
	//LPWSTR targetLPWSTR = targetText;
	//
	//uint32_t codePage = CP_UTF8;
	//ComPtr<IDxcBlobEncoding> sourceBlob;
	//HRESULT hre = m_utils->CreateBlobFromFile(ptr, &codePage, &sourceBlob);
	//if (!SUCCEEDED(hre))
	//{
	//	ERROR_AND_DIE("Failed while Creating source blob");
	//}
	//ComPtr<IDxcOperationResult> result;
	//HRESULT hr = m_compiler->Compile(
	//	sourceBlob.Get(), // pSource
	//	wtext, // pSourceName
	//	entry, // pEntryPoint
	//	targetLPWSTR, // pTargetProfile
	//	NULL, 0, // pArguments, argCount
	//	NULL, 0, // pDefines, defineCount
	//	m_handler.Get(), // pIncludeHandler
	//	&result); // ppResult

	//if (hr == E_FAIL)
	//{
	//	ERROR_AND_DIE("Failed while compiling Shader");
	//}
	//if (SUCCEEDED(hr))
	//	result->GetStatus(&hr);

	//if (result)
	//{
	//	ComPtr<IDxcBlobEncoding> errorsBlob;
	//	hr = result->GetErrorBuffer(&errorsBlob);
	//	if (FAILED(hr) && errorsBlob)
	//	{
	//		DebuggerPrintf("Compilation failed with errors:\n%hs\n",
	//			(const char*)errorsBlob->GetBufferPointer());
	//	}
	//	else
	//	{
	//		DebuggerPrintf("Shader Compiled successfully");
	//	}
	//}
	//ComPtr<IDxcBlob> code;
	//result->GetResult(&code);
	//return code;
}
ComPtr<IDxcBlob> ShaderCompiler::CompileComputeShader(const char* ShaderFilePath)
{
	wchar_t wtext[40];
	size_t size = strlen(ShaderFilePath) + 1;
	mbstowcs_s(&size, wtext, ShaderFilePath, strlen(ShaderFilePath) + 1);//Plus null
	LPWSTR ptr = wtext;

	uint32_t codePage = CP_UTF8;
	ComPtr<IDxcBlobEncoding> sourceBlob;
	HRESULT hr = m_utils->CreateBlobFromFile(ptr, &codePage, &sourceBlob);
	//if(FAILED(hr)) Handle file loading error...

	ComPtr<IDxcOperationResult> result;
	hr = m_compiler->Compile(
		sourceBlob.Get(), // pSource
		wtext, // pSourceName
		L"main", // pEntryPoint
		L"cs_6_3", // pTargetProfile
		NULL, 0, // pArguments, argCount
		NULL, 0, // pDefines, defineCount
		m_handler.Get(), // pIncludeHandler
		&result); // ppResult
	if (SUCCEEDED(hr))
		result->GetStatus(&hr);

	if (result)
	{
		ComPtr<IDxcBlobEncoding> errorsBlob;
		hr = result->GetErrorBuffer(&errorsBlob);
		if (FAILED(hr) && errorsBlob)
		{
			DebuggerPrintf("Compilation failed with errors:\n%hs\n",
				(const char*)errorsBlob->GetBufferPointer());
		}
		else
		{
			DebuggerPrintf("Shader Compiled successfully");
		}
	}
	ComPtr<IDxcBlob> code;
	result->GetResult(&code);
	return code;
}
IDxcBlob* ShaderCompiler::Compile(IDxcBlobEncoding* sourceBlob, LPCWSTR* args, unsigned long long numargs)
{
	UNUSED((void)args);
	UNUSED((void)numargs);
	UNUSED((void)sourceBlob);
	//DxcBuffer buffer;
	//buffer.Ptr = sourceBlob->GetBufferPointer();
	//buffer.Size = sourceBlob->GetBufferSize();
	//buffer.Encoding = DXC_CP_ACP;

	//HRESULT hr{ S_OK };
	//ComPtr<IDxcResult> result;
	//hr = m_compiler->Compile(&buffer, args, numargs, m_handler.Get(), IID_PPV_ARGS(&result));

	//ComPtr<IDxcBlobUtf8> pErrors = nullptr;
	//result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&pErrors), nullptr);

	//if (pErrors && pErrors->GetStringLength())
	//{
	//	DebuggerPrintf("\n Shader Compilation error : \n");
	//	DebuggerPrintf(pErrors->GetStringPointer());
	//}
	//else
	//{
	//	DebuggerPrintf("Shader Compiled Successfully");
	//}
	//HRESULT status{ S_OK };
	//hr = result->GetStatus(&status);

	////if (FAILED(hr) || (FAILED(status))) { ERROR_AND_DIE("Shader Compilation Failed"); }

	//ComPtr<IDxcBlob> finalShaderOutput{ nullptr };
	//hr = result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&finalShaderOutput), nullptr);

	//if (FAILED(hr)) { ERROR_AND_DIE("Shader Compilation Failed") };

	//return finalShaderOutput.Detach();
	return nullptr;
}

ShaderD12* RendererD12::CreateOrGetShader(const char* shaderName, const char* shaderFilePath,bool isCompute, bool containsTesselation, bool isShadowShader)
{
	for (int i = 0; i < m_loadedShaders.size(); i++)
	{
		if (m_loadedShaders[i]->m_config.m_name == shaderName)
		{
			return m_loadedShaders[i];
		}
	}
	ShaderConfigD12 shaderConfig;
	shaderConfig.m_name = shaderName;
	shaderConfig.m_shaderFilePath = shaderFilePath;
	shaderConfig.m_isTesselated = containsTesselation;

	ShaderD12* shaderD12 = new ShaderD12(shaderConfig, this);

	if (isCompute)
	{
		shaderD12->CreateComputeShaderObjects();
	}
	else
	{
		shaderD12->CreateShaderObjects();
	}

	m_loadedShaders.push_back(shaderD12);
	return shaderD12;
}
ShaderD12* RendererD12::GetShader(const char* shaderName)
{
	for (int i = 0; i < m_loadedShaders.size(); i++)
	{
		if (m_loadedShaders[i]->m_config.m_name == shaderName)
		{
			return m_loadedShaders[i];
		}
	}
	
	return nullptr;
}

void RendererD12::BindComputeShader(ShaderD12* shader)
{
	auto commandList = m_RcommandList.Get();
	commandList->SetDescriptorHeaps(1, m_descriptorHeap.GetAddressOf());
	commandList->SetComputeRootSignature(shader->m_rootSignature.Get());
	commandList->SetPipelineState(shader->m_pipelineStateObject.Get());
}
void RendererD12::DispatchComputeShader(int x, int y , int z, ShaderD12* shader)
{
	auto commandList = m_RcommandList.Get();
	commandList->Dispatch(x,y,z);
}
void RendererD12::BindShader(ShaderD12* shader, bool isForShadowMap)
{
	auto commandList = m_RcommandList.Get();
	m_currentShader = shader;
	commandList->SetDescriptorHeaps(1, m_descriptorHeap.GetAddressOf());
	commandList->SetGraphicsRootSignature(shader->m_rootSignature.Get());
	if (shader->GetShaderType() == ShaderDetails::Shader3D)
	{
		commandList->SetGraphicsRootConstantBufferView((UINT)Default3DRootSignatureParams::CameraConstantBuffer, m_cameraCB.GpuVirtualAddress(m_frameIndex));
		//commandList->SetGraphicsRootConstantBufferView((UINT)Default3DRootSignatureParams::ModelConstantBufferD12, m_modelConstantsCB.GpuVirtualAddress(m_frameIndex));
		commandList->SetGraphicsRootConstantBufferView((UINT)Default3DRootSignatureParams::GameConstantBuffer, m_gameDataCB.GpuVirtualAddress(m_frameIndex));
	}
	else if (shader->GetShaderType() == ShaderDetails::PBRShader3D)
	{
		commandList->SetGraphicsRootConstantBufferView((UINT)PBRRootSignatureParams::CameraConstantBuffer, m_cameraCB.GpuVirtualAddress(m_frameIndex));
		commandList->SetGraphicsRootConstantBufferView((UINT)PBRRootSignatureParams::GameConstantBuffer, m_gameDataCB.GpuVirtualAddress(m_frameIndex));
	}
	else
	{
		commandList->SetGraphicsRootConstantBufferView((UINT)DefaultRootSignatureParams::CameraConstantBuffer, m_cameraCB.GpuVirtualAddress(m_frameIndex));
	}

	if (isForShadowMap)
	{
		commandList->SetPipelineState(shader->m_shadowMapPSO.Get());
	}
	else
	{
		commandList->SetPipelineState(shader->m_pipelineStateObject.Get());
	}
}
BitmapFont* RendererD12::CreateBitmapFont(const char* fontFilePath)
{
	std::string fontFilePathWithExtension = std::string(fontFilePath) + ".png";
	BitmapFont* newFont = new BitmapFont(fontFilePath, IntVec2(256,256));
	return newFont;
}

//-----------------------------GET FUNCTIONS-------------------------------
ID3D12Device* RendererD12::GetDevice()
{
	return m_Rdevice.Get();
}
ComPtr<ID3D12GraphicsCommandList> RendererD12::GetCommandListComPtr()
{
	return m_RcommandList;
}
ID3D12GraphicsCommandList* RendererD12::GetCommandList()
{
	return m_RcommandList.Get();
}
ID3D12Resource* RendererD12::GetBackBuffer()
{
	return m_backBufferRenderTarget[m_frameIndex].Get();
}
D3D12_CPU_DESCRIPTOR_HANDLE* RendererD12::GetBackBufferCPUHandle()
{
	return &m_backBufferHeapCPUHandle[m_frameIndex];
}
D3D12_CPU_DESCRIPTOR_HANDLE RendererD12::GetDepthStencilViewHandle()
{
	return m_dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
}
ID3D12CommandAllocator* RendererD12::GetCommandAllocator()
{
	return m_RcommandAllocator[m_frameIndex].Get();
}
void RendererD12::ResetCommandAllocator()
{
	auto commandList = m_RcommandList.Get();
	auto commandAllocator = m_RcommandAllocator[m_frameIndex].Get();
	commandList->Reset(commandAllocator, nullptr);
}

D3D12_CPU_DESCRIPTOR_HANDLE RendererD12::CreateAndGetImGuiCPUDescriptorHandle()
{
	/*AllocateDescriptor(m_imguiDescriptorHeap.Get(), &m_imGUICpuHandle, 0);*/
	return m_imGUICpuHandle;
}
D3D12_CPU_DESCRIPTOR_HANDLE RendererD12::GetIMGUIRenderTarget()
{
	return m_ImguirenderTargetViewHandle;
}
D3D12_GPU_DESCRIPTOR_HANDLE RendererD12::GetGPUDescriptorHandle(ID3D12DescriptorHeap* heap, int offsetindex)
{
	CD3DX12_GPU_DESCRIPTOR_HANDLE handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(heap->GetGPUDescriptorHandleForHeapStart(), offsetindex, m_descriptorSize);
	return handle;
}
ID3D12DescriptorHeap* RendererD12::GetDescriptorHeap()
{
	return m_descriptorHeap.Get();
}
ResourceManager* RendererD12::GetResourceManager()
{
	return m_resourceManager;
}
ID3D12DescriptorHeap* RendererD12::GetIMGUIDescriptorHeap()
{
	return m_imguiDescriptorHeap.Get();
}

int RendererD12::GetFrameIndex()
{
	return m_frameIndex;
}

void RendererD12::CreateTextureSRV(TextureD12* texture)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = texture->m_textureSRVDimension;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = texture->m_textureResource->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = texture->m_textureResource->GetDesc().MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	texture->m_heapIndex = AllocateDescriptor(&texture->m_cpuDescriptorHandle, texture->m_heapIndex);
	m_Rdevice->CreateShaderResourceView(texture->m_textureResource.Get(), &srvDesc, texture->m_cpuDescriptorHandle);
	texture->m_gpuDescriptorHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descriptorHeap->GetGPUDescriptorHandleForHeapStart(), texture->m_heapIndex, m_descriptorSize);
}
UINT RendererD12::AllocateImguiDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE* cpuDescriptor, UINT descriptorIndexToUse)
{
	auto descriptorHeapCpuBase = m_imguiDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	if (descriptorIndexToUse >= m_imguiDescriptorHeap->GetDesc().NumDescriptors)
	{
		descriptorIndexToUse = m_imguiDescriptorsAllocated++;
	}
	*cpuDescriptor = CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptorHeapCpuBase, descriptorIndexToUse, m_RrtvDescriptorSize);
	return descriptorIndexToUse;
}
UINT RendererD12::AllocateDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE* cpuDescriptor, UINT descriptorIndexToUse)
{
	auto descriptorHeapCpuBase = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	UINT numDesc = m_descriptorHeap->GetDesc().NumDescriptors;
	if (descriptorIndexToUse >= numDesc)
	{
		descriptorIndexToUse = m_descriptorsAllocated++;
	}
	*cpuDescriptor = CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptorHeapCpuBase, descriptorIndexToUse, m_descriptorSize);
	return descriptorIndexToUse;
}
UINT RendererD12::AllocateDepthDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE* cpuDescriptor, UINT descriptorIndexToUse)
{
	auto descriptorHeapCpuBase = m_dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	UINT numDepthDescriptors = m_dsvDescriptorHeap->GetDesc().NumDescriptors;
	if (descriptorIndexToUse >= numDepthDescriptors)
	{
		descriptorIndexToUse = m_depthDescriptorsAllocated++;
	}
	*cpuDescriptor = CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptorHeapCpuBase, descriptorIndexToUse, m_depthDescriptorSize);
	return descriptorIndexToUse;
}
UINT RendererD12::AllocateRenderTargetDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE* cpuDescriptor, UINT descriptorIndexToUse)
{
	auto descriptorHeapCpuBase = m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	UINT numDescriptors = m_rtvDescriptorHeap->GetDesc().NumDescriptors;
	if (descriptorIndexToUse >= numDescriptors)
	{
		descriptorIndexToUse = m_rtvDescriptorsAllocated++;
	}
	*cpuDescriptor = CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptorHeapCpuBase, descriptorIndexToUse, m_depthDescriptorSize);
	return descriptorIndexToUse;
}
UINT RendererD12::AllocateDescriptor(ID3D12DescriptorHeap* heap, D3D12_CPU_DESCRIPTOR_HANDLE* cpuDescriptor, UINT& allocatedDescriptors, UINT descriptorIndexToUse)
{
	auto descriptorHeapCpuBase = heap->GetCPUDescriptorHandleForHeapStart();
	if (descriptorIndexToUse >= heap->GetDesc().NumDescriptors)
	{
		descriptorIndexToUse = allocatedDescriptors++;
	}
	*cpuDescriptor = CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptorHeapCpuBase, descriptorIndexToUse, m_descriptorSize);
	return descriptorIndexToUse;
}


void RendererD12::RenderFrame()
{
	//// Record all the commands we need to render the scene into the command list.
	//PopulateCommandList();
	//// Execute the command list.
	//ID3D12CommandList* ppCommandLists[] = { m_RcommandList.Get() };
	//m_RcommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	//// Present the frame.
	//ThrowIfFailed(m_RswapChain->Present(1, 0), "Failed While Presenting");

	//WaitForPreviousFrame();
}
void RendererD12::PopulateCommandList()
{
	//ThrowIfFailed(m_RcommandAllocator->Reset(), "Failed While Resetting Command Allocator");
	//ThrowIfFailed(m_RcommandList->Reset(m_RcommandAllocator.Get(), m_RpipelineState.Get()), "Failed While Resetting command List");

	////SET NECESSARY STATES
	//m_RcommandList->SetGraphicsRootSignature(m_RrootSignature.Get());
	//m_RcommandList->RSSetViewports(1, &m_Rviewport);
	//m_RcommandList->RSSetScissorRects(1, &m_RscissorRect);

	////INDICATING BACK BUFFER TO BE USED AS RENDER TARGET
	//CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_RrenderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	//m_RcommandList->ResourceBarrier(1, &barrier);

	//CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_RrtvDescriptorSize);
	//m_RcommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	//const float ClearColor[] = {0.0f,0.2f,0.1f,1.0f};
	//m_RcommandList->ClearRenderTargetView(rtvHandle, ClearColor, 0, nullptr);
	//m_RcommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//m_RcommandList->IASetVertexBuffers(0, 1, &m_RvertexBufferView);
	//m_RcommandList->DrawInstanced(3, 1, 0, 0);

	////INDICATE BACK BUFFER TO BE PRESENTED
	//CD3DX12_RESOURCE_BARRIER barrier2 = CD3DX12_RESOURCE_BARRIER::Transition(m_RrenderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	//m_RcommandList->ResourceBarrier(1, &barrier2);
	//ThrowIfFailed(m_RcommandList->Close(), "Failed While Closing Command List");
}

void RendererD12::GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
{
	ComPtr<IDXGIAdapter1> adapter;
	*ppAdapter = nullptr;

	for (UINT adapterIndex = 0; pFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND; ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Don't select the Basic Render Driver adapter.
			// If you want a software adapter, pass in "/warp" on the command line.
			continue;
		}
		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}
	*ppAdapter = adapter.Detach();
}
inline void GetAssetsPath(_Out_writes_(pathSize) WCHAR* path, UINT pathSize)
{
	if (path == nullptr)
	{
		throw std::exception();
	}

	DWORD size = GetModuleFileName(nullptr, path, pathSize);
	if (size == 0 || size == pathSize)
	{
		// Method failed or path was truncated.
		throw std::exception();
	}

	WCHAR* lastSlash = wcsrchr(path, L'\\');
	if (lastSlash)
	{
		*(lastSlash + 1) = L'\0';
	}
}
std::wstring RendererD12::GetAssetFullPath(LPCWSTR assetName)
{
	std::wstring m_assetsPath = L"Run/Data/Shaders";
	WCHAR assetsPath[512];
	GetAssetsPath(assetsPath, _countof(assetsPath));
	m_assetsPath = assetsPath;
	return m_assetsPath + assetName;
}

//---------------------------------IMGUI-----------------------------
void RendererD12::CreateIMGUIRenderTarget()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc = {};
	rtvDescriptorHeapDesc.NumDescriptors = m_backBufferCount;
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	ThrowIfFailed(m_Rdevice->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&m_imguiDescriptorHeap)), "Failed while creating descriptor heap");
	/*for (UINT n = 0; n < m_backBufferCount; n++)
	{
		ThrowIfFailed(m_RswapChain->GetBuffer(n, IID_PPV_ARGS(&m_imguiBackBufferRenderTarget[n])), "Failed while getting render targets");

		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = m_backBufferFormat;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvDescriptor(m_imguiDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), n, m_RrtvDescriptorSize);
		m_Rdevice->CreateRenderTargetView(m_imguiBackBufferRenderTarget[n].Get(), &rtvDesc, rtvDescriptor);
	}*/

}
void RendererD12::SetupImGuiRenderTarget()
{
	auto device = m_Rdevice.Get();
	auto commandList = m_RcommandList.Get();
	unsigned int renderTargetViewDescriptorSize;
	float color[4];
	m_ImguirenderTargetViewHandle = m_imguiDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	renderTargetViewDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	if (m_frameIndex == 0)
	{
		m_ImguirenderTargetViewHandle.ptr += renderTargetViewDescriptorSize;
	}

	// Set the back buffer as the render target.
	commandList->OMSetRenderTargets(m_frameIndex, &m_ImguirenderTargetViewHandle, FALSE, NULL);
	// Then set the color to clear the window to.
	color[0] = 0.5;
	color[1] = 0.5;
	color[2] = 0.5;
	color[3] = 1.0;
	commandList->ClearRenderTargetView(m_ImguirenderTargetViewHandle, color, 0, NULL);
}

//-------------------------------MESH FUNCTIONS----------------------
Mesh* RendererD12::GetMesh(const char* filePath)
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
Mesh* RendererD12::CreateMesh(const char* filePath)
{
	Mesh* mesh = new Mesh();
	MeshImportOptions options;
	mesh->ImportFromOBJFile(filePath, options);
	m_loadedMeshes.push_back(mesh);
	return mesh;
}
Mesh* RendererD12::CreateOrGetMesh(const char* filePath)
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

	Mesh* mesh = CreateMesh(filePath);
	return mesh;
}
Mesh* RendererD12::GetMeshForName(const char* name)
{
	for (int i = 0; i < m_loadedMeshes.size(); i++)
	{
		if (m_loadedMeshes[i]->m_modelName == name)
		{
			return m_loadedMeshes[i];
		}
	}

	return nullptr;
}
Mesh* RendererD12::GetMeshAtIndex(int index)
{
	return m_loadedMeshes[index];
}
Mesh* RendererD12::CreateMeshFromSavedFile(const char* filePath)
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

	Mesh* mesh = new Mesh();
	mesh->Load(filePath);
	mesh->m_filePath = filePath;
	m_loadedMeshes.push_back(mesh);
	return mesh;
}
