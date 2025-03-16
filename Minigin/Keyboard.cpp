#include "Keyboard.h"

Keyboard::Keyboard():
	m_CurrentState(SDL_NUM_SCANCODES),
	m_PreviousState(SDL_NUM_SCANCODES)
{
}

void Keyboard::ProcessInput()
{
	int size{};

	auto currentState = SDL_GetKeyboardState(&size);

	m_PreviousState = m_CurrentState;
	m_CurrentState.assign(currentState, currentState + size);
}

bool Keyboard::IsDownThisFrame(uint8_t button) const
{
	return m_CurrentState[button] && !m_PreviousState[button];
}

bool Keyboard::IsUpThisFrame(uint8_t button) const
{
	return !m_CurrentState[button] && m_PreviousState[button];
}

bool Keyboard::IsPressed(uint8_t button) const
{
	return m_CurrentState[button];
}
