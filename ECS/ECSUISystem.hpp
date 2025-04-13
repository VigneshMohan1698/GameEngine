#pragma once

#include <unordered_map>
#include <Engine/ECS/ECSRenderingSystem.hpp>
#include <Engine/Renderer/RendererD12.hpp>
#include <Engine/Input/InputSystem.hpp>
#include <Engine/ECS/ECSComponents.hpp>
#include "Engine/ECS/ECSSystem.hpp"

class ECS;
class ECSUISystem : public ECSSystem
{
public:
	ECSUISystem(ECS* ecs, RendererD12* renderer);

	~ECSUISystem() {
		m_renderer = nullptr;

		delete m_font;
		m_font = nullptr;
	};

	void Update(float deltaSeconds) override;
	void RenderUI(CameraComponent* uiCamera);

private:
	RendererD12*		    m_renderer = nullptr;
	BitmapFont*			    m_font = nullptr;
	VertexArray		        m_game2DVertices;
};