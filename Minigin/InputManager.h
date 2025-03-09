#pragma once
#include "Singleton.h"
#include "Gamepad.h"
#include "Command.h"
#include <vector>
#include <memory>


namespace dae
{
	enum class TriggerEvent
	{
		Pressed,
		Down,
		Up
	};

	//Not used yet
	//enum class ActionType
	//{
	//	Value,
	//	Button
	//};

	struct Action
	{
		TriggerEvent Event{};
		std::vector<uint32_t> Buttons{};
		std::vector<SDL_KeyCode> KeyboardButtons{};
		std::string Name{};
		std::unique_ptr<Command> Command{};
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void CheckForConnectedGamepads();

		void CreateActionWithController(const std::string& name, uint32_t button);
		void CreateActionWithKeyboard(const std::string& name, SDL_KeyCode key);
		void AddKeyToAction(const std::string& name, SDL_KeyCode key);
		void ChangeKeyToAction(const std::string& name, SDL_KeyCode key, int idx);
		void AddButtonToAction(const std::string& name, uint32_t button);
		void ChangeButtonToAction(const std::string& name, uint32_t button, int idx);

		template <typename Actor>
		void SetBinding(const std::string& name, TriggerEvent event,Actor* actor, void (Actor::* command)())
		{
			static_assert(std::is_base_of<GameObject, Actor>::value, "Actor must be derived from GameObject");

			auto it = std::find_if(m_Actions.begin(), m_Actions.end(), [&name](const Action& action) {
				return action.Name == name;
				});

			if (it != m_Actions.end())
			{
				it->Command = std::make_unique<Command>();
				it->Command->SetCommand([actor, command]() { (actor->*command)(); });
				it->Event = event;
			}
		}

	private:
		std::vector<std::unique_ptr<Gamepad>> m_Gamepads;
		std::vector<Action> m_Actions;
	};

}
