#include "DataStructures.hlsli"
#include "MathFunctions.hlsli"
#define PI 3.1415


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
	float3 tangent : TANGENTCOORD;
	float3 bitangent : BITANGENTCOORD;
	float4 worldPosition : WORLD_POSITION;
	float4 ShadowMapping : SHADOW_MAPPING;
};

//--------------Shader resource views-----------------------
Texture2D DiffuseTexture						: register(t0);
Texture2D NormalTexture							: register(t1);
Texture2D MetallicTexture						: register(t2);
Texture2D RoughnessTexture						: register(t3);
Texture2D<float> ShadowMap						: register(t4);

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

//--------------Constant Buffers----------------------------
cbuffer GameDataBuffer : register(b1)
{
	float4 globalLightPosition[4];
	float4 globalLightDirection[4];
	float4 globalLightColor[4];
	float4 ViewX_GIOnY_ShadowPassZ_FrameTime;
	float  renderOutput; // ------- 0 final output 1 Normals 
};

//--------------Samplers--------------------
SamplerState SimpleSampler						: register(s0);


float SchlickFresnel(float3 vertexPosition, float2 uv, int lightNumber)
{
	float schlickOutput = 0.0f;
	float4 metallicMap = MetallicTexture.SampleLevel(SimpleSampler, uv, 0);
	float reflectivity = metallicMap.x;
	
	float3 lightDirection = normalize(globalLightPosition[lightNumber].xyz - vertexPosition);
	float3 viewDirection = normalize(cameraPosition.xyz - vertexPosition);
	float3 halfwayDirection = normalize(lightDirection + viewDirection);
	
	schlickOutput = reflectivity + ((1 - reflectivity) * pow(1 - DotProduct3D(viewDirection, halfwayDirection), 5));
	
	return schlickOutput;
}

float SchlickBeckmannShadowing(float3 vertexPosition, float2 uv, float3 normal, float3 lightOrview)
{
	float4 roughnessMap = RoughnessTexture.SampleLevel(SimpleSampler, uv, 0);
	float roughness = roughnessMap.x;
	//roughness = 1 - roughness;
	float k = roughness * roughness / 2;
	
	float numerator = DotProduct3D(normal, lightOrview);
	float denominator = (numerator * (1 - k)) + k;
	denominator = max(denominator, 0.000001f);
	return numerator / denominator;
}
//----------GGX TROWBRIDGE-REITZ-------------------
float NormalDistribution(float3 vertexPosition, float2 uv, float3 normal, int lightNumber)
{
	float normalDistribution = 0.0f;
	float3 lightDirection = normalize(globalLightPosition[lightNumber].xyz - vertexPosition);
	float3 viewDirection = normalize(cameraPosition.xyz - vertexPosition);
	float3 halfwayDirection = normalize(lightDirection + viewDirection);
	
	float4 roughnessMap = RoughnessTexture.SampleLevel(SimpleSampler, uv, 0);
	float roughness = roughnessMap.x;
	//roughness = 1 - roughness;
	
	float alpha = roughness * roughness;
	float NdotH = max(dot(normal, halfwayDirection), 0.0f);
	float ggxDenominator = PI * pow(pow(NdotH, 2) * (pow(alpha, 2) - 1) + 1, 2);
	ggxDenominator = max(ggxDenominator, 0.00001f);
	normalDistribution = alpha / ggxDenominator;

	return normalDistribution;
}

float4 CookTorranceOutput(float3 vertexPosition, float2 uv, float3 normal, int lightNumber)
{
	float cookTorranceValue = 0.0f;
	
	//----------NORMAL DISTRIBUTION----------------

	
	float3 lightDirection = normalize(globalLightPosition[lightNumber].xyz - vertexPosition);
	float3 viewDirection = normalize(cameraPosition.xyz - vertexPosition);
	float3 halfwayDirection = normalize(lightDirection + viewDirection);
	
	float CookTorranceDenominator = 4 * (DotProduct3D(viewDirection, normal) * (DotProduct3D(lightDirection, normal)));
	CookTorranceDenominator = max(CookTorranceDenominator, 0.00001f);
	
	float CookTorranceNumerator = 0.0f; //DGF --> Normal Distribution * Geometry shadowing * fresnel
	float D = NormalDistribution(vertexPosition, uv, normal, lightNumber);
	float G = SchlickBeckmannShadowing(vertexPosition, uv, normal, lightDirection) * SchlickBeckmannShadowing(vertexPosition, uv, normal, viewDirection);
	float F = SchlickFresnel(vertexPosition, uv, lightNumber);
	
	// Multiplying fresnel later in BRDF function. 
	// Since Lambertian coeff kd = 1 - fresnel;
	
	CookTorranceNumerator = D * G * F;
	cookTorranceValue = CookTorranceNumerator / CookTorranceDenominator;
	//--------- TO DO : Specular color same as light color ? ----------------
	float4 specularColor = globalLightColor[0];
	return specularColor * cookTorranceValue;
}


