#pragma once
#include <unordered_map>
#include <Engine\ECS\ECSComponents.hpp>
#include <Engine\Math\MathUtils.hpp>
#include <unordered_set>

typedef unsigned int EntityID;

class ECS
{ 
	public:
		void Startup();
		void Shutdown();

		EntityID CreateEntity();
		void DestroyEntity(const EntityID entityID);

		template<typename T>
		T* GetComponentOfType(const EntityID entityID);

		template<typename T>
		void AddComponentToEntity(EntityID entityID);

		void DebugPrintEntityInformation();

	private:
		EntityID m_nextEntityID = 0;
		std::unordered_set<EntityID> m_activeEntities;
		std::unordered_map<EntityID, TransformComponent> m_transformComponents;
		std::unordered_map<EntityID, MeshComponent> m_meshComponents;
};
