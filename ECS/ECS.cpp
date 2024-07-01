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
			return (T*) & m_transformComponents[entityID];
		}
	}
	if (std::is_same<T, MeshComponent>::value)
	{
		if (m_meshComponents.find(entityID) != m_meshComponents.end())
		{
			return (T*)&m_meshComponents[entityID];
		}
	}
	if (std::is_same<T, CameraComponent>::value)
	{
		if (m_cameraComponents.find(entityID) != m_cameraComponents.end())
		{
			return (T*)&m_cameraComponents[entityID];
		}
	}

	return nullptr;
}

template<typename T>
T* ECS::AddComponentToEntity(EntityID entityID)
{
	if (std::is_same<T, TransformComponent>::value)
	{
		if (m_transformComponents.find(entityID) == m_transformComponents.end())
		{
			m_transformComponents[entityID] = TransformComponent();
			return(T*)&m_transformComponents[entityID];
		} 
		else 
		{
			ERROR_AND_DIE("Transform Component already exists in entity " + std::to_string(entityID));
			return nullptr;
		}
	}
	if (std::is_same<T, MeshComponent>::value)
	{
		if (m_meshComponents.find(entityID) == m_meshComponents.end())
		{
			m_meshComponents[entityID] = MeshComponent();
			return (T*)&m_meshComponents[entityID];
		}
		else
		{
			ERROR_AND_DIE("Mesh Component already exists in entity " + std::to_string(entityID));
			return nullptr;
		}
	}
	if (std::is_same<T, CameraComponent>::value)
	{
		if (m_cameraComponents.find(entityID) == m_cameraComponents.end())
		{
			m_cameraComponents[entityID] = CameraComponent();
			return (T*)&m_cameraComponents[entityID];
		}
		else
		{
			ERROR_AND_DIE("Camera Component already exists in entity " + std::to_string(entityID));
		}
	}
	return nullptr;
}
