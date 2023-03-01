#pragma once
const char* shaderSources = R"(
float1 RangeMap(float1 inputValue, float1 inputStart, float1 inputEnd, float1 outputStart, float1 outputEnd)
{
	float1 range = inputEnd - inputStart;
	float1 fraction = (inputValue - inputStart) / range;
	float1 returnValue = (outputStart + ((outputEnd - outputStart) * fraction));
	return returnValue;
}

cbuffer CameraBuffer : register(b2)
{
	float4x4 ProjectionMatrix;
	float4x4 ViewMatrix;
};

cbuffer ModelConstants : register(b3)
{
	float4x4 ModelMatrix;
	float4   ModelColor;
};
struct vs_input_t
{
	float3 localPosition : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

struct v2p_t
{
	float4 position : SV_Position;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

v2p_t VertexMain(vs_input_t input)
{
	v2p_t v2p;
	float4 positions;
	positions.x = input.localPosition.x;
	positions.y = input.localPosition.y;
	positions.z = input.localPosition.z;
	positions.w = 1.0f;
	positions = mul(ModelMatrix, positions);
	float4 modelToView = mul(ViewMatrix, positions);
	float4 viewToScreen = mul(ProjectionMatrix, modelToView);
	v2p.position = viewToScreen;
	v2p.color = input.color * ModelColor;
	v2p.uv = input.uv;
	return v2p;
}

Texture2D diffuseTexture : register(t0);
SamplerState diffuseSampler : register(s0);

float4 PixelMain(v2p_t input) : SV_Target0
{
	
	float4 color = diffuseTexture.Sample(diffuseSampler,input.uv);
	return color * input.color;
}


float4x4 CreateMatrixFromRows(float4 r0, float4 r1, float4 r2, float4 r3) {
    return float4x4(r0.x, r0.y, r0.z, r0.w,
                    r1.x, r1.y, r1.z, r1.w,
                    r2.x, r2.y, r2.z, r2.w,
                    r3.x, r3.y, r3.z, r3.w);
}
)";


