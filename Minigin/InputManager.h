#pragma once
#include "Singleton.h"
#include "Gamepad.h"
#include "Command.h"
#include "Keyboard.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
//This is only for the defines
#include "Xinput.h"


namespace dae
{
	enum class TriggerEvent
	{
		Pressed,
		Down,
		Up
	};

	struct Action
	{
		TriggerEvent Event{};
		uint32_t Button{};
		uint32_t ControllerIdx{};
		std::unique_ptr<Command> Command{};
	};

	struct KeyAction
	{
		TriggerEvent Event{};
		uint8_t Key{};
		std::unique_ptr<Command> Command{};
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:

		bool ProcessInput();
		void CheckForConnectedGamepads();

		void AddAction(const std::string& name, const uint32_t button, const TriggerEvent state, std::unique_ptr<Command> command, const uint32_t controllerIndex);
		void AddAction(const std::string& name, const uint8_t key, const TriggerEvent state, std::unique_ptr<Command> command);

		//Removes a keyaction
		void RemoveKeyAction(const std::string& name);
		//Removes an action used by the controller
		void RemoveGamepadAction(const std::string& name);
		//Removes an action from both the keyboard and the gamepad
		void RemoveAction(const std::string& name);

	private:

		std::vector<std::unique_ptr<Gamepad>> m_Gamepads{};
		std::unique_ptr<Keyboard> m_Keyboard{};

		std::unordered_multimap<std::string, std::unique_ptr<Action>> m_ControllerActions;
		std::unordered_multimap<std::string, std::unique_ptr<KeyAction>> m_KeyActions;

	};

}
