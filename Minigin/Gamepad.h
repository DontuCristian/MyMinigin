#pragma once
#define WIN32_LEAN_AND_MEAN 
#include <glm.hpp>
#include <windows.h>

class Gamepad
{

public:

	Gamepad();
	~Gamepad();

	// Delete copy constructor and copy assignment operator
	Gamepad(const Gamepad& other) = delete;
	Gamepad& operator=(const Gamepad& other) = delete;

	// Delete move constructor and move assignment operator
	Gamepad(Gamepad&& other) = delete;
	Gamepad& operator=(Gamepad&& other) = delete;


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

	// Static method to check if a controller is connected
	static bool IsControllerConnected(DWORD controllerIndex);

private:

	class GamepadImpl;
	GamepadImpl* m_pImpl;
};
