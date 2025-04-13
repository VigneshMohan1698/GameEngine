#include "ECSSystems.hpp"
#include <Engine/ECS/ECS.hpp>
#include <Engine/Renderer/ShadowMap.hpp>

extern EventSystem* g_theEventSystem;

ECSRenderingSystem::ECSRenderingSystem(ECS* ecs, RendererD12* renderer)
{
	m_ecs = ecs;
	m_renderer = renderer;

	IntVec2 shadowMapDimensions(m_renderer->m_dimensions);
	m_shadowShader = m_renderer->CreateOrGetShader("Shadow3D", "Data/Shaders/Shadow3D.hlsl");
	m_engineShadowMap = new ShadowMap(m_renderer, m_renderer->GetDevice(), shadowMapDimensions.x, shadowMapDimensions.y);
	g_theEventSystem->SubscribeEventCallbackObjectMethod("DebugKeyPressed", *this, &ECSRenderingSystem::DebugKeyPressed);
}

//---------------------------RENDERING SYSTEM---------------------------------------
void ECSRenderingSystem::Update(float deltaSeconds)
{
	UNUSED((void) deltaSeconds);
	m_renderer->Prepare();
	m_renderer->ClearScreen(Rgba8::BLACK);
   
   //Update the light components first for shadow maps
	UpdateLightComponents();

	UpdateCameraComponents();
}

void ECSRenderingSystem::UpdateCameraComponents()
{
	//Go through a list of all cameracomponents
	//If it's UI camera update - set ui main camera buffer for gpu
	//else set 3D main camera buffer for gpu
	CameraComponent* mainCamera3D = nullptr;

	for (auto& pair : m_ecs->m_cameraComponents)
	{
		if (pair.second.m_main3DCamera)
		{
			mainCamera3D = &pair.second;
		}
	}

	//----------------------SHADOW PRE-PASS--------------------
	bool shadowMapFeatureEnabled = m_ecs->GetEngineState().m_enabledFeatures.shadowMapEnabled;

	if(m_engineShadowMap->m_isEnabled && shadowMapFeatureEnabled)
	{
		m_renderer->BeginShadowMapRender(m_engineShadowMap);
		Render3DEntitiesShadows(m_engineShadowMap, m_shadowShader);
		m_renderer->EndShadowMapRender(m_engineShadowMap);
	}
	
	if(mainCamera3D)
	{
		m_renderer->BeginRasterizerCamera(mainCamera3D->m_camera, m_engineShadowMap);
		//TO DO: If the entity is inactive don't render the mesh component.
		Render3DEntities();
	}

	m_engineShadowMap->ClearShadows();
}

void ECSRenderingSystem::UpdateLightComponents()
{
	for (auto& pair : m_ecs->m_lightComponents)
	{
		TransformComponent* transformComponent = m_ecs->GetComponentOfType<TransformComponent>(pair.first);
		if (!transformComponent)
		{
			ERROR_AND_DIE("Light needs a transform component");
		}
		m_engineShadowMap->m_isEnabled = true;
		m_engineShadowMap->UpdateCameraPosition(transformComponent->m_position, transformComponent->m_orientationDegrees);
	}
}

