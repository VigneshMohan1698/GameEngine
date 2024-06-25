#include "ECS.hpp"

void ECS::Startup()
{	
}

void ECS::Shutdown()
{
	for (auto entity : m_activeEntities) 
	{
		DestroyEntity(entity);
	}
}

void ECS::CreateEntity()
{
	m_nextEntityID++;
	m_activeEntities.insert(m_nextEntityID);
}

void ECS::DestroyEntity(const EntityID entityID)
{
	m_transformComponents.erase(entityID);
	m_activeEntities.erase(entityID);
}

template<typename T>
T* ECS::GetComponentOfType(const EntityID entityID)
{
	if (std::is_same<T, TransformComponent>)
	{
		if (m_transformComponents.find(entityID) != m_transformComponents.end())
		{
			return &m_transformComponents[entityID];
		}
	}
	return nullptr;
}

template<typename T>
void ECS::AddComponents(EntityID entityID, const T& component)
{
	if (std::is_same<T, TransformComponent>)
	{
		if (m_transformComponents.find(entityID) == m_transformComponents.end())
		{
			m_transformComponents.insert(entityID);
		} 
		else 
		{
			ERROR_AND_DIE("Component already exists in entity :" + (int)EntityID.to_string);
		}
	}
}