float4 LambertianColor(float3 vertexPosition, float2 uv, float3 normal)
{
	float4 lambertianColor = float4(0, 0, 0, 0);
	float4 diffuseMap = DiffuseTexture.SampleLevel(SimpleSampler, uv, 0);
	lambertianColor = (diffuseMap / PI);
	return lambertianColor;
}


float4 BRDF(float3 vertexPosition, float2 uv,float3 normal, int lightNumber)
{
	//------------ BRDF  = KdDiffuse + KsSpecular ----
	//------------ Note : ks here refers to schlick fresnel coefficient and Kd is lambertian coefficient ----
	//------------ Note : In order to maintain law of conservation of energy kd + ks = 1 ----
	//------------ Specular is calculated by cook torrance method 
	//------------ Diffuse is calculated by lambertian model
	float4 brdfValue = 0.0f;
	
	float4 specular = CookTorranceOutput(vertexPosition, uv, normal, lightNumber);
	float4 lambert = LambertianColor(vertexPosition, uv, normal);
	float ks = SchlickFresnel(vertexPosition, uv, lightNumber);
	float kd = 1 - ks;
	brdfValue = (kd * lambert) + (specular);
	return brdfValue;
}

//----------------THE RENDERING EQUATION--------------
// The light at vertexs  --> L(x,v) = Lemmisive(x,v) + sum(f(x,l,v) Li(x, L) (L.N))dl
// Which means --> emmisive color + integral of (BRDF * INCOMING LIGHT * NDOTL TERM)
float4 RenderingEquation(float3 vertexPosition, float3 normal, float2 uv, int lightNumber)
{
	float4 PBROutput = float4(0, 0, 0, 0);
	float lightDistance = distance(vertexPosition, globalLightPosition[lightNumber].xyz);
	float3 lightDirection = normalize(globalLightPosition[lightNumber].xyz - vertexPosition.xyz);
	
	float NdotL = DotProduct3D(normal, lightDirection);
	PBROutput = globalLightColor[lightNumber] * 5.0f * BRDF(vertexPosition.xyz, uv, normal, lightNumber) * NdotL;
	//pbrOutput = LambertianColor(vertexPosition, uv, normal);
	return PBROutput;
}

float3 CalculateNormals(float2 inputUV, float3 inputNormal, float3 inputTangent, float3 inputBitangent)
{
	float3 normalMap = float3(0, 0, 0);
	float3 finalNormal = float3(0, 0, 0);
	normalMap = float3(NormalTexture.SampleLevel(SimpleSampler, inputUV, 0).xyz);
	normalMap.x = 2 * normalMap.x - 1;
	normalMap.y = 2 * normalMap.y - 1;
	normalMap.z = 2 * normalMap.z - 1;
	
	//normal = HitAttribute(vertexNormals, attr);
	float3 biTangent = float3(0, 0, 0);
	float3 tangent = normalize(inputTangent - dot(inputTangent, inputNormal) * inputNormal);
	biTangent = normalize(inputBitangent - dot(inputBitangent, inputNormal) * inputNormal);
	
	float3x3 texSpace = float3x3(tangent, biTangent, inputNormal); //Create the "Texture Space"
	finalNormal = normalize(mul(normalMap, texSpace));
	return finalNormal;
}