void ECSRenderingSystem::Render3DEntities()
{
	//Go through a list of mesh components 
	//Get their material which has shader and texture
	//Bind shaders and textures
	for(auto& pair : m_ecs->m_meshComponents)
	{
		VertexNormalTangentArray verts;

		MeshComponent* meshComponent = &pair.second;
		TransformComponent* transformComponent = m_ecs->GetComponentOfType<TransformComponent>(pair.first);

		if (meshComponent->m_material.GetMaterialType() == MaterialType::NoMaterial)
		{
			ERROR_AND_DIE("Mesh Component has no material assigned");
		}

		//If the entity has no transform component, just draw the mesh as it is.
		if (transformComponent)
		{
			/*Mat44 modelMatrix = transformComponent->GetTransformMatrix();
			m_renderer->SetModelConstantData(modelMatrix, Vec4(1.0f, 0.0f, 1.0f, 0.0f));
			verts = meshComponent->m_mesh->m_cpuMesh->m_verticesWithTangent;*/
			meshComponent->m_mesh->SetTransform(transformComponent->GetTransformMatrix());
			verts = meshComponent->m_mesh->GetTransformedVertices(transformComponent->GetTransformMatrix());
		}
		else
		{
			m_renderer->SetModelConstantData(Mat44(), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			verts = meshComponent->m_mesh->m_cpuMesh->m_verticesWithTangent;
		}

		//If the entity mesh has no material ignore textures.
		ShaderD12* shader = meshComponent->m_material.GetShader();
		m_renderer->BindShader(shader);

		if (meshComponent->m_material.GetMaterialType() != MaterialType::Basic3DNoColor)
		{
			int albedoTextureIndex = meshComponent->m_material.GetAlbedoTextureIndex();
			TextureD12* sphereTexture = m_renderer->GetTextureAtIndex(0);
			m_renderer->BindTexture(0, albedoTextureIndex);
		}

		m_renderer->BindHandle(1, m_engineShadowMap->GetShaderResourceBuffer()->gpuReadDescriptorHandle);
		m_renderer->DrawIndexedVertexArray((int)verts.size(), verts, meshComponent->m_mesh->m_cpuMesh->m_indices);
	}
	m_renderer->SetModelConstantData(Mat44(), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_renderer->FinishUpGPUWork();

	//TO DO: Remove these 2 lines from game and uncomment here: 
	//m_renderer->Present();
	//m_renderer->MoveToNextFrame();

	//Finish up rendering. Reset command allocator, Present frame buffer and move to next frame
}

void ECSRenderingSystem::Render3DEntitiesShadows(ShadowMap* shadowMap, ShaderD12* shader)
{
	for (auto& pair : m_ecs->m_meshComponents)
	{
		VertexNormalTangentArray verts;

		MeshComponent* meshComponent = &pair.second;
		TransformComponent* transformComponent = m_ecs->GetComponentOfType<TransformComponent>(pair.first);

		//If the entity has no transform component, just draw the mesh as it is.
		if (transformComponent)
		{
			Mat44 modelMatrix = transformComponent->GetTransformMatrix();
			m_renderer->SetModelConstantData(modelMatrix, Vec4(1.0f,1.0f,1.0f,1.0f));
			meshComponent->m_mesh->SetTransform(transformComponent->GetTransformMatrix());
			verts = meshComponent->m_mesh->GetTransformedVertices(transformComponent->GetTransformMatrix());
			//verts = meshComponent->m_mesh->m_cpuMesh->m_verticesWithTangent;
		}
		else
		{
			m_renderer->SetModelConstantData(Mat44(), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			verts = meshComponent->m_mesh->m_cpuMesh->m_verticesWithTangent;
		}

		//Go through a list of mesh components 
		//Just draw them with the shadow shader.
		m_renderer->BindShader(shader);
		m_renderer->DrawIndexedVertexArray((int)verts.size(), verts, meshComponent->m_mesh->m_cpuMesh->m_indices);
	}
	m_renderer->SetModelConstantData(Mat44(), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void ECSRenderingSystem::DebugKeyPressed(EventArgs& args)
{
	unsigned char keyCode = args.GetValue<unsigned char>("KeyPressed", 0);
	Vec4& debugDataValues = m_renderer->m_cameraCB->debugData;
	EngineState& engineState = m_ecs->GetEngineState();
	if(keyCode == KEYCODE_F1) {
		debugDataValues.x += 1.0f;
	}
	else if (keyCode == KEYCODE_F2) {
		debugDataValues.y += 1.0f;
	}
	else if (keyCode == KEYCODE_F3) {
		debugDataValues.z += 1.0f;
	}
	else if (keyCode == KEYCODE_F4) {
		debugDataValues.w += 1.0f;
	}
	else if (keyCode == KEYCODE_F5) {
		engineState.m_enabledFeatures.debugRendererEnabled = !engineState.m_enabledFeatures.debugRendererEnabled;
	}
	else if (keyCode == KEYCODE_F6) {
		engineState.m_enabledFeatures.shadowMapEnabled = !engineState.m_enabledFeatures.shadowMapEnabled;
	}
	else if (keyCode == KEYCODE_F11) {
		debugDataValues = Vec4();
	}
}

//----------------------------INPUT SYSTEM--------------------------------
void ECSInputSystem::Update(float deltaSeconds)
{
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
			Mat44 matrix = pair.second.GetTransformMatrix();

			Vec3& position = pair.second.m_position;
			EulerAngles& orientation = pair.second.m_orientationDegrees;

			ControlledEntityMovement(deltaSeconds, matrix, position, orientation);

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