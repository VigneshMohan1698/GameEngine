#pragma once
#include "Engine/Input/XBoxController.hpp"
#include "Engine/Input/KeyButtonState.hpp"

extern const unsigned char KEYCODE_F1;
extern const unsigned char KEYCODE_F2;
extern const unsigned char KEYCODE_F3;
extern const unsigned char KEYCODE_F4;
extern const unsigned char KEYCODE_F5;
extern const unsigned char KEYCODE_F6;
extern const unsigned char KEYCODE_F7;
extern const unsigned char KEYCODE_F8;
extern const unsigned char KEYCODE_F9;
extern const unsigned char KEYCODE_F10;
extern const unsigned char KEYCODE_F11;
extern const unsigned char KEYCODE_F12;
extern const unsigned char KEYCODE_ESCAPE;
extern const unsigned char KEYCODE_UPARROW;
extern const unsigned char KEYCODE_DOWNARROW;
extern const unsigned char KEYCODE_LEFTARROW;
extern const unsigned char KEYCODE_RIGHTARROW;
extern const unsigned char KEYCODE_LEFT_MOUSE;
extern const unsigned char KEYCODE_RIGHT_MOUSE;
extern const unsigned char KEYCODE_SPACE;

constexpr int NUM_KEYCODES = 256;
constexpr int NUM_XBOX_CONTROLLERS = 4;

//enum class MouseVisibility {
//	Hidden,
//	NotHidden,
//};
//enum class MouseMode {
//	Clipped,
//	Relative,
//	
//};
//enum class MouseMovement {
//	Free,
//	Absolute
//};
struct InputSystemConfig
{

};

class InputSystem
{
public:
	InputSystem();
	InputSystem(InputSystemConfig const& inputConfig);
	~InputSystem();
	void Startup();
	void ShutDown();
	void BeginFrame();
	void EndFrame();
	bool WasKeyJustPressed(unsigned char keyCode);
	bool WasKeyJustReleased(unsigned char keyCode);
	bool IsKeyDown(unsigned char keyCode);
	void HandleKeyPressed(unsigned char keyCode);
	void HandleKeyReleased(unsigned char keyCode);
	bool HandleCharInput(int charCode);
	void ConsumeKeyJustPressed(unsigned char keyCode);
	XBoxController const& GetController(int controllerID);

	//-------------------------------MOUSE FUNCTIONS------------------------------------
	void SetMouseMode(bool hidden, bool clipped, bool relative);
	void SetCursorAtCenter();
	void UpdateCursorPositionForThesis();
	void SetCursorPosition(int positionX, int positionY);
	Vec2 GetMouseClientPosition();
	Vec2 GetMouseClientDelta();
	float GetMouseWheelDelta();
	bool m_isHidden;
	bool m_isClipped;
	bool m_isRelative;
	float m_mouseWheelDelta;
protected:
	KeyButtonState m_keyStates[NUM_KEYCODES];
	XBoxController m_controllers[NUM_XBOX_CONTROLLERS];
private:
	int m_keyBoardButtonStates = 256;
	int m_numberOfControllers = 4;
	int m_clientCenterX;
	int m_clientCenterY;
	Vec2 m_currentMousePosition;
	Vec2 m_previousMousePosition;
	Vec2 m_currentMouseTexturePosition;
	Vec2 m_previousMouseTexturePosition;
};

