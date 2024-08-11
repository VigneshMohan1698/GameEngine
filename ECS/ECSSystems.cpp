#include "ECSSystems.hpp"


void ECSRenderingSystem::Update()
{
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
		MeshComponent* meshComponent = &pair.second;
		TransformComponent* transformComponent = m_ecs->GetComponentOfType<TransformComponent>(pair.first);
		meshComponent->m_mesh.SetTransform(transformComponent->GetTransformMatrix());
		ShaderD12* shader = meshComponent->m_material.GetShader();
		int albedoTextureIndex = meshComponent->m_material.GetAlbedoTextureIndex();
		TextureD12* sphereTexture = m_renderer->GetTextureAtIndex(0);
		m_renderer->BindShader(shader);
		m_renderer->BindTexture(0, albedoTextureIndex);
		VertexNormalTangentArray verts = meshComponent->m_mesh.GetTransformedVertices(transformComponent->GetTransformMatrix());
		m_renderer->DrawIndexedVertexArray((int)verts.size(), verts, meshComponent->m_mesh.m_cpuMesh->m_indices);
	}
	
	//Finish up rendering. Reset command allocator, Present frame buffer and move to next frame
	m_renderer->FinishUpGPUWork();
	m_renderer->Present();
	m_renderer->MoveToNextFrame();
}
