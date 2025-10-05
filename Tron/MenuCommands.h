#pragma once
#include <Command.h>
#include <vector>
namespace dae
{
	class GameObject;

	enum class MenuSelectionType
	{
		Next,
		Previous,
		Confirm
	};

	class MenuSelectionCommand :public CommandObject
	{
	public:

		MenuSelectionCommand(GameObject& obj, MenuSelectionType type);
		virtual ~MenuSelectionCommand() = default;

		virtual void Execute() override;
	private:

		MenuSelectionType m_Type;
	};
}

