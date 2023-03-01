#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "XBoxController.hpp"
#include <Xinput.h> // include the Xinput API header file (interface)
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/MathUtils.hpp"
#pragma comment( lib, "xinput9_1_0" ) // Link in the xinput.lib static library // #Eiserloh: Xinput 1_4 doesn't work in Windows 7; use version 9_1_0 explicitly for broadest compatibility

XBoxController::XBoxController()
{

}

XBoxController::XBoxController(int controllerId)
{
    this->m_id = controllerId;
}

XBoxController::~XBoxController()
{
}

bool XBoxController::IsConnected() const
{
    return this->m_isConnected;
}

int XBoxController::GetControllerID() const
{
    return this->m_id;
}

AnalogJoystick const& XBoxController::GetLeftStick() const
{
    return this->m_lefStick;
}

AnalogJoystick const& XBoxController::GetRightStick() const
{
    return this->m_rightStick;
}

float XBoxController::GetLeftTrigger() const
{
    return this->m_leftTrigger;
}

float XBoxController::GetRightTrigger() const
{
    return this->m_rightTrigger;
}

KeyButtonState const& XBoxController::GetButton(XboxButtonID buttonID) const
{
    return m_buttons[(int)buttonID];
}

bool XBoxController::IsButtonDown(XboxButtonID buttonID) const
{
    return m_buttons[(int)buttonID].isKeyDownThisFrame;
}

bool XBoxController::WasButtonJustPressed(XboxButtonID buttonID) const
{
    if (m_buttons[(int)buttonID].isKeyDownThisFrame && (m_buttons[(int)buttonID].isKeyDownThisFrame != m_buttons[(int)buttonID].wasKeyDownLastFrame))
          return true;

    else
        return false;
}

bool XBoxController::WasButtonJustReleased(XboxButtonID buttonID) const
{
    if (!m_buttons[(int)buttonID].isKeyDownThisFrame && (m_buttons[(int)buttonID].isKeyDownThisFrame != m_buttons[(int)buttonID].wasKeyDownLastFrame))
        return true;
	else
		return false;
}


void XBoxController::Update()
{
	XINPUT_STATE xboxControllerState;
	DWORD errorStatus = XInputGetState(m_id, &xboxControllerState);
    if (errorStatus == ERROR_SUCCESS)
    {
        this->m_isConnected = true;
       short sThumbLX = xboxControllerState.Gamepad.sThumbLX;
       short sThumbLY = xboxControllerState.Gamepad.sThumbLY;
       UpdateJoystick(this->m_lefStick, sThumbLX, sThumbLY);

	   short sThumbRX = xboxControllerState.Gamepad.sThumbRX;
       short sThumbRY = xboxControllerState.Gamepad.sThumbRY;
       UpdateJoystick(this->m_rightStick, sThumbRX, sThumbRY);

	   UpdateTriggers(m_leftTrigger, xboxControllerState.Gamepad.bLeftTrigger);
	   UpdateTriggers(m_rightTrigger, xboxControllerState.Gamepad.bRightTrigger);

       UpdateButton(XboxButtonID::XBOX_BUTTON_DPADUP, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_UP);
       UpdateButton(XboxButtonID::XBOX_BUTTON_DPADDOWN, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_DOWN);
       UpdateButton(XboxButtonID::XBOX_BUTTON_DPADLEFT, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_LEFT);
       UpdateButton(XboxButtonID::XBOX_BUTTON_DPADRIGHT, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_RIGHT);

       UpdateButton(XboxButtonID::XBOX_BUTTON_A, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_A);
       UpdateButton(XboxButtonID::XBOX_BUTTON_B, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_B);
       UpdateButton(XboxButtonID::XBOX_BUTTON_X, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_X);
       UpdateButton(XboxButtonID::XBOX_BUTTON_Y, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_Y);

       UpdateButton(XboxButtonID::XBOX_BUTTON_START, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_START);
       UpdateButton(XboxButtonID::XBOX_BUTTON_BACK, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_BACK);

       UpdateButton(XboxButtonID::XBOX_BUTTON_LEFTTHUMB, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_LEFT_THUMB);
       UpdateButton(XboxButtonID::XBOX_BUTTON_RIGHTTHUMB, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_RIGHT_THUMB);

       UpdateButton(XboxButtonID::XBOX_BUTTON_LEFTSHOULDER, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER);
       UpdateButton(XboxButtonID::XBOX_BUTTON_RIGHTSHOULDER, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER);
    }
}

void XBoxController::UpdateTriggers(float& out_triggerValue, unsigned char rawValue)
{
	out_triggerValue = RangeMap(rawValue, 0.f, 255.f, 0.f, 1.f);
}

void XBoxController::Reset()
{
}

void XBoxController::UpdateJoystick(AnalogJoystick& out_joystick, short rawX, short rawY)
{
  /*  DebuggerPrintf("XBox controller #%d reports: \n",rawY );*/
    
    out_joystick.UpdatePosition(rawX, rawY);
}
//
//void XBoxController::UpdateTrigger(float& out_triggerValue, unsigned char rawValue)
//{
//
//}

void XBoxController::UpdateButton(XboxButtonID buttonID, unsigned short buttonFlags, unsigned short buttonFlag)
{
    KeyButtonState& button = m_buttons[(int)buttonID];
    button.wasKeyDownLastFrame = m_buttons[(int)buttonID].isKeyDownThisFrame;
    button.isKeyDownThisFrame = (buttonFlags & buttonFlag) == buttonFlag;
}
