#include "Editor.hpp"

void Editor::Startup()
{
	//The constructor will initialize imgui
	m_editorImgui = new EditorImGui(this);
	m_settings.editorTheme = EditorTheme::Dark;
}

void Editor::Render()
{
	m_editorImgui->DrawEditor();
}

void Editor::Update(float deltaSeconds)
{
	
}

void Editor::Shutdown()
{
	//The destructor will shutdown imgui
	delete m_editorImgui;
	m_editorImgui = nullptr;
}
