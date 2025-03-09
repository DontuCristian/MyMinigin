#define WIN32_LEAN_AND_MEAN
#include "Gamepad.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <XInput.h>
#include <cstdint>

#undef min
#undef max

class Gamepad::GamepadImpl
{
public:

	GamepadImpl() = default;
	~GamepadImpl() = default;

	// Delete copy constructor and copy assignment operator
	GamepadImpl(const GamepadImpl& other) = delete;
	GamepadImpl& operator=(const GamepadImpl& other) = delete;

	// Delete move constructor and move assignment operator
	GamepadImpl(GamepadImpl&& other) = delete;
	GamepadImpl& operator=(GamepadImpl&& other) = delete;

	void ProcessInput(DWORD idx);

	bool IsDownThisFrame(uint32_t button) const;
	bool IsUpThisFrame(uint32_t button) const;
	bool IsPressed(uint32_t button) const;

	glm::vec2 LeftStickInput() const;
	glm::vec2 RightStickInput() const;

	BYTE LeftTriggerInput() const;
	BYTE RightTriggerInput() const;

	void SetLeftStickDeadzone(USHORT deadzone);
	void SetRightStickDeadzone(USHORT deadzone);
	void SetTriggerDeadzone(BYTE deadzone);


private:
    XINPUT_STATE m_PreviousState{};
    XINPUT_STATE m_CurrentState{};

    DWORD m_ButtonsPressed{};
    DWORD m_ButtonsReleased{};

    USHORT m_LeftStickDeadzone = 7849;
    USHORT m_RightStickDeadzone = 8689;
    BYTE m_TriggerDeadzone = 30;
};

void Gamepad::GamepadImpl::ProcessInput(DWORD idx)
{

    CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
    ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
    DWORD dwResult = XInputGetState(idx, &m_CurrentState);


    if (dwResult == ERROR_SUCCESS)
    {

        auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
        m_ButtonsPressed = buttonChanges & m_CurrentState.Gamepad.wButtons;
        m_ButtonsReleased = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
    }
    else
    {
        std::cout << "No Controller connected";
    }
}

bool Gamepad::GamepadImpl::IsDownThisFrame(uint32_t button) const
{
    
    return m_ButtonsPressed & button;
}

bool Gamepad::GamepadImpl::IsUpThisFrame(uint32_t button) const
{
    return m_ButtonsReleased & button;
}

bool Gamepad::GamepadImpl::IsPressed(uint32_t button) const
{
    return m_CurrentState.Gamepad.wButtons & button;
}

glm::vec2 Gamepad::GamepadImpl::LeftStickInput() const
{
    glm::vec2 input{};

    input.x = static_cast<float>(m_CurrentState.Gamepad.sThumbLX);
    input.y = static_cast<float>(m_CurrentState.Gamepad.sThumbLY);

    //determine how far the controller is pushed
    float magnitude = sqrt(input.x * input.x + input.y * input.y);

    float normalizedMagnitude = 0.f;

    //check if the controller is outside a circular dead zone
    if (magnitude > m_LeftStickDeadzone)
    {
        //determine the direction the controller is pushed
        input.x /= 32767.f - m_RightStickDeadzone;
        input.y /= 32767.f - m_RightStickDeadzone;
    }
    else //if the controller is in the deadzone zero out the magnitude
    {
        magnitude = 0.f;
        normalizedMagnitude = 0.f;

        input.x = 0.f;
        input.y = 0.f;
    }

    return input;
}

glm::vec2 Gamepad::GamepadImpl::RightStickInput() const
{
    glm::vec2 input{};

    input.x = static_cast<float>(m_CurrentState.Gamepad.sThumbRX);
    input.y = static_cast<float>(m_CurrentState.Gamepad.sThumbRY);

    //determine how far the controller is pushed
    float magnitude = sqrt(input.x * input.x + input.y * input.y);

    //check if the controller is outside a circular dead zone
    if (magnitude > m_RightStickDeadzone)
    {
        //determine the direction the controller is pushed
        input.x /= 32767.f - m_RightStickDeadzone;
        input.y /= 32767.f - m_RightStickDeadzone;
    }
    else //if the controller is in the deadzone zero out the magnitude
    {
        magnitude = 0.f;

        input.x = 0.f;
        input.y = 0.f;
    }

    return input;
}

BYTE Gamepad::GamepadImpl::LeftTriggerInput() const
{
    BYTE input = m_CurrentState.Gamepad.bLeftTrigger;

    if (input > m_TriggerDeadzone)
        return input;

    return 0;
}

BYTE Gamepad::GamepadImpl::RightTriggerInput() const
{
    BYTE input = m_CurrentState.Gamepad.bRightTrigger;

    if (input > m_TriggerDeadzone)
        return input;

    return 0;
}

void Gamepad::GamepadImpl::SetLeftStickDeadzone(USHORT deadzone)
{
    m_LeftStickDeadzone = std::clamp<USHORT>(deadzone, std::numeric_limits<USHORT>::min(), std::numeric_limits<USHORT>::max());

}

void Gamepad::GamepadImpl::SetRightStickDeadzone(USHORT deadzone)
{
    m_RightStickDeadzone = std::clamp<USHORT>(deadzone, std::numeric_limits<USHORT>::min(), std::numeric_limits<USHORT>::max());
}

void Gamepad::GamepadImpl::SetTriggerDeadzone(BYTE deadzone)
{
    m_TriggerDeadzone = std::clamp<BYTE>(deadzone, std::numeric_limits<BYTE>::min(), std::numeric_limits<BYTE>::max());
}

Gamepad::Gamepad()
{
	m_pImpl = new GamepadImpl();
}

Gamepad::~Gamepad()
{
	delete m_pImpl;
}

void Gamepad::ProcessInput(DWORD idx)
{
	m_pImpl->ProcessInput(idx);
}

bool Gamepad::IsDownThisFrame(uint32_t button) const
{
    return m_pImpl->IsDownThisFrame(button);
}

bool Gamepad::IsUpThisFrame(uint32_t button) const
{
    return m_pImpl->IsUpThisFrame(button);
}

bool Gamepad::IsPressed(uint32_t button) const
{
    return m_pImpl->IsPressed(button);
}

glm::vec2 Gamepad::LeftStickInput() const
{
	return m_pImpl->LeftStickInput();
}

glm::vec2 Gamepad::RightStickInput() const
{
    return m_pImpl->RightStickInput();
}

BYTE Gamepad::LeftTriggerInput() const
{
    return m_pImpl->LeftTriggerInput();
}

BYTE Gamepad::RightTriggerInput() const
{
    return m_pImpl->RightTriggerInput();
}

void Gamepad::SetLeftStickDeadzone(USHORT deadzone)
{
	m_pImpl->SetLeftStickDeadzone(deadzone);
}

void Gamepad::SetRightStickDeadzone(USHORT deadzone)
{
	m_pImpl->SetRightStickDeadzone(deadzone);
}

void Gamepad::SetTriggerDeadzone(BYTE deadzone)
{
	m_pImpl->SetTriggerDeadzone(deadzone);
}

bool Gamepad::IsControllerConnected(DWORD controllerIndex)
{
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));
    DWORD dwResult = XInputGetState(controllerIndex, &state);
    return dwResult == ERROR_SUCCESS;
}
