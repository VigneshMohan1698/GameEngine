#pragma once
#include "Engine/Input/AnalogJoystick.hpp"
#include "Engine/Input/KeyButtonState.hpp"

enum class XboxButtonID
{
	XBOX_BUTTON_A = 0,
	XBOX_BUTTON_B,
	XBOX_BUTTON_X,
	XBOX_BUTTON_Y,
	
	XBOX_BUTTON_RIGHTSHOULDER,
	XBOX_BUTTON_LEFTSHOULDER,

	XBOX_BUTTON_DPADUP,
	XBOX_BUTTON_DPADDOWN,
	XBOX_BUTTON_DPADLEFT,
	XBOX_BUTTON_DPADRIGHT,


	XBOX_BUTTON_LEFTTHUMB,
	XBOX_BUTTON_RIGHTTHUMB,

	XBOX_BUTTON_START,
	XBOX_BUTTON_BACK,

	NUM
};
class XBoxController
{
	friend class InputSystem;
public:
	XBoxController();
	XBoxController(int controllerId);
	~XBoxController();

	bool						IsConnected() const;
	int							GetControllerID() const;
	AnalogJoystick const&		GetLeftStick() const;
	AnalogJoystick const&		GetRightStick() const;
	float						GetLeftTrigger() const;
	float						GetRightTrigger() const;
	KeyButtonState const&		GetButton(XboxButtonID buttonID) const;
	bool						IsButtonDown(XboxButtonID buttonID) const;
	bool						WasButtonJustPressed(XboxButtonID buttonID) const;
	bool						WasButtonJustReleased(XboxButtonID buttonID) const;

private:
	void Update();
	void Reset();
	void UpdateJoystick(AnalogJoystick& out_joystick, short rawX, short rawY);
	void UpdateTriggers(float& out_triggerValue, unsigned char rawValue);
	void UpdateButton(XboxButtonID buttonID, unsigned short buttonFlags, unsigned short buttonFlag);

private:
	int				m_id = -1;
	bool			m_isConnected = false;
	float			m_leftTrigger = 0.f;
	float			m_rightTrigger = 0.f;
	KeyButtonState	m_buttons[(int)XboxButtonID::NUM];
	AnalogJoystick	m_lefStick;
	AnalogJoystick  m_rightStick;
};

