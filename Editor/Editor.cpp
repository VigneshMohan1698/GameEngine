#include "Editor.hpp"
#include "Engine/Renderer/RendererD12.hpp"

extern RendererD12* g_theRenderer;

void Editor::Startup()
{
	//The constructor will initialize imgui
	m_settings.editorTheme = EditorTheme::Dark;
	m_editorImgui = new EditorImGui(this);
}

void Editor::Update(float deltaSeconds)
{
	m_editorImgui->UpdateEditor(deltaSeconds);
}

void Editor::Render()
{
	m_editorImgui->DrawEditor();
}


void Editor::Shutdown()
{
	//The destructor will shutdown imgui
	delete m_editorImgui;
	m_editorImgui = nullptr;
}
