
#include "DataStructures.hlsli"
#include "Math.hlsli"


//--------------TESSELATION-----------
struct PatchConstants
{
	float tesselationFactor : SV_TessFactor;
};

//--------------Shader resource views-----------------------
Texture2D DiffuseTexture						: register(t0);


//--------------Samplers--------------------
SamplerState SimpleSampler						: register(s0);
SamplerComparisonState  ShadowBilinearSampler	: register(s1);


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

	float4 lightCoordinates = mul(lightViewMatrix, worldPosition);
	float4 lightProjs = mul(lightProjMatrix, lightCoordinates);
	float4 shadowMap = lightProjs;

	VertexShaderOutput vsOutput;
	vsOutput.position = clipPosition;
	vsOutput.normal = localNormal.xyz;
	vsOutput.color = input.color;
	vsOutput.uv = input.uv;
	vsOutput.worldPosition = worldPosition;
	vsOutput.ShadowMapping = shadowMap;

	return vsOutput;
}

//------------------------------------------------------------------------------------------------
float4 PixelMain(VertexShaderOutput input) : SV_Target0
{
	float4 color = DiffuseTexture.Sample(SimpleSampler, input.uv);
	return color * input.color;
}


//[patchconstantfunc("FunctionName")]
//void PatchConstantFunction(uint patchID : SV_PrimitiveID , out PatchConstants patchConstants)
//{
//}

//[domain("tri")]
//[partitioning("integer")]
//[outputtopology("triangle_cw")]
//void HullMain(inout PointStream<VertexShaderOutput> output, uint index : SV_OutputControlPointID, uint patchId : SV_PrimitiveID, out PatchConstants patchConstants )
//{
	
//}

//[domain("tri")]
//void DomainMain(PatchConstants patchConstants,float3 baryCentric : SV_DomainLocation, const OutputPatch<VertexShaderOutput, 3> input, inout VertexShaderOutput output)
//{
	
//}