#pragma once
#include <vector>
#include <cstdint>
#include <SDL.h>

class Keyboard
{
public:

	Keyboard();
	~Keyboard() = default;

	//Methods
	void ProcessInput();

	bool IsDownThisFrame(uint8_t  button) const;
	bool IsUpThisFrame(uint8_t button) const;
	bool IsPressed(uint8_t button) const;

	Keyboard(const Keyboard& other) = delete;
	Keyboard& operator=(const Keyboard& other) = delete;
	Keyboard(Keyboard&& other) = delete;
	Keyboard& operator=(Keyboard&& other) = delete;

private:

	std::vector<uint8_t> m_PreviousState;
	std::vector<uint8_t> m_CurrentState;

};

