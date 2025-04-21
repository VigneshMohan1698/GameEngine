#include "EditorImgui.hpp"
#include "Editor.hpp"

#include "ThirdParty/ImGui/imgui_impl_dx12.h"
#include "ThirdParty/ImGui/imgui_impl_win32.h"
#include "ThirdParty/ImGui/imgui_internal.h"
#include "Engine/Renderer/RendererD12.hpp"
#include <Engine/ECS/ECS.hpp>

extern RendererD12* g_theRenderer;
extern ECS* g_theECS;

EditorImGui::EditorImGui(Editor* editor)
{
	m_editor = editor;
	InitializeImGui();
	SetStyle();
}

EditorImGui::~EditorImGui()
{
	ShutdownImGui();
	m_editor = nullptr;
}

void EditorImGui::InitializeImGui()
{
	auto device = g_theRenderer->GetDevice();
	auto imguiHeap = g_theRenderer->GetIMGUIDescriptorHeap();
	IMGUI_CHECKVERSION();
	m_context = ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(g_theRenderer->GetRenderConfig().m_window->GetHwnd());
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = imguiHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = imguiHeap->GetGPUDescriptorHandleForHeapStart();
	ImGui_ImplDX12_Init(device, 3, DXGI_FORMAT_R8G8B8A8_UNORM, imguiHeap,
		cpuHandle, gpuHandle);

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	InitializeFonts(&io);
	ImGui_ImplDX12_CreateDeviceObjects();

	//m_color = new ImColor(255, 255, 255);
	m_windowdim = g_theRenderer->GetRenderConfig().m_window->GetClientDimensions();
	//io.ImeWindowHandle = g_theRenderer->GetRenderConfig().m_window->GetHwnd();
	m_relativeScale = g_theRenderer->m_dimensions.x / static_cast<float>(m_windowdim.x);
}

void EditorImGui::UpdateEditor(float deltaSeconds)
{
	EngineState& engineState = g_theECS->GetEngineState();
	int index = engineState.m_currentFrameNumber % 100;
	float roundedDelta = 0.0f;
	if (deltaSeconds == 0.0f) {
		m_frameTimesData[index] = 0.0f;
	} else {
		roundedDelta = std::round(deltaSeconds * 1000.0f) / 1000.0f;
		m_frameTimesData[index] = 1.0f / roundedDelta;
	}
	

	engineState.m_fps = m_frameTimesData[engineState.m_currentFrameNumber % 100];
	engineState.m_frameTime = deltaSeconds;
	engineState.m_currentFrameNumber++;
	engineState.m_averageFPS = engineState.m_fps;

	//TODO : ONLY ADD THIS WHILE TESTING PERFORMANCE
	engineState.m_averageFrameTime = 0;
	engineState.m_averageFPS = 0;
	if(engineState.m_currentFrameNumber >= 100) {
		for (int i = 0; i < 100; i++) {
			engineState.m_averageFPS += m_frameTimesData[i];
			engineState.m_averageFrameTime += 1.0f / m_frameTimesData[i];
			
		}
	}
	engineState.m_averageFrameTime /= 100;
	engineState.m_averageFPS /= 100;
	
}

