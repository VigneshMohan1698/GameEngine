#include "ECSRenderingSystem.hpp"

ECSRenderingSystem::ECSRenderingSystem(ECS* ecs, RendererD12* renderer)
{
	m_ecs = ecs;
	m_renderer = renderer;
}

ECSRenderingSystem::~ECSRenderingSystem()
{
	m_ecs = nullptr;
	m_renderer = nullptr;
}

void ECSRenderingSystem::Update3DCameraComponents(std::unordered_map<EntityID, CameraComponent>& cameraComponents, const bool updatingUICamera)
{
	//Go through a list of all cameracomponents
	//If it's UI camera update - set ui main camera buffer for gpu
	//else set 3D main camera buffer for gpu
	for (auto& pair : cameraComponents)
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


void ECSRenderingSystem::RenderEntities(std::unordered_map<EntityID, MeshComponent>& meshComponents)
{
	//Go through a list of mesh components 
	//Get their material which has shader and texture
	//Bind shaders and textures
	for(auto& pair : meshComponents)
	{
		MeshComponent* meshComponent = &pair.second;
		TransformComponent* transformComponent = m_ecs->GetComponentOfType<TransformComponent>(pair.first);

		meshComponent->m_mesh.ApplyTransform(transformComponent->GetTransformMatrix());
		ShaderD12* shader = meshComponent->m_material.GetShader();
		int albedoTextureIndex = meshComponent->m_material.GetAlbedoTextureIndex();
		m_renderer->BindShader(shader);
		m_renderer->BindTexture(0, albedoTextureIndex);
		m_renderer->DrawIndexedVertexArray((int)meshComponent->m_mesh.m_cpuMesh->m_verticesWithTangent.size(), meshComponent->m_mesh.m_cpuMesh->m_verticesWithTangent, meshComponent->m_mesh.m_cpuMesh->m_indices);
	}
}
