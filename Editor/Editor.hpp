#pragma once

#include "EditorImgui.hpp"

enum class EditorTheme {
	Dark,
	Light,
	Blue
};

struct EditorSettings {
	EditorTheme editorTheme;
};

class Editor
{
	public: 

	Editor() = default;
	~Editor() {};

	void Startup();
	void Render();
	void Update(float deltaSeconds);
	void Shutdown();

	public: 
	EditorImGui*		 m_editorImgui = nullptr;
	EditorSettings		 m_settings  = EditorSettings();

};