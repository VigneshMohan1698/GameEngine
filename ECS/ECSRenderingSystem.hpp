#pragma once
#include <unordered_map>
#include "ECS.hpp"
#include <Engine\Renderer\RendererD12.hpp>

class ECSRenderingSystem
{
	public:
	ECSRenderingSystem(ECS* ecs, RendererD12* renderer);
	~ECSRenderingSystem();

	//Goes through a list of all entities and renders them if they have a Mesh -- First thought
	//Goes through a list of mesh components and renders them if the entity is active  -- second thought
	void RenderEntities(std::unordered_map<EntityID, MeshComponent>& meshComponents);

	void Update3DCameraComponents(std::unordered_map<EntityID, CameraComponent>& cameraComponents, const bool updatingUICamera = false);

	public:
	//Adding it here so that the g_theECS extern won't break other projects
	ECS* m_ecs = nullptr; 
	RendererD12* m_renderer = nullptr;
};
