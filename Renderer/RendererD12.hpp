#pragma once	

#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Core/VertexDefinitions.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/Core/Image.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/RaytracingHelpers.hpp"
#include "Engine/Renderer/TextureD12.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
//#include "ThirdParty/D3D12DXR/DDSTextureLoader.h"
#include "Engine/Core/Model.hpp"
#include <Engine/EngineData.hpp>

#include <d3d12.h>
#include <dxgi1_4.h>
#include <vector>
#include <wrl.h>
#include <string.h>
#include <stdlib.h>
#include "ThirdParty/D3D12DXR/WICTextureLoader.h"
#include "ThirdParty/D3D12DXR/d3dx12.h"
#include "RenderResources/RenderBuffers.hpp"

using namespace Microsoft::WRL;

class ResourceManager;
class Denoiser;
class ShadowMap;
class ShaderD12;
class GlobalIllumination;
class Composition;
class PostProcess;
struct IDxcBlob;
struct IDxcBlobEncoding;
struct IDxcCompiler;
struct IDxcLibrary;
struct IDxcUtils;
struct IDxcIncludeHandler;
struct IDXGIDebug;

struct RenderItems
{
	VertexBufferD12<Vertex_PNCUTB> verticesPNCUTB;
	VertexBufferD12<Vertex_PCU> verticesPCU;
	IndexBufferD12<unsigned int> indices;
	ID3D12Fence* fence;
	UINT64 fenceValue = 1;
	HANDLE fenceEvent;
};

enum class RenderingPipeline
{
	Rasterization,
	Raytracing
};

enum class CompiledShaderByteCodes
{
	Raytracing3D = 0,
	GaussianFilter,
	Count
};

enum class DefaultRootSignatureParams {

	//----SRV---
	DiffuseTexture,
	//---CBV-----
	CameraConstantBuffer,
	Count
};

enum class Default3DRootSignatureParams {

	//----SRV---
	DiffuseTexture,
	ShadowMapTexture,
	//---CBV-----
	CameraConstantBuffer,
	ModelConstantBufferD12,
	GameConstantBuffer,
	Count
};

enum class PBRRootSignatureParams {

	//----SRV---
	DiffuseTexture = 0,
	NormalTexture,
	MetallicTexture,
	RoughnessTexture,
	ShadowMapTexture,

	//---CBV-----
	CameraConstantBuffer,
	GameConstantBuffer,
	Count
};

enum class LocalRootSignatureParams
{
	IndexBuffer = 0,
	VertexBuffer = 1,
	Count
};


namespace TextureRP {
	enum Value {
		TextureSRV,
		TextureSampler,
		ConstantBuffer,
		RootParameterCount
	};
}

enum class DepthTestD12
{
	ALWAYS,
	NEVER,
	EQUAL,
	NOTEQUAL,
	LESS,
	LESSEQUAL,
	GREATER,
	GREATEREQUAL,
};

enum class ShaderType
{
	VertexOrPixelShader,
	ComputeShader,
	RaytracingShader
};


struct RendererD12Config
{
	Window* m_window;
	bool m_VR = false;
	bool m_isFullscreen = false;
	bool m_isVsyncEnabled = false;
	bool m_isRaytracingEnabled = false;
};

struct Viewport
{
	float left;
	float top;
	float right;
	float bottom;
};

class ShaderCompiler
{

public:
	ShaderCompiler();
	~ShaderCompiler();

	ComPtr<IDxcBlob>  Compile(const char* ShaderFilePath);
	ComPtr<ID3DBlob>  CompileVsPs(const char* filePath, const D3D_SHADER_MACRO* defines, const std::string& entryPoint, const std::string& target);
	ComPtr<IDxcBlob>  CompileComputeShader(const char* ShaderFilePath);
	IDxcBlob* Compile(IDxcBlobEncoding* sourceBlob, LPCWSTR* args, unsigned long long numargs);

private:
	ComPtr<IDxcCompiler>					m_compiler;
	ComPtr<IDxcLibrary>						m_utils;
	ComPtr<IDxcIncludeHandler>				m_handler;
};

