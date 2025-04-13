#pragma once
#include <unordered_map>
#include <Engine/Renderer/RendererD12.hpp>
#include <Engine/Input/InputSystem.hpp>
#include "Engine/ECS/ECSSystem.hpp"

class ECS;
class ECSTransformSystem : public ECSSystem
{
	public:
	ECSTransformSystem(ECS* ecs) : ECSSystem(ecs)
	{		
	}
	
	~ECSTransformSystem() {
	};

	void Update(float deltaSeconds) override;
};