#include "InputNameCommand.h"
#include "GameObject.h"
#include "TextInput.h"

dae::InputNameCommand::InputNameCommand(GameObject& obj, InputTextAction action):
	CommandObject(obj),
	m_Action{action}
{
}

void dae::InputNameCommand::Execute()
{
	auto* textInput = GetGameObject()->GetComponent<TextInput>();

	if (textInput)
	{
		switch (m_Action)
		{
		case dae::InputTextAction::Up:
			textInput->Up();
			break;
		case dae::InputTextAction::Down:
			textInput->Down();
			break;
		case dae::InputTextAction::Left:
			textInput->Left();
			break;
		case dae::InputTextAction::Right:
			textInput->Right();
			break;
		default:
			break;
		}
	}
}