class RendererD12
{
	public:
		RendererD12();
		RendererD12(RendererD12Config const& rendererConfig);

	public:
		 //---------------------- MAIN FUNCTIONS -----------------------------
		 void			Startup();
		 void			BeginFrame();
		 void			EndFrame();
		 void			ShutDown();
		 void			RenderFrame();

		 void			D3D12InterfaceInitialization();
		 void			InitializeAdapterAndCheckRaytracingSupport();
		 void			Prepare();
		 void		    SerializeAndCreateRaytracingRootSignature(D3D12_ROOT_SIGNATURE_DESC& desc, ComPtr<ID3D12RootSignature>* rootSig);
		 void		    SerializeAndCreateRaytracingRootSignature(ID3D12Device5* device, D3D12_ROOT_SIGNATURE_DESC& desc, ComPtr<ID3D12RootSignature>* rootSig);
		 void			Present();
		 void			MoveToNextFrame();
		 void			InitializeRasterization();
		 void			BeginShadowMapRender(ShadowMap* shadowMap);
		 void			EndShadowMapRender(ShadowMap* shadowMap);
		 void			FinishUpGPUWork();

		 //---------------------- IMGUI ------------------------------
		 void			CreateIMGUIRenderTarget();
		 void			SetupImGuiRenderTarget();

		 //----------------------------MESH FUNCTIONS--------------------------
		 Mesh*			GetMesh(const char* filePath);
		 Mesh*			GetMeshForName(const char* name);
		 Mesh*			GetMeshAtIndex(int index);
		 Mesh*			CreateMesh(const char* filePath);
		 Mesh*			CreateOrGetMesh(const char* filePath);
		 Mesh*		    CreateMeshFromSavedFile(const char* filePath);

