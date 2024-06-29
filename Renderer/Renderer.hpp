#pragma once	
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/IntVec2.hpp"
//#include <gl/gl.h>
#include <vector>
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/ConstantBuffer.hpp"
#include <Engine/Core/Vertex_PNCU.hpp>
#include <Engine/Core/Model.hpp>
#include "Engine/Renderer/TextureView.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vec4.hpp"

class Shader;
class VertexBuffer;
class IndexBuffer;
class Image;

struct  ID3D11View;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11RasterizerState;
struct ID3D11BlendState;
struct ID3D11SamplerState;
struct IDXGIDebug;
struct ID3D11DeviceChild;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;

#define DX_SAFE_RELEASE(dxObject)								\
{											\
	if (( dxObject) != nullptr)								\
	{										\
		(dxObject)->Release();							\
		(dxObject) = nullptr;								\
	}										\
}

#undef OPAQUE
struct RendererConfig
{
	Window* m_window;
	bool m_VR = false;
};
enum class BlendMode
{
	Opaque,
	ALPHA,
	ADDITIVE,
};
enum class CullMode
{
	NONE,
	FRONT,
	BACK,
};
enum class FillMode
{
	SOLID,
	WIREFRAME,
};
enum class WindingOrder
{
	CLOCKWISE,
	COUNTERCLOCKWISE,
};

struct CameraConstants
{
	Mat44 ProjectionMatrix;
	Mat44 ViewMatrix;
};

struct EffectsBuffer
{
	Vec4 effects;
};

//----------------------LIGHT VARIABLES---------------------
struct LightConstants
{
	float sunDirection[3];
	float SunIntensity;
	float AmbientIntensity;
	float padding[3];
};
struct ModelConstants
{
	Mat44 ModelMatrix;
	float color[4];
};

//---------------------RAYTRACING OBJECT CONSTANTS--------------------
struct WorldSphereObjects
{
	float CameraPosition[4];
	float CameraDirection[4];
	float SpherePosition[8][4];
	float SphereColors[8][4];
	float SphereRadius[8][4];
	float TotalSphereObjects;
	float WorldObjectpadding[3];
};

struct WorldCubeObjects
{
	float CubePosition[4][4];
	float CubeColors[4][4];
	float CubeDimensions[4];
	float TotalCubeObjects;
	float PaddingCube[3];
};

struct WorldPlaneObjects
{
	float PlanePosition[4][4];
	float PlaneColor[4][4];
	float PlaneDimensions[4][4];
	float PlaneNormal[4];
	float TotalPlaneObject;
	float PaddingPlane[3];
};
struct MinecraftGameConstants
{
	float		b_camWorldPos[4];
	float		b_skyColor[4];
	float		b_outdoorLightColor[4];
	float		b_indoorLightColor[4];
	float		b_fogStartDist;
	float		b_fogEndDist;
	float		b_fogMaxAlpha;
	float		b_time;
};

//----------------FIRE SHADER VARIABLES-------------------------
struct NoiseConstants
{
	float frameTime;
	float scrollSpeeds[3];
	float scales[3];
	float padding;
};

struct PointLights
{
	float PointLightPosition[3];
	float PointLightRadius;
	float PointLightColor[4];
	float PointLightIntensity;
	float PaddingLight[3];
};
struct ShadowLights
{
	float ShadowLightPosition;
	float ShadowLightRadius;
	Mat44 ShadowLightMatrix;

};

struct DistortionConstants
{
	float distortion1[2];
	float distortion2[2];
	float distortion3[2];
	float distortionScale;
	float distortionBias;
};

enum class DepthTest
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

enum class SamplerMode
{
	POINTCLAMP,
	POINTWRAP,
	BILINEARCLAMP,
	BILINEARWRAP,
};
enum class StereoscopicEye
{
	Left,
	Right
};
class Renderer
{

public:
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
	IDXGISwapChain* m_swapChain = nullptr;
	ID3D11RasterizerState* m_rasterizerState = nullptr;
	ID3D11BlendState* m_blendState = nullptr;
	ID3D11SamplerState* m_samplerState = nullptr;
	ID3D11SamplerState* m_samplerState2 = nullptr;
	IDXGIDebug* m_dxgiDebug = nullptr;
	void* m_dxgiDebugModule = nullptr;
	ID3D11DepthStencilState* m_depthStencilState = nullptr;
	ID3D11Texture2D* m_depthStencilTexture = nullptr;

public:
	Renderer();
	Renderer(RendererConfig const& rendererConfig);
	virtual void					Startup();
	virtual void					BeginFrame();
	virtual void					RenderFrame() const;
	virtual void					EndFrame();
	virtual void					ShutDown();
	RendererConfig					GetRenderConfig();


