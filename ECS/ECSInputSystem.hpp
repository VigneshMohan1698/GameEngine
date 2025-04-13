#pragma once
#include <unordered_map>
#include <Engine/Renderer/RendererD12.hpp>
#include <Engine/Input/InputSystem.hpp>
#include "Engine/ECS/ECSSystem.hpp"

class ECS;
class ECSInputSystem : public ECSSystem
{
	public:
	ECSInputSystem(ECS* ecs, InputSystem* inputSytem) : ECSSystem(ecs)
	{
		m_inputSystem = inputSytem;
	}
	
	~ECSInputSystem() {
		m_inputSystem = nullptr;
	};

	void Update(float deltaSeconds) override;

	//Cameras are special.
	//They don't have a transform component.
	//The camera component has a position and orientation
	//Which is then used in BeginRasterizerCamera. 
	void ControlledEntityMovement(float deltaSeconds, Mat44& ModelMatrix, Vec3& entityPosition, EulerAngles& entityOrientation);

	private:
	InputSystem* m_inputSystem = nullptr;
};