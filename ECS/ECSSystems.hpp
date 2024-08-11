#pragma once
#include <unordered_map>
#include "ECS.hpp"
#include <Engine\Renderer\RendererD12.hpp>

class ECS;
class ECSSystem
{
	public:	
	ECSSystem() {}
	ECSSystem(ECS* ecs, RendererD12* renderer) : m_ecs(ecs), m_renderer(renderer) {}
	~ECSSystem() 
	{
		m_ecs = nullptr;
		m_renderer = nullptr;
	};

	virtual void Update() {};

	//Adding it here so that the g_theECS extern won't break other projects
	ECS* m_ecs = nullptr;
	RendererD12* m_renderer = nullptr;

};

class ECSRenderingSystem : public ECSSystem
{
	public:

	ECSRenderingSystem(ECS* ecs, RendererD12* renderer) : ECSSystem (ecs, renderer) {}
	~ECSRenderingSystem() {};

	void Update() override;

	//Updates all camera components. Looks for a main camera and uses that for rendering.
	void Update3DCameraComponents(const bool updatingUICamera = false);

	//Goes through a list of all entities and renders them if they have a Mesh -- First thought
	//Goes through a list of mesh components and renders them if the entity is active  -- second thought
	void RenderEntities();
};
