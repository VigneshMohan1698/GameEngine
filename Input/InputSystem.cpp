#include "Engine/Input/InputSystem.hpp"
#include <Windows.h>
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include <Engine/Core/ErrorWarningAssert.hpp>
#include "Engine/Core/DevConsole.hpp"

const unsigned char KEYCODE_F1 = VK_F1;
const unsigned char KEYCODE_F2 = VK_F2;
const unsigned char KEYCODE_F3 = VK_F3;
const unsigned char KEYCODE_F4 = VK_F4;
const unsigned char KEYCODE_F5 = VK_F5;
const unsigned char KEYCODE_F6 = VK_F6;
const unsigned char KEYCODE_F7 = VK_F7;
const unsigned char KEYCODE_F8 = VK_F8;
const unsigned char KEYCODE_F9 = VK_F9;
const unsigned char KEYCODE_F10 = VK_F10;
const unsigned char KEYCODE_F11 = VK_F11;
const unsigned char KEYCODE_F12 = VK_F12;
const unsigned char KEYCODE_ESCAPE = VK_ESCAPE;
const unsigned char KEYCODE_UPARROW = VK_UP;
const unsigned char KEYCODE_DOWNARROW = VK_DOWN;
const unsigned char KEYCODE_LEFTARROW = VK_LEFT;
const unsigned char KEYCODE_RIGHTARROW = VK_RIGHT;
const unsigned char KEYCODE_LEFT_MOUSE = VK_LBUTTON;
const unsigned char KEYCODE_RIGHT_MOUSE = VK_RBUTTON;
const unsigned char KEYCODE_SPACE = VK_SPACE;


InputSystem::InputSystem()
{

}

InputSystem::InputSystem(InputSystemConfig const& inputConfig)
{
	UNUSED((void)inputConfig);
}

InputSystem::~InputSystem()
{
}

void InputSystem::Startup()
{
}

void InputSystem::ShutDown()
{
}

void InputSystem::BeginFrame()
{
	for (int controllerIndex = 0; controllerIndex < m_numberOfControllers; controllerIndex++)
	{
		m_controllers[controllerIndex].m_id = controllerIndex;
		m_controllers[controllerIndex].Update();
	}
}

void InputSystem::EndFrame()
{
	for (int keyindex = 0; keyindex < m_keyBoardButtonStates; keyindex++)
	{
		this->m_keyStates[keyindex].wasKeyDownLastFrame = this->m_keyStates[keyindex].isKeyDownThisFrame;

	}
	for (int controllerIndex = 0; controllerIndex < (int)XboxButtonID::NUM; controllerIndex++)
	{
		m_controllers[0].m_buttons[controllerIndex].wasKeyDownLastFrame = m_controllers[0].m_buttons[controllerIndex].isKeyDownThisFrame;
	}
	m_mouseWheelDelta = 0.0f;
	m_previousMousePosition = m_currentMousePosition;
}

bool InputSystem::WasKeyJustPressed(unsigned char keyCode)
{
	if (this->m_keyStates[keyCode].isKeyDownThisFrame && (this->m_keyStates[keyCode].isKeyDownThisFrame != this->m_keyStates[keyCode].wasKeyDownLastFrame))
		return true;

	return false;
}

bool InputSystem::WasKeyJustReleased(unsigned char keyCode)
{
	if (!this->m_keyStates[keyCode].isKeyDownThisFrame && (this->m_keyStates[keyCode].isKeyDownThisFrame != this->m_keyStates[keyCode].wasKeyDownLastFrame))
		return true;

	return false;
}

bool InputSystem::IsKeyDown(unsigned char keyCode)
{
	return this->m_keyStates[keyCode].isKeyDownThisFrame;
}

void InputSystem::HandleKeyPressed(unsigned char keyCode)
{
	EventArgs charArgs;
	charArgs.SetValue("KeyName", keyCode);
	g_theEventSystem->FireEvent("HandleEventKeyPressed", charArgs);
	if (g_theConsole)
	{
		if (g_theConsole->m_mode == DevConsoleMode::VISIBLE && keyCode != VK_OEM_3)
		{
			ConsumeKeyJustPressed(keyCode);
		}
		else
		{
			this->m_keyStates[keyCode].isKeyDownThisFrame = true;
		}
	}
	else
	{
		this->m_keyStates[keyCode].isKeyDownThisFrame = true;
	}
}

void InputSystem::HandleKeyReleased(unsigned char keyCode)
{
	this->m_keyStates[keyCode].isKeyDownThisFrame = false;
}

bool InputSystem::HandleCharInput(int charCode)
{
	EventArgs charArgs;
	charArgs.SetValue("KeyName", charCode);
	g_theEventSystem->FireEvent("HandleCharacterInput", charArgs);
	return true;
}

void InputSystem::ConsumeKeyJustPressed(unsigned char keyCode)
{
	HandleKeyReleased(keyCode);
}

