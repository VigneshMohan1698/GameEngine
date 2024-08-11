#pragma once
#include <unordered_map>
#include <Engine\ECS\ECSComponents.hpp>
#include <Engine\Math\MathUtils.hpp>
#include <unordered_set>
#include "ECSSystems.hpp"
#include <memory>

typedef unsigned int EntityID;

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

		void DebugPrintEntityInformation();

	public:
		EntityID m_nextEntityID = 0;
		std::vector<std::unique_ptr<ECSSystem>>  m_ecsSystems;
		std::unordered_set<EntityID> m_activeEntities;
		std::unordered_map<EntityID, TransformComponent> m_transformComponents;
		std::unordered_map<EntityID, MeshComponent> m_meshComponents;
		std::unordered_map<EntityID, CameraComponent> m_cameraComponents;
};
