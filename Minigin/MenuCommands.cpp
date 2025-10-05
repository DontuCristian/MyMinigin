#include "MenuCommands.h"
#include "MenuSelector.h"
#include "GameObject.h"

dae::MenuSelectionCommand::MenuSelectionCommand(GameObject& obj, MenuSelectionType type):
	CommandObject(obj)
{
	m_Type = type;
}

void dae::MenuSelectionCommand::Execute()
{
	if (GetGameObject())
	{
		auto menu = GetGameObject()->GetComponent<MenuSelector>();
		if (menu)
		{
			switch (m_Type)
			{
			case MenuSelectionType::Next:
				menu->NextOption();
				break;
			case MenuSelectionType::Previous:
				menu->PreviousOption();
				break;
			case MenuSelectionType::Confirm:
				menu->ConfirmOption();
				break;
			default:
				break;
			}
		}
	}
}