	//----------------OPENGL FUNCTIONS---------------------
	void					CreateRendererContext();

	//----------------DIRECTX FUNCTIONS-----------------------
	virtual void			CreateDeviceSwapChain();
	void					ReportingSetup();
	virtual void			CreateRenderTarget();
	void				    CreateViewPort();
	virtual void			SetViewPortForCamera(const Camera& camera);
	void					SetDebugName(ID3D11DeviceChild* object, char const* name);
	void					ReportLiveObjects();
	void					SetModalMatrix( Mat44 const& modelMatrix);
	Mat44					GetModelMatrix();
	void					SetRenderTarget(ID3D11RenderTargetView* view);
	void					SetModalColor(float modelColor[4]);
	void					SetRasterizerState(CullMode cullMode, FillMode fillMode, WindingOrder windingOrder);
	//-----------------SHADER---------------------
	Shader*					CreateOrGetShader(const char* shaderName) ;
	void					BindShaderByName(const char* shaderName);
	void					BindShader(Shader* shader);
	Shader*					GetShaderForName(const char* shaderName);
	Shader*					CreateShader(char const* shaderName, char const* shaderSource);
	bool					CompileShaderToByteCode(std::vector<unsigned char>& outByteCode, char const* name, char const* source, char const* entryPoint, char const* target);
	void					CreateVertexShader(Shader* shader, const char* shaderSources, std::vector<uint8_t> vertexByte);
	void					CreatePixelShader(Shader* shader, char const* shaderSources, std::vector<uint8_t> pixelByte);
	void					CreateInputLayout(Shader* shader, std::vector<unsigned char> vertexByte);
	Shader*					CreateShaderFromFile(const char* shaderName);
	Shader*					GetDefaultShader();


	//-----------------Mesh------------------------
	Mesh*			CreateOrGetMesh(const char* filePath);
	Mesh*			CreateMeshFromSavedFile(const char* filePath);
	Mesh*			CreateOrGetMesh(const char* filePath, const char* fileName);
	Mesh*			Create(const char* filePath, const char* fileName);
	Mesh*			GetMeshByName(const char* Name);
	Mesh*			CreateMesh(const char* filePath);
	Mesh*			GetMesh(const char* filePath);
	Mesh*			CreateMesh(const char* filePath, const char* name);
	//Mesh*			BuildMesh(const char* filePath);

	//------------------VERTEX BUFFER--------------
	VertexBuffer*			CreateVertexBuffer(const size_t size);
	VertexBuffer*			CreateVertexBuffer(const size_t size, VertexBuffer* vbo);
	void					CopyCPUToGPU(const void* data, size_t size, VertexBuffer* vbo);
	void					BindVertexBuffer(VertexBuffer* vbo);
	void					DrawVertexBuffer(VertexBuffer* vbo, int vertexCount, int vertexOffset = 0);
	void					DrawIndexedVertexBuffer(VertexBuffer* vbo, IndexBuffer* ibo, int vertexCount, int vertexOffset,int indexCount, int indexOffset);


	//------------------CONSTANT BUFFER-------------
	ConstantBuffer*			CreateConstantBuffer(const size_t size, ConstantBuffer* cbo);
	void					CopyCPUToGPU(const void* data, size_t size, ConstantBuffer* cbo);
	void					BindConstantBuffer(int slot, ConstantBuffer* cbo);

	//------------------CONSTANT BUFFER-------------
	IndexBuffer*			CreateIndexBuffer(const size_t size, IndexBuffer* cbo);
	void					CopyCPUToGPU(const void* data, size_t size, IndexBuffer* cbo);
	void					BindIndexBuffer(int slot, IndexBuffer* cbo);

