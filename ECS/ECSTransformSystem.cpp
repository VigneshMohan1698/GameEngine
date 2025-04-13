#include "ECSTransformSystem.hpp"
#include <Engine/Renderer/ShadowMap.hpp>
#include <Engine/ECS/ECS.hpp>

extern EventSystem* g_theEventSystem;

//----------------------------INPUT SYSTEM--------------------------------
void ECSTransformSystem::Update(float deltaSeconds)
{
	for (auto& pair : m_ecs->m_transformComponents)
	{
		TransformComponent* transformComponent = &pair.second;
		if (transformComponent->IsDirty()) {
			transformComponent->CalculateAndCacheTransformMatrix();
			transformComponent->ClearDirty();
		}	
	}
}
