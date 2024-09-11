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
	ECSRenderingSystem(ECS* ecs, RendererD12* renderer);

	~ECSRenderingSystem() {
		m_renderer = nullptr;

		delete m_font;
		m_font = nullptr;

		delete m_shadowShader;
		m_shadowShader = nullptr;

		delete m_engineShadowMap;
		m_engineShadowMap = nullptr;
	};

	void Update(float deltaSeconds) override;

	//Updates all camera components. Looks for a main camera and uses that for rendering.
	void UpdateCameraComponents();

	//Go through lights and render shadow maps
	void UpdateLightComponents();

	void Render3DEntities();
	void Render3DEntitiesShadows(ShadowMap* shadowMap, ShaderD12* shadowShader);

	void Render2DUI();

private:
	RendererD12* m_renderer = nullptr;
	BitmapFont* m_font = nullptr;

	ShaderD12* m_shadowShader = nullptr;
	ShadowMap* m_engineShadowMap = nullptr;
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
	void ControlledEntityMovement(float deltaSeconds, Mat44& ModelMatrix, Vec3& entityPosition, EulerAngles& entityOrientation);

	private:
	InputSystem* m_inputSystem = nullptr;
};