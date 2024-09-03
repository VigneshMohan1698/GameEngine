#include "ECSSystems.hpp"
#include <Engine/ECS/ECS.hpp>

//---------------------------RENDERING SYSTEM---------------------------------------
void ECSRenderingSystem::Update(float deltaSeconds)
{
	UNUSED((void) deltaSeconds);
	m_renderer->Prepare();
	m_renderer->ClearScreen(Rgba8::BLACK);
   
	//TO DO: Add 2D UI content.
	Update3DCameraComponents( false);
	
	//TO DO: If the entity is inactive don't render the mesh component.
	RenderEntities();
	
}

void ECSRenderingSystem::Update3DCameraComponents(const bool updatingUICamera)
{
	//Go through a list of all cameracomponents
	//If it's UI camera update - set ui main camera buffer for gpu
	//else set 3D main camera buffer for gpu
	for (auto& pair : m_ecs->m_cameraComponents)
	{
		if (updatingUICamera && pair.second.m_mainUICamera)
		{
			m_renderer->BeginRasterizerCamera(pair.second.m_camera);
		}
		else if (pair.second.m_main3DCamera)
		{
			m_renderer->BeginRasterizerCamera(pair.second.m_camera);
		}
	}
}

void ECSRenderingSystem::RenderEntities()
{
	
	//Go through a list of mesh components 
	//Get their material which has shader and texture
	//Bind shaders and textures
	for(auto& pair : m_ecs->m_meshComponents)
	{
		VertexNormalTangentArray verts;

		MeshComponent* meshComponent = &pair.second;
		TransformComponent* transformComponent = m_ecs->GetComponentOfType<TransformComponent>(pair.first);

		ShaderD12* shader = meshComponent->m_material.GetShader();
		m_renderer->BindShader(shader);

		//If the entity mesh has no material ignore textures.
		if(meshComponent->m_material.GetMaterialType() != MaterialType::NoMaterial )
		{
			int albedoTextureIndex = meshComponent->m_material.GetAlbedoTextureIndex();
			TextureD12* sphereTexture = m_renderer->GetTextureAtIndex(0);
			m_renderer->BindTexture(0, albedoTextureIndex);
		}

		

		//If the entity has no transform component, just draw the mesh as it is.
		if (transformComponent)
		{
			meshComponent->m_mesh.SetTransform(transformComponent->GetTransformMatrix());
			verts = meshComponent->m_mesh.GetTransformedVertices(transformComponent->GetTransformMatrix());
		}
		else 
		{
			verts = meshComponent->m_mesh.m_cpuMesh->m_verticesWithTangent;
		}

		
		m_renderer->DrawIndexedVertexArray((int)verts.size(), verts, meshComponent->m_mesh.m_cpuMesh->m_indices);
	}
	
	//Finish up rendering. Reset command allocator, Present frame buffer and move to next frame
	m_renderer->FinishUpGPUWork();
	m_renderer->Present();
	m_renderer->MoveToNextFrame();
}


//----------------------------INPUT SYSTEM--------------------------------
void ECSInputSystem::Update(float deltaSeconds)
{
	for (auto& pair : m_ecs->m_cameraComponents)
	{
		if (pair.second.m_main3DCamera)
		{	
			EngineCameraMovement(deltaSeconds, pair.second.m_camera);
		}
	}
}

void ECSInputSystem::EngineCameraMovement(float deltaSeconds, Camera& engineCamera)
{
	Mat44 engineCameraMatrix = engineCamera.GetModalMatrix();

	float engineCameraVelocity = 5.0f;
	Vec3 engineCameraPosition = engineCamera.m_position;
	EulerAngles engineCameraOrientation = engineCamera.m_orientation;

	if (m_inputSystem->IsKeyDown(16) || m_inputSystem->GetController(0).IsButtonDown(XboxButtonID::XBOX_BUTTON_A))
	{
		engineCameraVelocity = 20.0f;
	}

	if (m_inputSystem->IsKeyDown('H') || m_inputSystem->GetController(0).IsButtonDown(XboxButtonID::XBOX_BUTTON_START))
	{
		engineCameraPosition = Vec3(0.0f, 0.0f, 0.0f);
		engineCameraOrientation = EulerAngles(0.0f, 0.0f, 0.0f);
	}

	if (m_inputSystem->IsKeyDown('W'))
	{
		engineCameraPosition += engineCameraMatrix.GetIBasis3D() * deltaSeconds * engineCameraVelocity;
	}
	if (m_inputSystem->IsKeyDown('S'))
	{
		engineCameraPosition -= engineCameraMatrix.GetIBasis3D() * deltaSeconds * engineCameraVelocity;
	}
	if (m_inputSystem->IsKeyDown('D'))
	{
		engineCameraPosition -= engineCameraMatrix.GetJBasis3D() * deltaSeconds * engineCameraVelocity;
	}
	if (m_inputSystem->IsKeyDown('A'))
	{
		engineCameraPosition += engineCameraMatrix.GetJBasis3D() * deltaSeconds * engineCameraVelocity;
	}
	if (m_inputSystem->IsKeyDown('Z'))
	{
		engineCameraPosition += engineCameraMatrix.GetKBasis3D() * deltaSeconds * engineCameraVelocity;
	}
	if (m_inputSystem->IsKeyDown('C'))
	{

		engineCameraPosition -= engineCameraMatrix.GetKBasis3D() * deltaSeconds * engineCameraVelocity;
	}

	Vec2 mouseDelta = m_inputSystem->GetMouseClientDelta();
	if(mouseDelta.x != 0 || mouseDelta.y != 0)
	{
		engineCameraOrientation.m_yawDegrees += (mouseDelta.x * 0.1f);
		engineCameraOrientation.m_pitchDegrees -= (mouseDelta.y * 0.1f);
	}

	engineCamera.SetTransform(engineCameraPosition, engineCameraOrientation);

}