		 void			PopulateCommandList();
		 void			GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);
		 std::wstring	GetAssetFullPath(LPCWSTR assetName);

		 //-----------------TEXTURES-----------------------------
		 void			CreateTextureSRV(TextureD12* texture);
		 UINT			AllocateDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE* cpuDescriptor, UINT descriptorIndexToUse = UINT_MAX);
		 UINT			AllocateDepthDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE* cpuDescriptor, UINT descriptorIndexToUse = UINT_MAX);
		 UINT			AllocateRenderTargetDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE* cpuDescriptor, UINT descriptorIndexToUse = UINT_MAX);
		 UINT			AllocateImguiDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE* cpuDescriptor, UINT descriptorIndexToUse = UINT_MAX);
		 UINT			AllocateDescriptor(ID3D12DescriptorHeap* heap, D3D12_CPU_DESCRIPTOR_HANDLE* cpuDescriptor, UINT& allocatedDescriptors, UINT descriptorIndexToUse = UINT_MAX);

		 TextureD12*	GetTextureForFileName(char const* name);
		 TextureD12*	GetTextureForFileNameOrPath(char const* fileName, char const* imageFilePath);
		 TextureD12*	CreateTextureFromImage(const Image& image);;
		 TextureD12*	CreateOrGetTextureFromFile(char const* name, char const* imageFilePath);
		 void			CreateOrGetTextureFromFile(char const* fileName, char const* imageFilePath, int& textureIndex);
		 TextureD12*	CreateTextureFromFile(char const* imageFilePath);
		 TextureD12*	GetTextureAtIndex(int index);
		 int			GetTextureIndex(char const* fileName);
		 void	     	BindTexture(int index, TextureD12* textureToBind);
		 void	     	BindTexture(int bufferIndex, int textureIndex);
		 void	     	BindComputeTexture(int index, TextureD12* textureToBind);
		 void	     	BindHandle(int index, D3D12_GPU_DESCRIPTOR_HANDLE& handle);

		 //----------------SHADERS----------------------
		 ShaderD12*		CreateOrGetShader(const char* shaderName, const char* shaderFilePath, bool isCompute = false, bool containsTesselation = false, bool isShadowShader = false);
		 ShaderD12*		GetShader(const char* shaderName);
		 void			BindShader(ShaderD12* shader, bool isForShadowMap = false);
		 void			BindComputeShader(ShaderD12* shader);
		 void			DispatchComputeShader(int x, int y, int z, ShaderD12* shader);

		 //----------------BITMAP FONTS----------------------
		 BitmapFont*	CreateBitmapFont(const char* bitmapFontFilePathWithNoExtension);

		 //----------------CAMERA FUNCTIONS----------------------------
		 void			BeginCamera(const Camera& camera);
		 void			BeginRasterizerCamera(const Camera& camera, ShadowMap* shadowMap = nullptr);
		 void			SetModelConstantData(Mat44 modelMatrix, Vec4 color, int index);
		 void			EndCamera(const Camera& camera);

		 //---------------------MAIN RENDER FUNCTIONS------------------------------
		 void			ClearScreen(Rgba8 color);
		 void			DrawVertexArray(int size, VertexNormalArray array);
		 void			DrawIndexedVertexArray(int numberOfVertices, VertexNormalTangentArray& verticesToDraw, std::vector<unsigned int>& indexes);
		 void			DrawVertexArray(int numberOfVertices, VertexNormalTangentArray& verticesToDraw);
		 void			DrawVertexArray(int size, VertexArray& array);
		 void			SetDepthStencilState(DepthTestD12 depthTest, bool writeDepth);

		 //----------------GET FUNCTIONS----------------
		 RenderingPipeline					m_renderingPipeline = RenderingPipeline::Raytracing;
		 ID3D12Device*						GetDevice();
		 ID3D12GraphicsCommandList*			GetCommandList();
		 ComPtr<ID3D12GraphicsCommandList>  GetCommandListComPtr();
		 ID3D12Resource*					GetBackBuffer();
		 D3D12_CPU_DESCRIPTOR_HANDLE*		GetBackBufferCPUHandle();
		 ID3D12CommandAllocator*			GetCommandAllocator();
		 void								ResetCommandAllocator();
		 ID3D12DescriptorHeap*				GetDescriptorHeap();
		 ResourceManager*					GetResourceManager();
		 ID3D12DescriptorHeap*				GetIMGUIDescriptorHeap();
		 D3D12_CPU_DESCRIPTOR_HANDLE		CreateAndGetImGuiCPUDescriptorHandle();
		 D3D12_CPU_DESCRIPTOR_HANDLE		GetIMGUIRenderTarget();
		 D3D12_GPU_DESCRIPTOR_HANDLE		GetGPUDescriptorHandle(ID3D12DescriptorHeap* heap, int offsetindex);
		 int								GetFrameIndex();
		 RendererD12Config					GetRenderConfig();
		 IDXGISwapChain3*					GetSwapChain();
		 D3D12_CPU_DESCRIPTOR_HANDLE		GetDepthStencilViewHandle();
		 void								PopulateSimpleCubeVertices();
		 void								AllocateUploadBuffer(ID3D12Device* pDevice, void* pData, UINT64 datasize, ID3D12Resource** ppResource, const wchar_t* resourceName);
		 void								AllocateUAVBuffer(ID3D12Device* pDevice, UINT64 bufferSize, ID3D12Resource** ppResource, D3D12_RESOURCE_STATES initialResourceState, const wchar_t* resourceName);
		 void								ExecuteCommandList(ComPtr<ID3D12GraphicsCommandList> m_commandList);
		 void								ExecuteCommandList(int commandsToExecute,ComPtr<ID3D12GraphicsCommandList> m_commandList);
		 void								WaitForGpu();
		 void								ThrowIfFailed(HRESULT hr, const char* msg);
		 void								ThrowIfFalse(bool result, const char* msg);

	//--------------------SCENE VARIABLES-----------------------
	public:	
		RendererD12Config				 m_renderConfig = {};
		bool							 m_isFirstFrame = true;
		IntVec2							 m_dimensions;
		IntVec2							 m_windowDimensions;
		Camera							 m_currentCamera;

		D3D12_CPU_DESCRIPTOR_HANDLE		 m_imGUICpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE		 m_imGUIGpuHandle;
		float							 m_gpuWaitTime;

		//-----------------------------RESOURCE MANAGEMENT---------------
		ResourceManager*				 m_resourceManager = nullptr;

	public:
		//------------------VARIABLES SPECIFIC TO DX12 DXR---------------------------
		std::vector<TextureD12*>		 m_loadedTextures;
		std::vector<Mesh*>				 m_loadedMeshes;
		std::vector<ShaderD12*>		     m_loadedShaders;
		ShaderD12*						 m_currentShader;
		ShaderCompiler*					 m_shaderCompiler = nullptr;
		UINT							 m_frameIndex;

		//-----DESCRIPTOR VARIABLES------------------
		UINT							 m_descriptorsAllocated;
		UINT							 m_depthDescriptorsAllocated;
		UINT							 m_rtvDescriptorsAllocated;
		UINT							 m_imguiDescriptorsAllocated;
		UINT							 m_descriptorSize;
		UINT							 m_depthDescriptorSize;
		UINT							 m_imGuidescriptorSize;
		UINT							 m_RrtvDescriptorSize;
		ComPtr<ID3D12DescriptorHeap>     m_dsvDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap>	 m_descriptorHeap;	
		ComPtr<ID3D12DescriptorHeap>	 m_imguiDescriptorHeap;	
		ComPtr<ID3D12DescriptorHeap>	 m_rtvDescriptorHeap;

		ComPtr<IDXGISwapChain3>			 m_RswapChain;
		ComPtr<ID3D12Device>			 m_Rdevice;
		ComPtr<IDXGIFactory4>            m_dxgiFactory;
		UINT                             m_adapterIDoverride;
		UINT                             m_adapterID;
		ComPtr<IDXGIAdapter1>            m_adapter;
		std::wstring                     m_adapterDescription;
		D3D_FEATURE_LEVEL                m_d3dMinFeatureLevel = D3D_FEATURE_LEVEL_11_0;
		const static UINT                m_backBufferCount = 2;
		ComPtr<ID3D12CommandAllocator>   m_RcommandAllocator[m_backBufferCount];
		
		ComPtr<ID3D12CommandQueue>		 m_RcommandQueue;
		ComPtr<ID3D12GraphicsCommandList>m_RcommandList;
		bool							 m_isRendererPrepared = false;
		ComPtr<ID3D12GraphicsCommandList4>m_dxrCommandList;
		std::vector<RenderItems>		 m_dynamicRenderItems;
		D3D12_VIEWPORT                   m_screenViewport;
		D3D12_RECT                       m_scissorRect;
		ComPtr<ID3D12Resource>			 m_backBufferRenderTarget[m_backBufferCount] = {};

		IDXGIDebug*						 m_dxgiDebug = nullptr;
		void*							 m_dxgiDebugModule = nullptr;
		DepthStencilHandle				 m_depthStencilHandle;
		D3D12_RASTERIZER_DESC			 m_rasterizerDesc;
		CD3DX12_CPU_DESCRIPTOR_HANDLE	 m_backBufferHeapCPUHandle[m_backBufferCount] = {};

		ComPtr<ID3D12Resource>			 m_imguiBackBufferRenderTarget[m_backBufferCount] = {};
		DXGI_FORMAT                      m_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_FORMAT                      m_depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		
	
		//------------------VARIABLES SPECIFIC TO D3D12---------------------------								
		D3D12_CPU_DESCRIPTOR_HANDLE		m_renderTargetViewHandle;
		D3D12_CPU_DESCRIPTOR_HANDLE		m_ImguirenderTargetViewHandle;
		D3D12_RESOURCE_STATES			m_beforeState;

		//----------------PRESENTATION FENCE OBJECTS-----------------------
		ComPtr<ID3D12Fence>								 m_fence;
		UINT64                                           m_fenceValues[m_backBufferCount];
		Microsoft::WRL::Wrappers::Event                  m_fenceEvent;

		//------------------------RASTERIZATION VARIABLES-----------------
		ConstantBufferHandle<CameraConstantBuffer>	m_cameraCB;
		ConstantBufferHandle<EngineDataBuffer>		m_gameDataCB;
		ConstantBufferHandle<ModelConstantBuffer>	m_modelConstantsCB;

};


