

//------------------------------------------------------------------------------------------------
struct VertexShaderInput
{
	float3 localPosition : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

//------------------------------------------------------------------------------------------------
struct VertexShaderOutput // Or Pixel Shader input
{
	float4 position : SV_Position;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

//--------------Shader resource views-----------------------
Texture2D DiffuseTexture : register(t0);

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

//--------------Samplers--------------------
SamplerState SimpleSampler : register(s0);

//---------------Vertex Shader--------------------------------------------------------------------------------
VertexShaderOutput VertexMain(VertexShaderInput input)
{
	VertexShaderOutput v2p;
	float4 positions;
	positions.x = input.localPosition.x;
	positions.y = input.localPosition.y;
	positions.z = input.localPosition.z;
	positions.w = 1.0f;
	float4 modelToView = mul(viewMatrix, positions);
	float4 viewToScreen = mul(projectionMatrix, modelToView);
	v2p.position = viewToScreen;
	v2p.color = input.color;
	v2p.uv = input.uv;
	v2p.uv = float2(input.uv.x, input.uv.y);
	return v2p;
}

//------------------------------------------------------------------------------------------------
float4 PixelMain(VertexShaderOutput input) : SV_Target0
{
	float4 color = DiffuseTexture.Sample(SimpleSampler, input.uv);
	return color * input.color;
}

