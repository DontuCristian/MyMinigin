#include "MoveCommand.h"
#include "GameObject.h"
#include "Timer.h"
#include "PhysicsComponents.h"
#include "SoundService.h"
#include "ServiceLocator.h"

dae::MoveCommand::MoveCommand(GameObject& object, glm::vec2 direction, float force):
	CommandObject(object),
	m_Force{ force }
{
	m_Direction = glm::normalize(direction);
}

void dae::MoveCommand::Execute()
{
	GetGameObject()->GetComponent<physics::RigidBody>()->AddForce(m_Direction, m_Force);

	ServiceLocator::GetSoundService().PlaySound("../Data/Sounds/QBertJump.mp3",0,10);
}
