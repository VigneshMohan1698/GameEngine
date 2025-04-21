

struct Vertex_PNCU
{
	float3 m_position;
	float3 m_normal;
	float4 m_color;
	float2 m_uvTexCoords;
};

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