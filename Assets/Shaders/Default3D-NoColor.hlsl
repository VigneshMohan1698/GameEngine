
#include "Math.hlsli"
//------------------------------------------------------------------------------------------------
struct VertexShaderInput
{
	float3 localPosition : POSITION;
	float3 localNormal : NORMAL;
	float4 color : COLOR;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float2 uv : TEXCOORD;
};

//------------------------------------------------------------------------------------------------
struct VertexShaderOutput // Or Pixel Shader input
{
	float4 position : SV_Position;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
	float4 worldPosition : WORLD_POSITION;
	float4 ShadowMapping : SHADOW_MAPPING;
};


//--------------Samplers--------------------
SamplerState SimpleSampler						: register(s0);

//--------------Constant Buffers----------------------------
cbuffer CameraConstantBuffer : register(b0)
{
	float4x4 projectionMatrix;
	float4x4 viewMatrix;
	float4x4 lightViewMatrix;
	float4x4 lightProjMatrix;
	float4 cameraPosition;
	float4 debugData;
};

cbuffer ModelConstantsD12 : register(b1)
{
	float4x4 ModelMatrix;
	float4 Color;
};

cbuffer GameDataBuffer : register(b2)
{
	float4 globalLightPosition[4];
	float4 globalLightDirection[4];
	float4 globalLightColor[4];
	float4 ViewX_GIOnY_ShadowPassZ;
	float  renderOutput; // ------- 0 final output 1 Normals 
};


//---------------Vertex Shader--------------------------------------------------------------------------------
VertexShaderOutput VertexMain(VertexShaderInput input)
{
	float4 localPosition = float4(input.localPosition, 1);
	float4 worldPosition = mul(ModelMatrix, localPosition);
	float4 viewPosition = mul(viewMatrix, worldPosition);
	float4 clipPosition = mul(projectionMatrix, viewPosition);
	float4 localNormal = float4(input.localNormal, 0);


	VertexShaderOutput vsOutput;
	vsOutput.position = clipPosition;
	vsOutput.normal = localNormal.xyz;
	vsOutput.color = input.color;
	vsOutput.uv = input.uv;
	vsOutput.worldPosition = worldPosition;

	return vsOutput;
}

//------------------------------------------------------------------------------------------------
float4 PixelMain(VertexShaderOutput input) : SV_Target0
{
	////------------If pixel is in shadow return black--------------
	return input.color;
}
