#include "EditorImgui.hpp"
#include "Editor.hpp"

#include "ThirdParty/ImGui/imgui_impl_dx12.h"
#include "ThirdParty/ImGui/imgui_impl_win32.h"
#include "ThirdParty/ImGui/imgui_internal.h"
#include "Engine/Renderer/RendererD12.hpp"

extern RendererD12* g_theRenderer;

EditorImGui::EditorImGui(Editor* editor)
{
	InitializeImGui();
	m_colors[0] = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	m_colors[1] = ImVec4(0.0f, 0.5f, 0.5f, 1.0f);
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
	ImGui_ImplDX12_CreateDeviceObjects();

	//m_color = new ImColor(255, 255, 255);
	m_windowdim = g_theRenderer->GetRenderConfig().m_window->GetClientDimensions();
	ImGui::GetIO().ImeWindowHandle = g_theRenderer->GetRenderConfig().m_window->GetHwnd();

	m_relativeScale = g_theRenderer->m_dimensions.x / static_cast<float>(m_windowdim.x);
}

void EditorImGui::DrawEditor()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	if (ImGui::CollapsingHeader("Player Info"))
	{
		ImGui::TextColored(m_colors[0], "Player Info : ");
		ImGui::TextColored(m_colors[1], "Game Info : ");
		ImGui::Text("F1-F4 Different Debug options");
		ImGui::Text("F5 Debug renderer enabled");
		ImGui::Text("F6 Shadows enabled");
		ImGui::Text("(SPACE) for Activate IMGUI (WASD) To Move Camera (IJKL, MN) to move Light");
	}

	ImGui::End();
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


