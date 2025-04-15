#pragma once
#include "Engine/Math/IntVec2.hpp"
#include "ThirdParty/ImGui/imgui_internal.h"
class Editor;

enum ImGuiColorIndexes {
	HeadingText,
	SubheadingText,
	SimpleText,
	Count
};

class EditorImGui
{
	public:
	EditorImGui(Editor* editor);
	~EditorImGui();

	void InitializeImGui();
	void DrawEditor();
	void ShutdownImGui();

	//bool ImGuiColorPicker(const char* label, ImColor* color);

	private:
	static ImVec4   m_colors[ImGuiColorIndexes::Count];
	float			m_relativeScale = 1.0f;
	IntVec2			m_windowdim = IntVec2();
	ImGuiContext*	m_context = nullptr;
	Editor*			m_editor  = nullptr;
};