//
//  InputSystemMac.hpp
//  Albus
//
//  Created by Vignesh Mohan on 4/6/24.
//

#ifndef InputSystemMac_hpp
#define InputSystemMac_hpp

#include <stdio.h>
#include "Math/Vec2.hpp"
#include "../Input/KeyButtonState.hpp"

constexpr int NUM_KEYCODES = 256;

class InputSystemMac
{
public:
    InputSystemMac(){};
    ~InputSystemMac();
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
private:
    int m_keyBoardButtonStates = 256;
    int m_clientCenterX;
    int m_clientCenterY;
    Vec2 m_currentMousePosition;
    Vec2 m_previousMousePosition;
    Vec2 m_currentMouseTexturePosition;
    Vec2 m_previousMouseTexturePosition;
};

#endif /* InputSystemMac_hpp */
