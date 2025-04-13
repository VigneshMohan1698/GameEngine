#pragma once
#include <unordered_map>
#include <Engine/Renderer/RendererD12.hpp>
#include <Engine/Input/InputSystem.hpp>
#include "Engine/ECS/ECSSystem.hpp"

class ECS;
class ECSRenderingSystem : public ECSSystem
{
public:
	ECSRenderingSystem(ECS* ecs, RendererD12* renderer);

	~ECSRenderingSystem() {
		m_renderer = nullptr;

		delete m_shadowShader;
		m_shadowShader = nullptr;

		delete m_engineShadowMap;
		m_engineShadowMap = nullptr;
	};

	void Update(float deltaSeconds) override;

	//Updates all camera components. Looks for a main camera and uses that for rendering.
	void UpdateCameraComponents();

	//Go through lights and render shadow maps
	void UpdateLightComponents();

	void RenderCameraPass();
	void RenderShadowPass(ShadowMap* shadowMap, ShaderD12* shadowShader);

	//Event system subscriptions
	void DebugKeyPressed(EventArgs& args);

private:
	RendererD12* m_renderer = nullptr;

	ShaderD12* m_shadowShader = nullptr;
	ShadowMap* m_engineShadowMap = nullptr;
};
