#include "MoveCommand.h"
#include "GameObject.h"
#include "Timer.h"

dae::MoveCommand::MoveCommand(GameObject& object, glm::vec3 direction, float speed):
	CommandObject(object),
	m_Direction{direction},
	m_Speed{speed}
{
}

void dae::MoveCommand::Execute()
{
	GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + m_Direction * m_Speed * Timer::GetInstance().GetDeltaTime());
}

