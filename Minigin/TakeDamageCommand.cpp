#include "TakeDamageCommand.h"
#include "GameObject.h"
#include "Health.h"
#include <cassert>

dae::TakeDamageCommand::TakeDamageCommand(GameObject& obj, int dmg):
	CommandObject(obj),
	m_DamageAmmount{dmg}
{
}

void dae::TakeDamageCommand::Execute()
{
	assert(GetGameObject()->HasComponent<Health>());

	GetGameObject()->GetComponent<Health>()->ReduceHealth(m_DamageAmmount);
}
