#include "Window.hpp"
#include "Engine/Input/InputSystem.hpp"
#include <windows.h>
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Game/GameCommon.hpp"
#include "ThirdParty/ImGui/imgui_impl_win32.h"


Window* Window::s_mainWindow = nullptr;

HDC g_displayDeviceContext = nullptr;			
HGLRC g_openGLRenderingContext = nullptr;			

Window::Window(WindowConfig const& config)
{
	m_config = config;
	s_mainWindow = this;
}
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowsMessageHandlingProcedure(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(windowHandle, wmMessageCode, wParam, lParam))
	{
		return true;
	}
	Window* windowContext = Window::GetWindowContext();
	InputSystem* input = windowContext->GetConfig().m_inputSystem;
	switch (wmMessageCode)
	{
		// App close requested via "X" button, or right-click "Close Window" on task bar, or "Close" from system menu, or Alt-F4
		case WM_CLOSE:
		{
			g_theEventSystem->FireEvent("ApplicationQuit");
			/*g_theApp->HandleQuitRequested();*/
			//return 0;
		}

		// Raw physical keyboard "key-was-just-depressed" event (case-insensitive, not translated)
		case WM_KEYDOWN:
		{
			unsigned char asKey = (unsigned char)wParam;
		
			if (input)
			{
				input->HandleKeyPressed(asKey);
				return 0;
			}
			break;
		}

		// Raw physical keyboard "key-was-just-released" event (case-insensitive, not translated)
		case WM_KEYUP:
		{
			unsigned char asKey = (unsigned char)wParam;

			if (input)
			{
				input->HandleKeyReleased(asKey);
				return 0;
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			unsigned char keyCode = KEYCODE_LEFT_MOUSE;
			if (input)
			{
				input->HandleKeyPressed(keyCode);
			}
			break;
		}
		case WM_RBUTTONDOWN:
		{
			unsigned char keyCode = KEYCODE_RIGHT_MOUSE;
			if (input)
			{
				input->HandleKeyPressed(keyCode);
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			unsigned char keyCode = KEYCODE_LEFT_MOUSE;
			if (input)
			{
				input->HandleKeyReleased(keyCode);
			}
			break;
		}
		case WM_RBUTTONUP:
		{
			unsigned char keyCode = KEYCODE_RIGHT_MOUSE;
			if (input)
			{
				input->HandleKeyReleased(keyCode);
			}
			break;
		}
		case WM_CHAR:
		{
			input->HandleCharInput((int)wParam);
			break;
		}
		case WM_ACTIVATE:
		{
			bool isActivated = (bool)wParam;
			if (isActivated)
			{
				input->SetMouseMode(input->m_isHidden, input->m_isClipped, input->m_isRelative);
			}
			else if(!isActivated)
			{
				input->SetMouseMode(false, false, false);
			}
			break;
		}
		case WM_MOUSEWHEEL:
		{
			auto delta = GET_WHEEL_DELTA_WPARAM(wParam);
			input->m_mouseWheelDelta = delta;
		}
	}

	// Send back to Windows any unhandled/unconsumed messages we want other apps to see (e.g. play/pause in music apps, etc.)
	return DefWindowProc(windowHandle, wmMessageCode, wParam, lParam);
}

void Window::CreateOSWindow()
{
	UNUSED((void)g_openGLRenderingContext);
	Window* windowContext = Window::GetWindowContext();
	const char* APP_NAME = windowContext->GetConfig().m_windowTitle.c_str();	// ...becomes ??? (Change this per project!)

	// Define a window style/class
	WNDCLASSEX windowClassDescription;
	memset(&windowClassDescription, 0, sizeof(windowClassDescription));
	windowClassDescription.cbSize = sizeof(windowClassDescription);
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast<WNDPROC>(WindowsMessageHandlingProcedure); // Register our Windows message-handling function
	windowClassDescription.hInstance = GetModuleHandle(NULL);
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT("Simple Window Class");
	RegisterClassEx(&windowClassDescription);

	// #SD1ToDo: Add support for fullscreen mode (requires different window style flags than windowed mode)
	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	// Get desktop rect, dimensions, aspect
	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect(desktopWindowHandle, &desktopRect);
	float desktopWidth = (float)(desktopRect.right - desktopRect.left);
	float desktopHeight = (float)(desktopRect.bottom - desktopRect.top);
	float desktopAspect = desktopWidth / desktopHeight;

	// Calculate maximum client size (as some % of desktop size)
	constexpr float maxClientFractionOfDesktop = 0.99f;
	float clientWidth = desktopWidth * maxClientFractionOfDesktop;
	float clientHeight = desktopHeight * maxClientFractionOfDesktop;
	float clientAspect = windowContext->GetConfig().m_clientAspect;
	if (clientAspect > desktopAspect)
	{
		// Client window has a wider aspect than desktop; shrink client height to match its width
		clientHeight = clientWidth / clientAspect;
	}
	else
	{
		// Client window has a taller aspect than desktop; shrink client width to match its height
		clientWidth = clientHeight * clientAspect;
	}
	/*clientWidth = 1280.0f;
	clientHeight = 720.0f;*/
	m_clientDimensions.x = (int)clientWidth;
	m_clientDimensions.y = (int)clientHeight;
	if (m_textureDimensions.x == 0)
	{
		m_textureDimensions.x = (int)clientWidth;
		m_textureDimensions.y = (int)clientHeight;
	}
	
	// Calculate client rect bounds by centering the client area
	float clientMarginX = 0.5f * (desktopWidth - clientWidth);
	float clientMarginY = 0.5f * (desktopHeight - clientHeight);
	RECT clientRect;
	clientRect.left = (int)clientMarginX;
	clientRect.right = clientRect.left + (int)clientWidth;
	clientRect.top = (int)clientMarginY;
	clientRect.bottom = clientRect.top + (int)clientHeight;

	// Calculate the outer dimensions of the physical window, including frame et. al.
	RECT windowRect = clientRect;
	AdjustWindowRectEx(&windowRect, windowStyleFlags, FALSE, windowStyleExFlags);
	HMODULE applicationInstanceHandle = ::GetModuleHandle(NULL);
	WCHAR windowTitle[1024];
	MultiByteToWideChar(GetACP(), 0, APP_NAME, -1, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
	HWND hwnd = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		(HINSTANCE)applicationInstanceHandle,
		NULL);


	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	m_OswindowHandle = hwnd;
	g_displayDeviceContext = GetDC(hwnd);
	HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
	SetCursor(cursor);
}

Vec2 Window::GetNormalizedCursorPosition() const
{
	HWND windowHandle = HWND(m_OswindowHandle);
	POINT cursorCoords;
	RECT clientRect;
	::GetCursorPos(&cursorCoords);
	::ScreenToClient(windowHandle, &cursorCoords);
	::GetClientRect(windowHandle, &clientRect);
	float cursorX = float(cursorCoords.x) / float(clientRect.right);
	float cursorY = float(cursorCoords.y) / float(clientRect.bottom);
	return Vec2(cursorX, 1.0f - cursorY);
}
Window::~Window()
{
}

void Window::Startup()
{
	CreateOSWindow();
}

void Window::BeginFrame()
{
	RunMessagePump();
}

void Window::EndFrame()
{
}

void Window::ShutDown()
{
}
void Window::ChangeTitle(std::string title)
{
	HWND windowHandle = (HWND)m_OswindowHandle;
	SetWindowTextA(windowHandle, (LPCSTR) title.c_str());
}

WindowConfig const& Window::GetConfig() const
{
	return m_config;
}

Window* Window::GetWindowContext()
{
	return s_mainWindow;
}

void* Window::GetHwnd() const
{
	return m_OswindowHandle;
}

IntVec2 Window::GetClientDimensions() const
{
	return m_clientDimensions;
}

IntVec2 Window::GetRenderTextureDimensions()
{
	return m_textureDimensions;
}

void Window::SetRenderTextureDimensions(IntVec2 dimensions)
{
	m_textureDimensions = dimensions;
}

void Window::RunMessagePump()
{
	MSG queuedMessage;
	for (;; )
	{
		const BOOL wasMessagePresent = PeekMessage(&queuedMessage, NULL, 0, 0, PM_REMOVE);
		if (!wasMessagePresent)
		{
			break;
		}

		TranslateMessage(&queuedMessage);
		DispatchMessage(&queuedMessage); // This tells Windows to call our "WindowsMessageHandlingProcedure" (a.k.a. "WinProc") function
	}
}


 bool Window::HasFocus()
{
	if (GetActiveWindow() == (HWND)m_OswindowHandle)
	{
		return true;
	}
	return false;
}