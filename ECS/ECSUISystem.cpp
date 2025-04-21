#include "ECSUISystem.hpp"
#include <Engine/ECS/ECS.hpp>
#include <Engine/Renderer/ShadowMap.hpp>
#include <Engine/Material/MaterialIncludes.hpp>

ECSUISystem::ECSUISystem(ECS* ecs, RendererD12* renderer)
{
	m_ecs = ecs;
	m_font = renderer->CreateBitmapFont("Data/Images/SquirrelFixedFont");
	m_renderer = renderer;
}

void ECSUISystem::Update(float deltaSeconds)
{
	if (!m_ecs->GetEngineState().m_enabledFeatures.debugRendererEnabled) 
	{
		return;
	}

	CameraComponent* mainUICamera = nullptr;

	for (auto& pair : m_ecs->m_cameraComponents)
	{
		if (pair.second.m_mainUICamera)
		{
			mainUICamera = &pair.second;
		}
	}
	if(mainUICamera) {
		
		RenderUI(mainUICamera);
	}
	
}

void ECSUISystem::RenderUI(CameraComponent* uiCamera)
{
	ShaderD12* shader2D = m_renderer->CreateOrGetShader("Default", Defaul2DShaderFilePath);
	TextureD12* fontTexture = m_renderer->CreateOrGetTextureFromFile("FontTexture", "Data/Images/SquirrelFixedFont.png");

	std::vector<Vertex_PCU> uiVerts;
	AABB2 debugRenderTextBounds = uiCamera->m_camera.GetCameraBounds();
	AABB2 textBoundsToUse = uiCamera->m_camera.GetCameraBounds();

	debugRenderTextBounds.m_mins.y = debugRenderTextBounds.m_maxs.y;
	debugRenderTextBounds.m_mins.x = debugRenderTextBounds.m_mins.x + 2.0f;

	for (auto& pair : m_ecs->m_UITextComponents)
	{
		UITextComponent* uiTextComponent = &pair.second;

		if(uiTextComponent->m_isDebugRender) {
			debugRenderTextBounds.m_mins.y -= uiTextComponent->m_textHeight;
			textBoundsToUse = debugRenderTextBounds;
		}

		//TO DO: Alignment maybe a component variable?
		m_font->AddVertsForTextInBox2D(uiVerts, textBoundsToUse, uiTextComponent->m_textHeight, uiTextComponent->m_text, uiTextComponent->m_color,
			DefaultUITextAspect, Vec2(0.0f, 0.0f), TextBoxMode::SHRINK_TO_FIT, 999999, 5.0f);

		textBoundsToUse = uiCamera->m_camera.GetCameraBounds();
	}

	m_renderer->BindShader(shader2D);
	m_renderer->BindTexture(0, fontTexture);
	m_renderer->BeginRasterizerCamera(uiCamera->m_camera);
	m_renderer->DrawVertexArray(uiVerts.size(), uiVerts);
	//m_renderer->FinishUpGPUWork();
}
