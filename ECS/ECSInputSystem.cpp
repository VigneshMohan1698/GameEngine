#include "Engine/ECS/ECSInputSystem.hpp"
#include "Engine/ECS/ECS.hpp"
#include <Engine/Renderer/ShadowMap.hpp>

extern EventSystem* g_theEventSystem;
extern RendererD12* g_theRenderer;

//----------------------------INPUT SYSTEM--------------------------------
void ECSInputSystem::Update(float deltaSeconds)
{
	bool windowHasFocus = g_theRenderer->GetRenderConfig().m_window->HasFocus();

	if (m_inputSystem->WasKeyJustPressed(KEYCODE_SPACE))
	{
		m_mouseVisible = !m_mouseVisible;
	}

	if (!windowHasFocus)
	{
		m_inputSystem->SetMouseMode(false, false, false);
	}
	else if (windowHasFocus)
	{
		if (m_mouseVisible)
		{
			m_inputSystem->SetMouseMode(false, false, false);
		}
		else
		{
			m_inputSystem->SetMouseMode(true, true, true);
		}
	}

	for (auto& pair : m_ecs->m_cameraComponents)
	{
		//----------Update main camera if that's what the game is controlling-----------------

		if (pair.second.m_main3DCamera && m_ecs->GetControlledEntity() == pair.first)
		{	
			Camera& camera = pair.second.m_camera;
			//TO DO: Fix this
			camera.SetLookAtMatrix(Mat44(), false);
			Mat44 engineCameraMatrix = camera.GetModalMatrix();

			Vec3& engineCameraPosition = camera.m_position;
			EulerAngles& engineCameraOrientation = camera.m_orientation;

			ControlledEntityMovement(deltaSeconds, engineCameraMatrix, engineCameraPosition, engineCameraOrientation);

			//return once updated inputs
			return;
		}
	}

	for (auto& pair : m_ecs->m_transformComponents)
	{
		//----------Update the game controlled entity-----------------

		if ( m_ecs->GetControlledEntity() == pair.first)
		{
		    auto transformComponent = pair.second;
			Mat44 matrix = transformComponent.GetTransformMatrix();

			Vec3& position = pair.second.m_position;
			EulerAngles& orientation = pair.second.m_orientationDegrees;

			ControlledEntityMovement(deltaSeconds, matrix, position, orientation);

			//Dirty the transform component so that it's updated by the transform system
			transformComponent.MarkDirty();
			//return once updated inputs
			return;
		}
	}
}

