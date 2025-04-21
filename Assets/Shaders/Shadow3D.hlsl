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

//--------------Constant Buffers----------------------------
cbuffer CameraConstants : register(b0)
{
	float4x4 projectionMatrix;
	float4x4 viewMatrix;
	float4x4 lightViewMatrix;
	float4x4 lightProjMatrix;
	float4 cameraPosition;
	float4 debugData;
};

cbuffer ModelConstantBuffer : register(b1)
{
	float4x4 modelMatrix;
	float4 color;
};

cbuffer GameDataBuffer : register(b2)
{
	float4 globalLightPosition[4];
	float4 globalLightDirection[4];
	float4 globalLightColor[4];
	float4 ViewX_GIOnY_ShadowPassZ;
	float  renderOutput; // ------- 0 final output 1 Normals 
};
//--------------Samplers--------------------
SamplerState SimpleSampler						: register(s0);


//---------------Vertex Shader--------------------------------------------------------------------------------
VertexShaderOutput VertexMain(VertexShaderInput input)
{
	float4 localPosition = float4(input.localPosition, 1);
	float4 worldPosition = mul(modelMatrix, localPosition);
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
	return float4(1,1,1,1);
}
