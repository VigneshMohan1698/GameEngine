#include "ECSRenderingSystem.hpp"
#include <Engine/ECS/ECS.hpp>
#include <Engine/Renderer/ShadowMap.hpp>

extern EventSystem* g_theEventSystem;

ECSRenderingSystem::ECSRenderingSystem(ECS* ecs, RendererD12* renderer) : ECSSystem(ecs)
{
	m_ecs = ecs;
	m_renderer = renderer;

	m_shadowShader = m_renderer->CreateOrGetShader("Shadow3D", "Data/Shaders/Shadow3D.hlsl");
	m_engineShadowMap = new ShadowMap(m_renderer, m_renderer->GetDevice());
	g_theEventSystem->SubscribeEventCallbackObjectMethod("DebugKeyPressed", *this, &ECSRenderingSystem::DebugKeyPressed);
}

//---------------------------RENDERING SYSTEM---------------------------------------
void ECSRenderingSystem::Update(float deltaSeconds)
{
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
		RenderShadowPass(m_engineShadowMap, m_shadowShader);
		m_renderer->EndShadowMapRender(m_engineShadowMap);
	}
	
	if(mainCamera3D)
	{
		m_renderer->BeginRasterizerCamera(mainCamera3D->m_camera, m_engineShadowMap);
		//TO DO: If the entity is inactive don't render the mesh component.
		RenderCameraPass();
	}

	m_engineShadowMap->ClearShadows();
}

void ECSRenderingSystem::UpdateLightComponents()
{
	for (auto& pair : m_ecs->m_lightComponents)
	{
		LightComponent& lightComp = pair.second;
		if (lightComp.m_isShadowed && lightComp.IsDirty()) {
			TransformComponent* transformComponent = m_ecs->GetComponentOfType<TransformComponent>(pair.first);
			if (!transformComponent)
			{
				ERROR_AND_DIE("Light needs a transform component");
			}
			m_engineShadowMap->m_isEnabled = true;
			m_engineShadowMap->UpdateCameraSettingsAndRecalculateProjection(lightComp.m_fov, lightComp.znear, lightComp.zFar);
			m_engineShadowMap->UpdateCameraPosition(transformComponent->m_position, transformComponent->m_orientationDegrees);
			lightComp.ClearDirty();
			break;
		}
		
	}
}

void ECSRenderingSystem::RenderShadowPass(ShadowMap* shadowMap, ShaderD12* shader)
{
	for (auto& pair : m_ecs->m_meshComponents)
	{
		VertexNormalTangentArray verts;

		MeshComponent* meshComponent = &pair.second;
		if(!meshComponent->m_castsShadows) 
		{
			continue;
		}

		TransformComponent* transformComponent = m_ecs->GetComponentOfType<TransformComponent>(pair.first);

		//If the entity has no transform component, just draw the mesh as it is.
		if (transformComponent)
		{
			Mat44 modelMatrix = transformComponent->GetTransformMatrix();
			m_renderer->SetModelConstantData(modelMatrix, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			verts = meshComponent->m_mesh->GetTransformedVertices(transformComponent->GetTransformMatrix());
			//verts = meshComponent->m_mesh->m_cpuMesh->m_verticesWithTangent;
		}
		else if (meshComponent->m_isStatic)
		{
			m_renderer->SetModelConstantData(Mat44(), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			verts = meshComponent->m_mesh->m_cpuMesh->m_verticesWithTangent;
		} 
		else 
		{
			ERROR_AND_DIE("Dynamic mesh missing transform component. Either mark it as static or add transform component")
		}

		//Go through a list of mesh components 
		//Just draw them with the shadow shader.
		m_renderer->BindShader(shader);
		m_renderer->DrawIndexedVertexArray((int)verts.size(), verts, meshComponent->m_mesh->m_cpuMesh->m_indices);
	}
	m_renderer->SetModelConstantData(Mat44(), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void ECSRenderingSystem::RenderCameraPass()
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
			Mat44 modelMatrix = transformComponent->GetTransformMatrix();
			verts = meshComponent->m_mesh->GetTransformedVertices(modelMatrix);
		}
		else if (meshComponent->m_isStatic)
		{
			m_renderer->SetModelConstantData(Mat44(), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			verts = meshComponent->m_mesh->m_cpuMesh->m_verticesWithTangent;
		}
		else 
		{
			ERROR_AND_DIE("Dynamic mesh missing transform component. Either mark it as static or add transform component")
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

