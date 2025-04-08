#include "Engine/ECS/ECSComponents.hpp"
#include <Engine/Renderer/RendererD12.hpp>


extern RendererD12* g_theRenderer;

//------------------THIS FILE  IS JUST FOR MESH COMPONENT SO WE CAN new() a Mesh
MeshComponent::MeshComponent()
{
	m_mesh = new Mesh();
}

MeshComponent::~MeshComponent()
{
	//We will delete it at Renderer shutdown
	m_mesh = nullptr;
}


void MeshComponent::Add3DModelMesh(std::string modelName)
{
	std::string  modelPath = "Data/Models/" + modelName + '/' + modelName + ".obj";
	if (FileExists(modelPath))
	{
		m_mesh = g_theRenderer->CreateOrGetMesh(modelPath.c_str());
	}
	else
	{
		ERROR_AND_DIE(modelName + " File doesn't exist.")
	}
}