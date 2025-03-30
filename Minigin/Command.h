#pragma once  

namespace dae
{
	class GameObject;

	class Command
	{
	public:
		Command() = default;
		virtual ~Command() = default;

		Command(const Command& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) = delete;

		virtual void Execute() = 0;

	};

	//Pretty bad name, but this is for the commands that are going to be executed on game objects
	class CommandObject : public Command
	{
	public:
		CommandObject(GameObject& obj) { m_pGameObject = &obj; }
		virtual ~CommandObject() = default;

		CommandObject(const CommandObject& other) = delete;
		CommandObject(CommandObject&& other) = delete;
		CommandObject& operator=(const CommandObject& other) = delete;
		CommandObject& operator=(CommandObject&& other) = delete;

	protected:

		GameObject* GetGameObject() { return m_pGameObject; }

	private:

		GameObject* m_pGameObject;
	};
}
