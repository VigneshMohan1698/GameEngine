
#include "DataStructures.hlsli"
#include "Math.hlsli"

#define PCF_SAMPLES 32
#define BLOCKER_SEARCH_SAMPLES 32

#define NEAR_PLANE 9.5
#define LIGHT_WORLD_SIZE .5
#define LIGHT_FRUSTUM_WIDTH 3.75

#define LIGHT_SIZE_UV (LIGHT_WORLD_SIZE / LIGHT_FRUSTUM_WIDTH)
//--------------TESSELATION-----------
struct PatchConstants
{
	float tesselationFactor : SV_TessFactor;
};

//--------------Shader resource views-----------------------
Texture2D DiffuseTexture						: register(t0);
Texture2D<float> ShadowMap						: register(t1);


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

#define NEAR_PLANE 0.02
#define LIGHT_WORLD_SIZE .5
#define LIGHT_FRUSTUM_WIDTH 3.75 
#define LIGHT_SIZE_UV (LIGHT_WORLD_SIZE / LIGHT_FRUSTUM_WIDTH) 
//
float2 FindBlockerDepth(float2 currentUV, float currentDepth, float2 shadowMapTexelSize, float lightSize)
{
	float randomAngle = frac(currentUV.x * 18534.6 + currentUV.y * 23853.2) * 2.0f * 3.1412f;

	float sin = 0.0f, cos = 0.0f;
	sincos(randomAngle, sin, cos);
	float2 randomRotation = float2(cos, sin);

	float2 result = float2(0.0f,0.0f);
	float avgBlockerDepth = 0.0f;
	int numBlockers = 0;

	float searchWidth = LIGHT_SIZE_UV * (currentDepth - NEAR_PLANE) / currentDepth;
	// Poisson Disc samples
	for (int poissonIndex = 0; poissonIndex < BLOCKER_SEARCH_SAMPLES; poissonIndex++)
	{
		float2 offsetUV = PoissonOffsets[poissonIndex] * 3.0f * shadowMapTexelSize * randomRotation;

		//Get the neighbour possion offsetted UV and sample depth 
		float2 neighbourTexel = currentUV + offsetUV;

		//if (!GetIsWithin0to1(neighbourTexel)) // If neighbour texel is out of bounds
		//{
		//	avgBlockerDepth += 1.0f;
		//	numBlockers++;
		//}
		float sampleDepth = ShadowMap.SampleLevel(SimpleSampler, neighbourTexel, 0.0f).r;
		if (sampleDepth < currentDepth)
		{
			avgBlockerDepth += sampleDepth;
			numBlockers++;
		}

	}

	// Compute average blocker depths
	if (numBlockers > 0)
	{
		avgBlockerDepth /= numBlockers;
	}
		
	result = float2(avgBlockerDepth, float(numBlockers));
	return result;
}

float ComputePenumbraSize(float lightSize, float depthReceiver, float depthBlocker)
{
	// Calculate the size of the penumbra based on distances
	return ((depthReceiver - depthBlocker) * lightSize) / depthBlocker ;
}

// 1 for full shadow, 0 for no shadow
// the higher you go the shadows get darker
float GetLightIntensity(float4 shadowMappingCoords, inout float blockerDepth, inout float penumbraSize)
{
	float lightIntensity = 1.0f;
	float3 shadowMapNDC = shadowMappingCoords.xyz / shadowMappingCoords.w;
	float2 shadowMapUV = (shadowMapNDC.xy * 0.5f) + 0.5f;
	shadowMapUV.y = 1 - shadowMapUV.y;

	float checkDepth = shadowMappingCoords.z;
	if (!GetIsWithin0to1(shadowMapUV))
	{
		return 0.0f; // Any point outside of shadow map bounds is in shadow
	}

	float width, height;
	ShadowMap.GetDimensions(width, height);

	float2 shadowMapSize = float2(width, height);
	float2 shadowMapTexelSize = float2(float(1 / width), float(1 / height));
	float2 currentUV = float2(shadowMapUV.x, shadowMapUV.y);

	if (debugData.x == 0.0f)
	{

		if (!GetIsWithin0to1(currentUV)) // If neighbour texel is out of bounds
		{
			lightIntensity = 1.0f;
		}
		lightIntensity = ShadowMap.SampleLevel(SimpleSampler, currentUV, 1.0f).r > shadowMapNDC.z;
		return lightIntensity;
	}

	if (debugData.x == 1.0f) // PCF with bilinear sampler
	{
		lightIntensity = 0.0f;
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				float2 offsetUV = float2(i, j) * shadowMapTexelSize;
				float2 neighbourTexel = currentUV + offsetUV;

				if (!GetIsWithin0to1(neighbourTexel)) // If neighbour texel is out of bounds
				{
					lightIntensity += 1.0f;
				}
				lightIntensity += ShadowMap.SampleCmp(ShadowBilinearSampler, neighbourTexel, shadowMapNDC.z);
			}
		}
		lightIntensity /= 9.0f;
	}

	if (debugData.x == 2.0f) // PCSS
	{
		lightIntensity = 0.0f;
		float lightSize = debugData.z;
		float depthReceiver = shadowMapNDC.z;
		float2 blockerData = FindBlockerDepth(currentUV, depthReceiver, shadowMapTexelSize, lightSize);
		blockerDepth = blockerData.x;
		float randomAngle = frac(currentUV.x * 18534.6 + currentUV.y * 23853.2) * 2.0f * 3.1412f;

		float sin = 0.0f,cos = 0.0f;
		sincos(randomAngle, sin, cos);
		float2 randomRotation = float2(cos, sin);

		penumbraSize = ((depthReceiver - blockerDepth) ) / blockerDepth;

		float filterRadiusUV = penumbraSize * lightSize;
	/*	float distanceFalloff = saturate(0.001f / penumbraSize + 0.001f);
		penumbraSize = (1.0f - distanceFalloff) * 0.01f;*/
		//float filterRadiusUV = penumbraSize * 0.75 / depthReceiver;
		if(blockerData.y < 1.0f)
		{
			return 1.0f;
		}
		
		

		for (int poissonIndex = 0; poissonIndex < PCF_SAMPLES; poissonIndex++)
		{
				float2 offsetUV = PoissonOffsets[poissonIndex] * filterRadiusUV * randomRotation ;
				float2 neighbourTexel = currentUV + offsetUV;
				
				lightIntensity += ShadowMap.SampleCmpLevelZero(ShadowBilinearSampler, neighbourTexel, depthReceiver);
		}
		lightIntensity /= PCF_SAMPLES;
	}
	return lightIntensity;
}

//---------------Vertex Shader--------------------------------------------------------------------------------
VertexShaderOutput VertexMain(VertexShaderInput input)
{
	float4 localPosition = float4(input.localPosition, 1);
	float4 worldPosition = mul(modelMatrix, localPosition);
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

	float4 lightColor = float4(1, 1, 1, 1);
	float4 ambientIntensity = 0.4f;
	float blockerDepth = 0.0f, penumbraSize = 0.0f;

	float lightIntensity = GetLightIntensity(input.ShadowMapping, blockerDepth, penumbraSize);
	float4 objectUnlitColor = color * input.color;
	float4 objectLitColor = color * input.color * lightColor;

	return lerp(objectUnlitColor * ambientIntensity, objectLitColor, lightIntensity);


}