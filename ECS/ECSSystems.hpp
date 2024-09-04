#pragma once
#include <unordered_map>
#include <Engine/Renderer/RendererD12.hpp>
#include <Engine/Input/InputSystem.hpp>


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

class ECSRenderingSystem : public ECSSystem
{
	public:
	ECSRenderingSystem(ECS* ecs, RendererD12* renderer) : ECSSystem (ecs) 
	{
		m_renderer = renderer;
	}

	~ECSRenderingSystem() {
		m_renderer = nullptr;
	};

	void Update(float deltaSeconds) override;

	//Updates all camera components. Looks for a main camera and uses that for rendering.
	void Update3DCameraComponents(const bool updatingUICamera = false);

	void RenderEntities();

	private:
	RendererD12* m_renderer = nullptr;
};

class ECSLightingSystem : public ECSSystem
{
	public:
	ECSLightingSystem(ECS* ecs, RendererD12* renderer) : ECSSystem(ecs) 
	{
		m_renderer = renderer;
	}

	~ECSLightingSystem() {};


	void Update(float deltaSeconds) override;

	private:
	RendererD12* m_renderer = nullptr;
};

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
	void EngineCameraMovement(float deltaSeconds, Camera& engineCamera);

	private:
	InputSystem* m_inputSystem = nullptr;
};