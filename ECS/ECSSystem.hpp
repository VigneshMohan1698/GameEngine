#pragma once
#include <unordered_map>
#include <Engine/ECS/ECSComponents.hpp>
#include <Engine/Math/MathUtils.hpp>
#include <Engine/Renderer/RendererD12.hpp>
#include <unordered_set>
#include <memory>


class ECS;
class ECSSystem
{
public:
	ECSSystem() {}

	//Have to define all constructors here or can't overload register them in ..
	//RegisterRequiredSystems()
	ECSSystem(ECS* ecs, RendererD12* renderer) : m_ecs(ecs) {}
	ECSSystem(ECS* ecs, InputSystem* inputSytem) : m_ecs(ecs) {}

	~ECSSystem()
	{
		m_ecs = nullptr;
	};
	ECSSystem(ECS* ecs) : m_ecs(ecs) {}
	virtual void Update(float deltaSeconds) {};

	//Adding it here so that the g_theECS extern won't break other projects
	ECS* m_ecs = nullptr;
};