void ECSInputSystem::ControlledEntityMovement(float deltaSeconds, Mat44& ModelMatrix, Vec3& entityPosition, EulerAngles& entityOrientation)
{
	float engineCameraVelocity = 5.0f;

	if (m_inputSystem->IsKeyDown(16) || m_inputSystem->GetController(0).IsButtonDown(XboxButtonID::XBOX_BUTTON_A))
	{
		engineCameraVelocity = 20.0f;
	}

	if (m_inputSystem->IsKeyDown(17) || m_inputSystem->GetController(0).IsButtonDown(XboxButtonID::XBOX_BUTTON_B))
	{
		engineCameraVelocity = 0.1f;
	}

	if (m_inputSystem->IsKeyDown('H') || m_inputSystem->GetController(0).IsButtonDown(XboxButtonID::XBOX_BUTTON_START))
	{
		entityPosition = Vec3(0.0f, 0.0f, 0.0f);
		entityOrientation = EulerAngles(0.0f, 0.0f, 0.0f);
	}

	if (m_inputSystem->IsKeyDown('W'))
	{
		entityPosition += ModelMatrix.GetIBasis3D() * deltaSeconds * engineCameraVelocity;
	}
	if (m_inputSystem->IsKeyDown('S'))
	{
		entityPosition -= ModelMatrix.GetIBasis3D() * deltaSeconds * engineCameraVelocity;
	}
	if (m_inputSystem->IsKeyDown('D'))
	{
		entityPosition -= ModelMatrix.GetJBasis3D() * deltaSeconds * engineCameraVelocity;
	}
	if (m_inputSystem->IsKeyDown('A'))
	{
		entityPosition += ModelMatrix.GetJBasis3D() * deltaSeconds * engineCameraVelocity;
	}
	if (m_inputSystem->IsKeyDown('Z'))
	{
		entityPosition += ModelMatrix.GetKBasis3D() * deltaSeconds * engineCameraVelocity;
	}
	if (m_inputSystem->IsKeyDown('C'))
	{

		entityPosition -= ModelMatrix.GetKBasis3D() * deltaSeconds * engineCameraVelocity;
	}


	//Shadow switch event

	EventArgs args;
	
	if (m_inputSystem->WasKeyJustPressed(KEYCODE_F1))
	{
		args.SetValue("KeyPressed", KEYCODE_F1);
		g_theEventSystem->FireEvent("DebugKeyPressed", args);
	}

	if (m_inputSystem->WasKeyJustPressed(KEYCODE_F2))
	{
		args.SetValue("KeyPressed", KEYCODE_F2);
		g_theEventSystem->FireEvent("DebugKeyPressed", args);
	}

	if (m_inputSystem->WasKeyJustPressed(KEYCODE_F3))
	{
		args.SetValue("KeyPressed", KEYCODE_F3);
		g_theEventSystem->FireEvent("DebugKeyPressed", args);
	}

	if (m_inputSystem->WasKeyJustPressed(KEYCODE_F4))
	{
		args.SetValue("KeyPressed", KEYCODE_F4);
		g_theEventSystem->FireEvent("DebugKeyPressed", args);
	}

	if (m_inputSystem->WasKeyJustPressed(KEYCODE_F5))
	{
		args.SetValue("KeyPressed", KEYCODE_F5);
		g_theEventSystem->FireEvent("DebugKeyPressed", args);
	}

	if (m_inputSystem->WasKeyJustPressed(KEYCODE_F6))
	{
		args.SetValue("KeyPressed", KEYCODE_F6);
		g_theEventSystem->FireEvent("DebugKeyPressed", args);
	}
	if (m_inputSystem->WasKeyJustPressed(KEYCODE_F7))
	{
		args.SetValue("KeyPressed", KEYCODE_F7);
		g_theEventSystem->FireEvent("DebugKeyPressed", args);
	}
	if (m_inputSystem->WasKeyJustPressed(KEYCODE_F8))
	{
		args.SetValue("KeyPressed", KEYCODE_F8);
		g_theEventSystem->FireEvent("DebugKeyPressed", args);
	}
	if (m_inputSystem->WasKeyJustPressed(KEYCODE_F9))
	{
		args.SetValue("KeyPressed", KEYCODE_F9);
		g_theEventSystem->FireEvent("DebugKeyPressed", args);
	}
	if (m_inputSystem->WasKeyJustPressed(KEYCODE_F10))
	{
		args.SetValue("KeyPressed", KEYCODE_F10);
		g_theEventSystem->FireEvent("DebugKeyPressed", args);
	}
	if (m_inputSystem->WasKeyJustPressed(KEYCODE_F11))
	{
		args.SetValue("KeyPressed", KEYCODE_F11);
		g_theEventSystem->FireEvent("DebugKeyPressed", args);
	}

	Vec2 mouseDelta = m_inputSystem->GetMouseClientDelta();
	if(mouseDelta.x != 0 || mouseDelta.y != 0)
	{
		entityOrientation.m_yawDegrees += (mouseDelta.x * 0.1f);
		entityOrientation.m_pitchDegrees -= (mouseDelta.y * 0.1f);
	}

	//Ignore look at matrix once we start moving camera
	//TO DO: Fix this to use look at matrix
	/*engineCamera.SetLookAtMatrix(Mat44(), false);
	if(engineCameraPosition != engineCamera.m_position || engineCameraOrientation != engineCamera.m_orientation)
	{
		engineCamera.SetLookAtMatrix(Mat44(), false);
	}*/

}