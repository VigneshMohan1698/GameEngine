#pragma once
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/Vec4.hpp"
#include "ThirdParty/ImGui/imgui.h"
class Editor;

enum Fonts {
	SF20,
	SF14,
	SFItalic,
	SFBold20,
	SFBold14,
	FontCount
};

enum ImGuiColorIndexes {
	HeadingText,
	SubheadingText,
	SimpleText,
	ImGuiColorsCount
};

class EditorImGui
{
	public:
	EditorImGui(Editor* editor);
	~EditorImGui();

	void InitializeImGui();
	void DrawEditor();
	void UpdateEditor(float deltaSeconds);
	void ShutdownImGui();

	void InitializeFonts(ImGuiIO* io);
	void SetStyle();
	//bool ImGuiColorPicker(const char* label, ImColor* color);

	private:
	//static Vec4     m_colors[ImGuiColorIndexes::ImGuiColorsCount];
	float			m_frameTimesData[100] = {};
	long long		m_currentFrameNumber = 0;
	float			m_relativeScale = 1.0f;
	IntVec2			m_windowdim = IntVec2();
	ImGuiContext*	m_context = nullptr;
	Editor*			m_editor  = nullptr;
	ImFont*			m_fonts[Fonts::FontCount] = {};
};