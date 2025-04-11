#pragma once
#include <unordered_map>
#include <Engine\ECS\ECSComponents.hpp>
#include <Engine\Math\MathUtils.hpp>
#include <unordered_set>
#include <Engine/ECS/ECSSystems.hpp>
#include <memory>

typedef unsigned int EntityID;
static const EntityID kInvalidEntityID = -1;

class ECSSystem;

class ECS
{ 
	public:
		void Startup();
		void Shutdown();

		void RegisterRequiredSystems();

		//Updates all ECS Systems
		//If it's a rendering system, it will render to the framebuffer
		void Update(float deltaSeconds);

		EntityID CreateEntity();
		void DestroyEntity(const EntityID entityID);

		template<typename T>
		T* GetComponentOfType(const EntityID entityID);

		template<typename T>
		T* AddComponentToEntity(EntityID entityID);


		//-------------SYSTEMS---------------------

		void DebugPrintEntityInformation();
		void SetControlledEntity(EntityID entityID)
		{
			m_controlledEntity = entityID;
		}

		EntityID GetControlledEntity()
		{
			return m_controlledEntity;
		}

	public:
		EntityID m_nextEntityID = 0;
		//The entity that the main keyboard is controlling.
		EntityID m_controlledEntity = 0;

		std::vector<std::unique_ptr<ECSSystem>>  m_ecsSystems;
		std::unordered_set<EntityID> m_activeEntities;
		std::unordered_map<EntityID, TransformComponent> m_transformComponents;
		std::unordered_map<EntityID, MeshComponent> m_meshComponents;
		std::unordered_map<EntityID, CameraComponent> m_cameraComponents;
		std::unordered_map<EntityID, LightComponent> m_lightComponents;
		std::unordered_map<EntityID, UITextComponent> m_UITextComponents;
};