bool GetIsWithinDimensions(float2 uvs)
{
	if (uvs.x < 0.0f || uvs.x > 1.0f || uvs.y < 0.0f || uvs.y > 1.0f)
	{
		return false;
	}
	return true;
}
//---------------Vertex Shader--------------------------------------------------------------------------------
VertexShaderOutput VertexMain(VertexShaderInput input)
{
	float4 localPosition = float4(input.localPosition, 1);
	float4 viewPosition = mul(viewMatrix, localPosition);
	float4 clipPosition = mul(projectionMatrix, viewPosition);
	float4 localNormal = float4(input.localNormal, 0);
	
	float4x4 bias = float4x4(float4(0.5f, 0.0f, 0.0f, 0.5f),
									float4(0.0f, -0.5f, 0.0f, 0.5f),
									float4(0.0f, 0.0f, 0.5f, 0.5f),
									float4(0.0f, 0.0f, 0.0f, 1.0f));
	
	float4 lightCoordinates = mul(lightViewMatrix, localPosition);
	float4 lightProjs = mul(lightProjMatrix, lightCoordinates);
	float4 shadowMap = lightProjs;
	
	VertexShaderOutput vsOutput;
	vsOutput.position = clipPosition;
	vsOutput.normal = localNormal.xyz;
	vsOutput.color = input.color;
	vsOutput.tangent = input.tangent;
	vsOutput.bitangent = input.bitangent;
	vsOutput.uv = input.uv;
	vsOutput.worldPosition = localPosition;
	vsOutput.ShadowMapping = shadowMap;

	return vsOutput;
}

//------------------------------------------------------------------------------------------------
float4 PixelMain(VertexShaderOutput input) : SV_Target0
{
	float4 diffuseValue = DiffuseTexture.SampleLevel(SimpleSampler, input.uv, 0);
	float4 metalValue = MetallicTexture.SampleLevel(SimpleSampler, input.uv, 0);
	float4 roughnessValue = RoughnessTexture.SampleLevel(SimpleSampler, input.uv, 0);
	
	float3 normalValue = normalize(input.normal);
	if (ViewX_GIOnY_ShadowPassZ_FrameTime.y == 1.0f)
	{
		normalValue = CalculateNormals(input.uv, normalValue, input.tangent, input.bitangent);
	}
	float3 lightDirection = globalLightPosition[0].xyz - input.worldPosition.xyz;
	float pointLightIntensity = max(dot(normalValue, lightDirection), 0.0f);
	
	lightDirection = globalLightPosition[1].xyz - input.worldPosition.xyz;
	pointLightIntensity += max(dot(normalValue, lightDirection), 0.0f);
	
	lightDirection = globalLightPosition[2].xyz - input.worldPosition.xyz;
	pointLightIntensity += max(dot(normalValue, lightDirection), 0.0f);
	
	float4 basicLighting = float4(( /*ambientIntensity + */ /*directionalIntensity +*/pointLightIntensity/4).xxx, 1) * diffuseValue;

	int lightNumber = 0;
	//-------------------PBR----------------------------
	float4 Light1Color = RenderingEquation(input.worldPosition.xyz, normalValue, input.uv,0);
	float4 Light2Color = RenderingEquation(input.worldPosition.xyz, normalValue, input.uv, 1);
	float4 Light3Color = RenderingEquation(input.worldPosition.xyz, normalValue, input.uv, 2);
	//float4 Light4Color = RenderingEquation(input.worldPosition.xyz, normalValue, input.uv, 3);

	float4 FinalColor = Light1Color + Light2Color + Light3Color ;

	if(renderOutput == 1)
	{
		FinalColor = diffuseValue;

	}
	else if (renderOutput == 2)
	{
		FinalColor = float4(normalValue, 1.0f);
		FinalColor = GetAbsolute(FinalColor);

	}
	else if (renderOutput == 3)
	{
		FinalColor = metalValue;
	}
	else if (renderOutput == 4)
	{
		FinalColor = roughnessValue;

	}
	else if (renderOutput == 5)
	{
		float4 absTangents = float4(input.tangent, 1);
		absTangents = GetAbsolute(absTangents);
		FinalColor = absTangents;

	}
	else if (renderOutput == 6)
	{
		float4 absBitangents = float4(input.bitangent, 1);
		absBitangents = GetAbsolute(absBitangents);
		FinalColor = absBitangents;
	}
	else if (renderOutput == 7)
	{
		FinalColor = basicLighting;
	}
	
	if (input.color.x !=0.0f)
	{
		return diffuseValue;

	}
	return FinalColor;

}
