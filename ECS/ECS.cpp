#include "ECS.hpp"
#include <Engine\Core\ErrorWarningAssert.hpp>

void ECS::Startup()
{	
}

void ECS::Shutdown()
{
	for (auto& entity : m_activeEntities) 
	{
		DestroyEntity(entity);
	}
	m_activeEntities.clear();
	}

EntityID ECS::CreateEntity()
{
	m_nextEntityID++;
	m_activeEntities.insert(m_nextEntityID);
	return m_nextEntityID;
}

void ECS::DestroyEntity(const EntityID entityID)
{
	m_transformComponents.erase(entityID);

}

void ECS::DebugPrintEntityInformation()
{
	for (auto& entity : m_activeEntities)
	{
		DebuggerPrintf(Stringf("Enitity ID : %i", entity).c_str());
	}
}

template<typename T>
T* ECS::GetComponentOfType(const EntityID entityID)
{
	if (std::is_same<T, TransformComponent>::value)
	{
		if (m_transformComponents.find(entityID) != m_transformComponents.end())
		{
			return &m_transformComponents[entityID];
		}
	}
	return nullptr;
}

template<typename T>
void ECS::AddComponentToEntity(EntityID entityID)
{
	if (std::is_same<T, TransformComponent>::value)
	{
		if (m_transformComponents.find(entityID) == m_transformComponents.end())
		{
			m_transformComponents[entityID] = TransformComponent();
		} 
		else 
		{
			ERROR_AND_DIE("Component already exists in entity %i:",(int)EntityID.to_string);
		}
	}
}
