#pragma once
#include "Engine/Core/EngineCommon.hpp"

//--------------------------------------------------------
class InputSystem;

//-----------------------------------------------------
struct WindowConfig
{
	InputSystem*		m_inputSystem = nullptr;
	std::string			m_windowTitle = "Untitled App";
	float				m_clientAspect = 2.0f;
	bool				m_isFullScreen = false;

};
//-----------------------------------------------------
class Window
{
public: 
	void*					m_OswindowHandle = nullptr;
	IntVec2					m_clientDimensions;
	IntVec2					m_textureDimensions;
	Window(WindowConfig const& config);
	Vec2 GetNormalizedCursorPosition() const;
	~Window();

	void Startup();
	void BeginFrame();
	void EndFrame();
	void ShutDown();

	void ChangeTitle(std::string title);

	WindowConfig const& GetConfig() const;
	static Window* GetWindowContext();
	void* GetHwnd() const;
	IntVec2 GetClientDimensions() const;
	IntVec2 GetRenderTextureDimensions();
	void SetRenderTextureDimensions(IntVec2 dimensions);
	bool HasFocus();
protected:
	void CreateOSWindow();
	void RunMessagePump();

	
protected:
	WindowConfig			m_config;
	static Window*			s_mainWindow;

};

