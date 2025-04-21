#pragma once
#include <unordered_map>
#include <Engine/Renderer/RendererD12.hpp>
#include <Engine/Input/InputSystem.hpp>
#include "Engine/ECS/ECSSystem.hpp"

class ECS;
class ECSMeshSystem : public ECSSystem
{
public:
	ECSMeshSystem(ECS* ecs, RendererD12* renderer);

	~ECSMeshSystem() {
		m_renderer = nullptr;
	};

	void Update(float deltaSeconds) override;

private:
	RendererD12* m_renderer = nullptr;
};
