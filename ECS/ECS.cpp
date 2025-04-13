#include "ECS.hpp"
#include <Engine/Core/ErrorWarningAssert.hpp>
#include <Engine/ECS/ECS.hpp>
#include <Engine/Core/EventSystem.hpp>

#include <Engine/Renderer/RendererD12.hpp>
#include "ECSUISystem.hpp"
//TO DO: Is this bad? Maybe pass it in constructor?

extern RendererD12* g_theRenderer;
extern InputSystem* g_theInputSystem;
extern EventSystem* g_theEventSystem;

void ECS::Startup()
{	
	RegisterRequiredSystems();
}

void ECS::Update(float deltaSeconds)
{
	
	for (auto& system : m_ecsSystems)
	{
		system->Update(deltaSeconds);
	}

	g_theRenderer->Present();
	g_theRenderer->MoveToNextFrame();

}

void ECS::Shutdown()
{
	for (auto& entity : m_activeEntities) 
	{
		DestroyEntity(entity);
	}
	m_activeEntities.clear();
	
	//Systems are unique pointers so don't have to worry about deleting them
}

void ECS::RegisterRequiredSystems()
{
	m_ecsSystems.push_back(std::make_unique<ECSInputSystem>(this, g_theInputSystem));
	m_ecsSystems.push_back(std::make_unique<ECSRenderingSystem>(this, g_theRenderer));
	m_ecsSystems.push_back(std::make_unique<ECSUISystem>(this, g_theRenderer));
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
	m_meshComponents.erase(entityID);
	m_cameraComponents.erase(entityID);
	m_lightComponents.erase(entityID);
	m_UITextComponents.erase(entityID);
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
	if (std::is_same<T, LightComponent>::value)
	{
		if (m_lightComponents.find(entityID) != m_lightComponents.end())
		{
			return (T*)&m_lightComponents[entityID];
		}
	}
	if (std::is_same<T, UITextComponent>::value)
	{
		if (m_UITextComponents.find(entityID) != m_UITextComponents.end())
		{
			return (T*)&m_UITextComponents[entityID];
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

	if (std::is_same<T, LightComponent>::value)
	{
		if (m_lightComponents.find(entityID) == m_lightComponents.end())
		{
			m_lightComponents[entityID] = LightComponent();
			return (T*)&m_lightComponents[entityID];
		}
		else
		{
			ERROR_AND_DIE("Light Component already exists in entity " + std::to_string(entityID));
		}
	}

	if (std::is_same<T, UITextComponent>::value)
	{
		if (m_UITextComponents.find(entityID) == m_UITextComponents.end())
		{
			m_UITextComponents[entityID] = UITextComponent();
			return (T*)&m_UITextComponents[entityID];
		}
		else
		{
			ERROR_AND_DIE("UI text Component already exists in entity " + std::to_string(entityID));
		}
	}

	return nullptr;
}
