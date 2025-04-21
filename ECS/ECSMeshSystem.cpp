#include "ECSMeshSystem.hpp"
#include <Engine/ECS/ECS.hpp>
#include <Engine/Renderer/ShadowMap.hpp>

extern EventSystem* g_theEventSystem;

ECSMeshSystem::ECSMeshSystem(ECS* ecs, RendererD12* renderer) : ECSSystem(ecs)
{
	m_ecs = ecs;
	m_renderer = renderer;
}

//---------------------------RENDERING SYSTEM---------------------------------------
void ECSMeshSystem::Update(float deltaSeconds)
{
	for (auto& pair : m_ecs->m_meshComponents)
	{
		MeshComponent& component = pair.second;
		if(!component.IsDirty()) {
			continue;
		}
	}
}

