//
//  InputSystemMac.cpp
//  Albus
//
//  Created by Vignesh Mohan on 4/6/24.
//
#ifdef __APPLE__
#include "InputSystemMac.hpp"
void InputSystemMac::Startup()
{
}

void InputSystemMac::ShutDown()
{
}

void InputSystemMac::BeginFrame()
{
    
}

void InputSystemMac::EndFrame()
{
    for (int keyindex = 0; keyindex < m_keyBoardButtonStates; keyindex++)
    {
        this->m_keyStates[keyindex].wasKeyDownLastFrame = this->m_keyStates[keyindex].isKeyDownThisFrame;

    }
    m_mouseWheelDelta = 0.0f;
    m_previousMousePosition = m_currentMousePosition;
}

bool InputSystemMac::WasKeyJustPressed(unsigned char keyCode)
{
    if (this->m_keyStates[keyCode].isKeyDownThisFrame && (this->m_keyStates[keyCode].isKeyDownThisFrame != this->m_keyStates[keyCode].wasKeyDownLastFrame))
        return true;

    return false;
}

bool InputSystemMac::WasKeyJustReleased(unsigned char keyCode)
{
    if (!this->m_keyStates[keyCode].isKeyDownThisFrame && (this->m_keyStates[keyCode].isKeyDownThisFrame != this->m_keyStates[keyCode].wasKeyDownLastFrame))
        return true;

    return false;
}

bool InputSystemMac::IsKeyDown(unsigned char keyCode)
{
    return this->m_keyStates[keyCode].isKeyDownThisFrame;
}

void InputSystemMac::HandleKeyPressed(unsigned char keyCode)
{
    this->m_keyStates[keyCode].isKeyDownThisFrame = true;
}

void InputSystemMac::HandleKeyReleased(unsigned char keyCode)
{
    this->m_keyStates[keyCode].isKeyDownThisFrame = false;
}

Vec2 InputSystemMac::GetMouseClientPosition()
{
    return m_currentMousePosition;
}

Vec2 InputSystemMac::GetMouseClientDelta()
{
    return m_currentMousePosition - m_previousMousePosition;
}

float InputSystemMac::GetMouseWheelDelta()
{
    return 0.0f;
}
#endif
