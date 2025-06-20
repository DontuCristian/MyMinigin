#include <SDL.h>
#include "InputManager.h"
#include <imgui.h>
#include "backends/imgui_impl_sdl2.h"
#include <iostream>
#include <algorithm>
#include "Xinput.h"


bool dae::InputManager::ProcessInput()
{
	CheckForConnectedGamepads();

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			return false;
		}
	}

	//Process input for all gamepads
	for (DWORD idx{}; idx<m_Gamepads.size(); idx++)
	{
		m_Gamepads[idx]->ProcessInput(idx);

		//For every action
		for (auto& action : m_ControllerActions)
		{
			switch (action.second->Event)
			{
				case TriggerEvent::Pressed:
				{
					if (m_Gamepads[idx]->IsPressed(action.second->Button))
					{
						action.second->Command->Execute();
					}
				}
					break;
				case TriggerEvent::Down:
				{
					if (m_Gamepads[idx]->IsDownThisFrame(action.second->Button))
					{
						action.second->Command->Execute();
					}
				}
					break;
				case TriggerEvent::Up:
				{
					if (m_Gamepads[idx]->IsUpThisFrame(action.second->Button))
					{
						action.second->Command->Execute();
					}
				}
					break;
			}
		}
	}
	//Process input for the keyboard
	m_Keyboard->ProcessInput();

	//For every keyboard action
	//For every action
	for (auto& action : m_KeyActions)
	{
		switch (action.second->Event)
		{
		case TriggerEvent::Pressed:
		{
			if (m_Keyboard->IsPressed(action.second->Key))
			{
				action.second->Command->Execute();
			}
		}
		break;
		case TriggerEvent::Down:
		{
			if (m_Keyboard->IsDownThisFrame(action.second->Key))
			{
				action.second->Command->Execute();
			}
		}
		break;
		case TriggerEvent::Up:
		{
			if (m_Keyboard->IsUpThisFrame(action.second->Key))
			{
				action.second->Command->Execute();
			}
		}
		break;
		}
	}

	Cleanup();
	return true;
}

void dae::InputManager::CheckForConnectedGamepads()
{

	// Periodically check for new controller connections
	for (DWORD idx = 0; idx < XUSER_MAX_COUNT; ++idx)
	{
		bool alreadyConnected = false;
		for (const auto& gamepad : m_Gamepads)
		{
			if (gamepad->IsControllerConnected(idx))
			{
				alreadyConnected = true;
				break;
			}
		}

		if (!alreadyConnected && Gamepad::IsControllerConnected(idx))
		{
			m_Gamepads.push_back(std::make_unique<Gamepad>());
			std::cout << "New controller connected: " << idx << std::endl;
		}
	}

	if (m_Keyboard == nullptr)
	{
		m_Keyboard = std::make_unique<Keyboard>();
	}
}

void dae::InputManager::AddAction(const std::string& name, const uint32_t button, const TriggerEvent state, std::unique_ptr<Command> command, const uint32_t controllerIndex)
{
		auto tempAction = std::make_unique<Action>();
		tempAction->Button = button;
		tempAction->Command = std::move(command);
		tempAction->ControllerIdx = controllerIndex;
		tempAction->Event = state;

		m_ControllerActions.insert({ name, std::move(tempAction) });
}

void dae::InputManager::AddAction(const std::string& name, const uint8_t key, const TriggerEvent state, std::unique_ptr<Command> command)
{
		auto tempAction = std::make_unique<KeyAction>();
		tempAction->Key = key;
		tempAction->Command = std::move(command);
		tempAction->Event = state;

		m_KeyActions.insert({ name, std::move(tempAction) });
}

void dae::InputManager::RemoveKeyAction(const std::string& name)
{
	auto it = m_KeyActions.find(name);

	if (it != m_KeyActions.end())
	{
		it->second->Delete = true;
	}
}

void dae::InputManager::RemoveGamepadAction(const std::string& name)
{
	auto it = m_ControllerActions.find(name);

	if (it != m_ControllerActions.end())
	{
		it->second->Delete = true;
	}
}

void dae::InputManager::RemoveAction(const std::string& name)
{
	RemoveGamepadAction(name);
	RemoveKeyAction(name);
}

void dae::InputManager::Cleanup()
{
	for (auto it = m_KeyActions.begin(); it != m_KeyActions.end(); )
	{
		if (it->second && it->second->Delete)
			it = m_KeyActions.erase(it);
		else
			++it;
	}

	for (auto it = m_ControllerActions.begin(); it != m_ControllerActions.end(); )
	{
		if (it->second && it->second->Delete)
			it = m_ControllerActions.erase(it);
		else
			++it;
	}
}