	void					ReleaseConstantBuffer(ConstantBuffer* buffer);
	void					ReleaseIndexBuffer(IndexBuffer* buffer);
	void					ReleaseVertexBuffer(VertexBuffer* buffer);

	//---------------GAME UTILITY FUNCTIONS  (CAMERA AND DRAW FUNCTIONS)------------------
	void					DrawTriangle(Vec2 point1, Vec2 point2, Vec2 point3, Rgba8 color);
	virtual void			ClearScreen(const Rgba8& clearColor);
	virtual void			BeginCamera(const Camera& camera);
	void					BindCameraConstantsToShader();
	void					EndCamera(const Camera& camera);
	virtual void			DrawVertexArray(int numVertexes, Vertex_PCU const* vertexes);
	virtual void			DrawVertexArray(std::vector<Vertex_PCU>& vertexes);
	virtual void			DrawIndexedVertexArray(int numVertexes, Vertex_PCU const* vertexes, int indeces, const unsigned int* indexes, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer);
	void					DrawIndexedVertexArray(int indeces);
	virtual void			DrawIndexedVertexArray(int numVertexes, Vertex_PNCU const* vertexes, int indeces, const unsigned int* indexes, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer);
	virtual void			DrawVertexArray(int numVertexes, Vertex_PNCU const* vertexes);
	void				    DrawVertexArray(std::vector<Vertex_PNCU> const& vertexes);
	void					DrawVrVertexArray(int numVertexes, std::vector<Vertex_PCU> vertexes, ID3D11DepthStencilView* depthView, ID3D11RenderTargetView* targetView);
	virtual void			DrawVertexArray(int numVertexes, std::vector<Vertex_PCU>& vertexes);
	void					DrawVertexArray(int numVertexes, std::vector<Vertex_PCU> const& vertexes);
	void					DrawVertexArray(int numVertexes, std::vector<Vertex_PNCU> const& vertexes);
	Camera					GetCurrentCamera();

	//-----------------------TEXTURE FUNCTIONS-------------------------------------
	Texture*	CreateOrGetTextureFromFile(char const* imageFilePath);
	Texture*	CreateTextureFromFile(char const* imageFilePath);
	Texture*	CreateTextureFromData(char const* name, IntVec2 dimensions, int bytesPerTexel, uint8_t* texelData);
	Texture*	GetTextureForFileName(char const* imageFilePath);
	Texture*	CreateTextureFromImage(const Image& image);			
	Texture*	CreateTextureFromInfo(const TextureCreateInfo& image);
	void		DestroyTexture(Texture* texture);
	Texture*	GetCurrentColorTarget();
	Texture*	GetCurrentDepthTarget();
	void		CopyTexture(Texture* from, Texture* to);

	//----------EFFECTS------------------------
	void		PrepareRendererForNextPass();
	void		ApplySpecialEffects(Shader* shader);
	void		ApplySpecialEffects(Shader* shader,ConstantBuffer* constantBuffer, EffectsBuffer& data);

	//-----------------------DIRECTX-------------------------------------

	BitmapFont*  CreateOrGetBitmapFont(const char* bitmapFontFilePathWithNoExtension);
	BitmapFont*  CreateFontFromFile(char const* fontFilePath);
	BitmapFont*  GetFontForFileName(char const* fontFilePath);
	void		BindTexture(const Texture* texture);
	void		BindTexture(const Texture* texture, int slot);
	void		BindTexture(std::vector<Texture*>  texture, std::vector<int> slots);
	void		SetBlendMode(BlendMode blendMode);


	//----------------------SAMPLER FUNCTIONS------------------------------------
	void					BindSampler();
	void					BindSampler(int samplerSlot, ID3D11SamplerState* m_samplerState);
	void					SetSamplerMode(SamplerMode samplerMode, int slot, ID3D11SamplerState* samplerState);
	void					SetSamplerMode(SamplerMode samplerMode);
	void					ReleaseSampler(ID3D11SamplerState* sampler);

	//-----------------------DEPTH STENCIL AND BACK BUFFER FUNCTIONS-----------------------
	void					CreateDepthStencilTextureAndView();
	void					CreateBackBuffer();
	void					ClearDepth(float value = 1.0f);
	void					SetDepthStencilState(DepthTest depthTest, bool writeDepth);

