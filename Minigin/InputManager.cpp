#include <SDL.h>
#include "InputManager.h"
#include <imgui.h>
#include "backends/imgui_impl_sdl2.h"
#include <iostream>
#include "Xinput.h"


bool dae::InputManager::ProcessInput()
{
	CheckForConnectedGamepads();

	//Single key presses
	SDL_Event e;
	while (SDL_PollEvent(&e)) 
	{
		switch (e.type)
		{
		case SDL_QUIT:
			return false;
		case SDL_KEYDOWN:
			{
				//For every action
				for (const auto& action : m_Actions)
				{
					if (action.Event == TriggerEvent::Down)
					{
						//For every button in the action
						for (const auto& button : action.KeyboardButtons)
						{
							if (e.key.keysym.sym == button)
							{
								action.Command->Execute();
							}
						}
					}

				}
			}
			break;
		case SDL_KEYUP:
			//For every action
			for (const auto& action : m_Actions)
			{
				if (action.Event == TriggerEvent::Up)
				{
					for (const auto& button : action.KeyboardButtons)
					{
						if (e.key.keysym.sym == button)
						{
							action.Command->Execute();
						}
					}
				}
			}
			break;
		}

		//Process input for ImGui
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	//Continous key presses
	Uint8* keystate = const_cast<Uint8*>(SDL_GetKeyboardState(nullptr));

	//For every action
	for (const auto& action : m_Actions)
	{
		if (action.Event == TriggerEvent::Pressed)
		{
			//For every button in the action
			for (const auto& button : action.KeyboardButtons)
			{
				if (keystate[SDL_GetScancodeFromKey(button)])
				{
					action.Command->Execute();
				}
			}
		}
	}

	// Process input for all connected gamepads
	for (int idx{}; idx<m_Gamepads.size();++idx)
	{
		m_Gamepads[idx]->ProcessInput(idx);

		//For every action
		for (const auto& action : m_Actions)
		{
			switch (action.Event)
			{
			case TriggerEvent::Pressed:
				//For every button in the action
				for (const auto& button : action.Buttons)
				{
					if (m_Gamepads[idx]->IsPressed(button))
					{
						action.Command->Execute();
					}
				}
				break;
			case TriggerEvent::Down:
				for (const auto& button : action.Buttons)
				{
					if (m_Gamepads[idx]->IsDownThisFrame(button))
					{
						action.Command->Execute();
					}
				}
				break;
			case TriggerEvent::Up:
				for (const auto& button : action.Buttons)
				{
					if (m_Gamepads[idx]->IsUpThisFrame(button))
					{
						action.Command->Execute();
					}
				}
				break;
			}
		}
	}

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
}

void dae::InputManager::CreateActionWithController(const std::string& name, uint32_t button)
{
	Action action{};
	action.Buttons.push_back(button);
	action.Name = name;
	m_Actions.push_back(std::move(action));
}

void dae::InputManager::CreateActionWithKeyboard(const std::string& name, SDL_KeyCode key)
{
	Action action{};
	action.KeyboardButtons.push_back(key);
	action.Name = name;
	m_Actions.push_back(std::move(action));
}

void dae::InputManager::AddKeyToAction(const std::string& name, SDL_KeyCode key)
{
	//Search for the action
	auto it = std::find_if(m_Actions.begin(), m_Actions.end(), [&name](const Action& action) {
		return action.Name == name;
		});
	if (it != m_Actions.end())
	{
		//Add the button to the action
		it->KeyboardButtons.push_back(key);
	}
}

void dae::InputManager::ChangeKeyToAction(const std::string& name, SDL_KeyCode key, int idx)
{
	//Search for the action
	auto it = std::find_if(m_Actions.begin(), m_Actions.end(), [&name](const Action& action) {
		return action.Name == name;
		});
	if (it != m_Actions.end())
	{
		//Change the button to the action
		it->KeyboardButtons[idx] = key;
	}
}

void dae::InputManager::AddButtonToAction(const std::string& name, uint32_t button)
{
	//Search for the action
	auto it = std::find_if(m_Actions.begin(), m_Actions.end(), [&name](const Action& action) {
		return action.Name == name;
		});
	if (it != m_Actions.end())
	{
		//Add the button to the action
		it->Buttons.push_back(button);
	}
}

void dae::InputManager::ChangeButtonToAction(const std::string& name, uint32_t button, int idx)
{
	//Search for the action
	auto it = std::find_if(m_Actions.begin(), m_Actions.end(), [&name](const Action& action) {
		return action.Name == name;
		});
	if (it != m_Actions.end())
	{
		//Change the button to the action
		it->Buttons[idx] = button;
	}
}