void EditorImGui::DrawEditor()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	ImGuiID dockspace_id = ImGui::GetID("Engine");
	ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::PushFont(m_fonts[Fonts::SF14]);

	ImGui::PushFont(m_fonts[Fonts::SFBold14]); 	ImGui::Begin("Scene Inspector", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing ); ImGui::PopFont();

	ImGui::Text("Contains scene information");

	ImGui::End();

	ImGui::PushFont(m_fonts[Fonts::SFBold14]);  ImGui::Begin("Assets", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing);  ImGui::PopFont();
	ImGui::Text("Acces to all assets");
	ImGui::End();

	ImGui::PushFont(m_fonts[Fonts::SFBold14]);  ImGui::Begin("Profiler", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing); ImGui::PopFont();
	ImGui::Text("Performance and memory information");
	
	EngineState& engineState = g_theECS->GetEngineState();
	if (engineState.m_fps < 60.0f) {
		ImGui::PushStyleColor(ImGuiCol_PlotLines, IM_COL32(255, 0, 0, 255)); // red
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_PlotLines, IM_COL32(0, 255, 0, 255)); // green
	}
	
	char buffer[16];
	std::snprintf(buffer, sizeof(buffer), "%.3f", engineState.m_averageFrameTime);
	std::string avgdeltaseconds = buffer;

	std::string overlay = "Average FPS: " + std::to_string(engineState.m_averageFPS) + " Average ds: " + avgdeltaseconds;

	ImGui::PlotLines("##fps", m_frameTimesData, IM_ARRAYSIZE(m_frameTimesData), engineState.m_currentFrameNumber % IM_ARRAYSIZE(m_frameTimesData),
		overlay.c_str(), 0.0f, 144.0f, ImVec2(300, 150));

	ImGui::PopStyleColor();
	ImGui::End();


	ImGui::PushFont(m_fonts[Fonts::SFBold14]);  ImGui::Begin("Render Options", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing);  ImGui::PopFont();
	ImGui::Text("Information about clicked entity?");
	ImGui::End();

	ImGui::PopFont();
	ImGui::Render();

	auto heap = g_theRenderer->GetIMGUIDescriptorHeap();
	auto renderTarget = g_theRenderer->GetBackBufferCPUHandle();
	g_theRenderer->GetCommandList()->SetDescriptorHeaps(1, &heap);
	g_theRenderer->GetCommandList()->OMSetRenderTargets(1, renderTarget, FALSE, NULL);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), g_theRenderer->GetCommandList());
	
}

void EditorImGui::ShutdownImGui()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void EditorImGui::SetStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	if (m_editor->m_settings.editorTheme == EditorTheme::Dark) {
		ImGui::StyleColorsDark();
	} else {
		ImGui::StyleColorsLight();
	}
	style.Colors[ImGuiCol_TitleBgActive] = style.Colors[ImGuiCol_TitleBg];
	style.Colors[ImGuiCol_DockingEmptyBg] = style.Colors[ImGuiCol_TitleBg];
	style.Colors[ImGuiCol_WindowBg].w = 1.0f;
}

void EditorImGui::InitializeFonts(ImGuiIO* io)
{
	m_fonts[Fonts::SF20] = io->Fonts->AddFontFromFileTTF("../../Engine/Code/Engine/Assets/Fonts/SF.ttf", 20.0f);
	m_fonts[Fonts::SF14] = io->Fonts->AddFontFromFileTTF("../../Engine/Code/Engine/Assets/Fonts/SF.ttf", 16.0f);
	m_fonts[Fonts::SFItalic] = io->Fonts->AddFontFromFileTTF("../../Engine/Code/Engine/Assets/Fonts/SFItalic.ttf", 14.0f);
	m_fonts[Fonts::SFBold20] = io->Fonts->AddFontFromFileTTF("../../Engine/Code/Engine/Assets/Fonts/SFBold.ttf", 20.0f);
	m_fonts[Fonts::SFBold14] = io->Fonts->AddFontFromFileTTF("../../Engine/Code/Engine/Assets/Fonts/SFBold.ttf", 16.0f);
	io->Fonts->Build();
}
//bool EditorImGui::ImGuiColorPicker(const char* label, ImColor* color)
//{
//	static const float HUE_PICKER_WIDTH = 20.0f;
//	static const float CROSSHAIR_SIZE = 7.0f;
//	static const ImVec2 SV_PICKER_SIZE = ImVec2(200, 200);
//
//	bool value_changed = false;
//
//	ImVec2 picker_pos = ImGui::GetCursorScreenPos();
//
//	float hue, saturation, value;
//	ImGui::ColorConvertRGBtoHSV(
//		color->Value.x, color->Value.y, color->Value.z, hue, saturation, value);
//	auto hue_color = ImColor::HSV(hue, 1, 1);
//
//	float x = saturation * value;
//	ImVec2 p(picker_pos.x + x * SV_PICKER_SIZE.x, picker_pos.y);
//
//	ImGui::SetCursorScreenPos(ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 10, picker_pos.y));
//	ImGui::InvisibleButton("hue_selector", ImVec2(HUE_PICKER_WIDTH, SV_PICKER_SIZE.y));
//
//	if (ImGui::IsItemHovered())
//	{
//		if (ImGui::GetIO().MouseDown[0])
//		{
//			hue = ((ImGui::GetIO().MousePos.y - picker_pos.y) / SV_PICKER_SIZE.y);
//			value_changed = true;
//		}
//	}
//
//	*color = ImColor::HSV(hue, saturation, value);
//	return value_changed || ImGui::ColorEdit3(label, &color->Value.x);
//}