	//----------------------LIGHT FUNCTIONS------------------------
	void					SetSunDirection(const Vec3& direction);
	void					SetSunIntensity(float intensity);
	void					SetAmbientIntensity(float intensity);
	void					SetSunColor(float sunColor[3]);
	void					AddPointLight(Vec3 Position, float intensity, Rgba8 color, float radius);
	Transform				GetLightData(int index);

	//----------------------RAYTRACING FUNCTIONS-----------------
	void					AddWorldObjectSphere(const Vec3& spherePosition, const float sphereRadius, Rgba8 color, int objectNumber);
	void					AddWorldObjectCube(const Vec3& cubePosition, const float cubeDimension, Rgba8 color, int objectNumber);
	void					AddWorldObjectPlane(const Vec3& planePosition, const Vec3& planeNormal, const Vec3& planeDimension, Rgba8 color, int objectNumber);

	//-----------------------VR SUPPORT FUNCTIONS-----------------
	virtual void			OpenXRRenderFrame();
	
	//-----------------------PUBLIC VARIABLES---------------------
	static bool				ToggleDebugRendererVisibility();
	RendererConfig						m_redererConfig;

	//-----------------------------CONSTANT BUFFER VARIABLES--------------------
	std::vector<PointLights>			m_pointLights;
	MinecraftGameConstants				m_GameConstants;
	WorldSphereObjects					m_worldSpheres;
	WorldCubeObjects					m_worldCubes;
	WorldPlaneObjects					m_worldPlanes;
	ConstantBuffer*						m_cameraCBO = nullptr;
	ConstantBuffer*						m_modalCBO = nullptr;
	ConstantBuffer*						m_lightCBO = nullptr;
	ConstantBuffer*						m_pointLightCBO = nullptr;
	ConstantBuffer*						m_gameConstantCBO = nullptr;
	ConstantBuffer*						m_fireDistortionCBO = nullptr;
	ConstantBuffer*						m_fireNoiseCBO = nullptr;
	ConstantBuffer*						m_worldSpheresCBO = nullptr;
	ConstantBuffer*						m_worldPlanesCBO = nullptr;
	ConstantBuffer*						m_worldCubesCBO = nullptr;
	ConstantBuffer*						m_raytracingWorldObjectsCBO = nullptr;
	const int							m_constantBufferSlot = 2;
	Texture*							m_depthBuffer = nullptr;
	Texture*							m_backBuffer = nullptr;
	Texture*							m_activeColorTarget[2] =  {} ;
	int									m_activeColorTargetIndex = 0;
private:
	const char*							defaultShaderName = "Default";
	Shader*								m_defaultShader = nullptr;
	
	std::vector<Texture*>				m_loadedTextures;
	std::vector<BitmapFont*>			m_loadedFonts;
	std::vector<Shader*>				m_loadedShaders;
	std::vector<Mesh*>			m_loadedMeshes;
	Shader const*						m_currentShader = nullptr;
	VertexBuffer*						m_immediateVBO = nullptr;
	VertexBuffer*						m_immediateVBO_PNCU = nullptr;
	IndexBuffer*						m_immediateIBO = nullptr;


	const int							m_modalconstantBufferSlot = 3;
	const int							m_lightConstantBufferSlot = 4;
	const int							m_fireNoiseConstantBufferSlot = 4;
	const int							m_fireDistortionConstantBufferSlot = 5;
	const int							m_sphereBufferSlot = 6;
	const int							m_cubeBufferSlot = 7;
	const int							m_planeBufferSlot = 8;
	const int							m_minecraftBufferSlot = 8;
	Texture*							m_defaultTexture = nullptr;
	Mat44								m_modelMatrix;
	float								m_modelColor[4];
	Camera*								m_currentCamera;


	Vec3								m_sunDirection = Vec3(0.0f, 0.0f, -1.0f);
	float								m_sunIntensity = 0.0f;
	float								m_ambientIntensity = 0.3f;
	float								m_sunColor[3];

public:
	//-----------------------------OPEN XR VARIABLES-----------------
	Mat44								m_stereoscopicProjectionMatrix;
	Mat44								m_stereoscopicViewToRenderMatrix;
	float								m_frameTime = 0;
	float								m_leftStereoscope = 0.0f;
	float								m_rightStereoscope = 0.0f;
};

