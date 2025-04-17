#pragma once

#include <Engine/Math/Mat44.hpp>
#include <Engine/Math/Vec4.hpp>
#include <Engine/Math/Vec2.hpp>

const static IntVec2 ShadowMapResolution = IntVec2(1024,1024);

struct FeaturesEnabled
{
	bool debugRendererEnabled = false;
	bool shadowMapEnabled = true;
};

struct CameraConstantBuffer
{
	Mat44 projectionMatrix;
	Mat44 viewMatrix;
	Mat44 lightViewMatrix;
	Mat44 lightProjMatrix;
	Vec4 cameraPosition;
	Vec4 debugData;
};

struct EngineDataBuffer
{
	Vec4 globalLightPosition[4];
	Vec4 globalLightDirection[4];
	Vec4 globalLightColor[4];
	Vec4 ViewX_GIOnY_ShadowPassZ_FrameTime;
	float  renderOutput; // ------- 0 final output 1 Normals 
};


struct EngineState
{
	FeaturesEnabled m_enabledFeatures;
	EngineDataBuffer m_engineDataBuffer;
	float			m_frameTime;
};