XBoxController const& InputSystem::GetController(int controllerID)
{
	return this->m_controllers[controllerID];
}

void InputSystem::SetMouseMode(bool hidden, bool clipped, bool relative)
{
	CURSORINFO cursor = { 0 };
	cursor.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&cursor);

	Window* mainWindow = Window::GetWindowContext();
	IntVec2 clientDimensions = mainWindow->GetClientDimensions();
	IntVec2 textureDimensions = mainWindow->GetRenderTextureDimensions();
	if ((cursor.flags == 0 && !hidden) || (cursor.flags != 0 && hidden))
	{
		if (!hidden)
		{
			int cursorHiddenValue = ShowCursor(TRUE);
			while (cursorHiddenValue < 0)
				cursorHiddenValue = ShowCursor(TRUE);
			m_isHidden = false;
			
		}
		else
		{
			int cursorHiddenValue = ShowCursor(FALSE);
			while (cursorHiddenValue >= 0)
				cursorHiddenValue = ShowCursor(FALSE);
			m_isHidden = true;

			POINT pt = { 0 };
			pt.x = int(clientDimensions.x * 0.5f);
			pt.y = int(clientDimensions.y * 0.5f);
			ClientToScreen((HWND)mainWindow->GetHwnd(), &pt);
			SetCursorPos(pt.x, pt.y);
		}
	}

	if (clipped || relative)
	{
		RECT windowRect = { 0 };
		windowRect.left = 0;
		windowRect.right = clientDimensions.x;
		windowRect.bottom = 0;
		windowRect.top = clientDimensions.y;
		ClipCursor(&windowRect);
	}

	if (relative)
	{
		POINT prevPos;
		GetCursorPos(&prevPos);
		ScreenToClient((HWND)mainWindow->GetHwnd(), &prevPos);
		m_previousMousePosition = Vec2((float)prevPos.x, (float)prevPos.y);
	
		POINT pt = { 0 };
		pt.x = int(clientDimensions.x * 0.5f);
		pt.y = int(clientDimensions.y * 0.5f);
		ClientToScreen((HWND)mainWindow->GetHwnd(), &pt);
		SetCursorPos(pt.x, pt.y);

		POINT currPos;
		GetCursorPos(&currPos);
		ScreenToClient((HWND)mainWindow->GetHwnd(), &currPos);
		m_currentMousePosition = Vec2((float)currPos.x, (float)currPos.y);
	}
}


void InputSystem::UpdateCursorPositionForThesis()
{
	Window* mainWindow = Window::GetWindowContext();
	IntVec2 clientDimensions = mainWindow->GetClientDimensions();
	IntVec2 textureDimensions = mainWindow->GetRenderTextureDimensions();

	POINT currPos;
	GetCursorPos(&currPos);
	ScreenToClient((HWND)mainWindow->GetHwnd(), &currPos);

	Vec2 clientDelta = GetMouseClientDelta();
	if (clientDelta.x != 0.0f || clientDelta.y != 0.0f)
	{

		float mouseScalex = (float)clientDimensions.x / (float)textureDimensions.x;
		float mouseScaley = (float)clientDimensions.y / (float)textureDimensions.y;
		Vec2 position;

		float xOffset = currPos.x + mouseScalex * clientDelta.x;
		float yOffset = currPos.y + mouseScaley * clientDelta.y;
		position = Vec2(xOffset, yOffset);


		POINT pt = { 0 };
		pt.x = int(position.x);
		pt.y = int(position.y);
		ClientToScreen((HWND)mainWindow->GetHwnd(), &pt);
		SetCursorPos(pt.x, pt.y);
	}
}

void InputSystem::SetCursorAtCenter()
{
	m_clientCenterX = Window::GetWindowContext()->m_clientDimensions.x / 2;
	m_clientCenterY = Window::GetWindowContext()->m_clientDimensions.y / 2;
	SetCursorPosition(m_clientCenterX, m_clientCenterY);
}

void InputSystem::SetCursorPosition(int positionX, int positionY)
{
	POINT cursorCoords;
	cursorCoords.x = positionX;
	cursorCoords.y = positionY;
	::ClientToScreen((HWND)Window::GetWindowContext()->m_OswindowHandle, &cursorCoords);
	::SetCursorPos(cursorCoords.x, cursorCoords.y);
	HWND windowHandle = HWND(Window::GetWindowContext()->m_OswindowHandle);
	POINT m_cursorCoords;
	::GetCursorPos(&m_cursorCoords);
	::ScreenToClient(windowHandle, &m_cursorCoords);
	m_currentMousePosition.x = (float)m_cursorCoords.x;
	m_currentMousePosition.y = (float)m_cursorCoords.y;
}

Vec2 InputSystem::GetMouseClientPosition()
{
	return m_currentMousePosition;
}

Vec2 InputSystem::GetMouseClientDelta()
{
	return m_currentMousePosition - m_previousMousePosition;
}

float InputSystem::GetMouseWheelDelta()
{
	return 0.0f;
}
