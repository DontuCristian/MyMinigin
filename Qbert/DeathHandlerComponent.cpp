#include "DeathHandlerComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "GameLoop.h"
#include "LevelState.h"

dae::DeathHandlerComponent::DeathHandlerComponent(GameObject& obj):
	BComponent(obj)
{
	m_pTransform = obj.GetTransform();
	m_RespawnPosition = m_pTransform->GetWorldPosition();

}

void dae::DeathHandlerComponent::Update()
{
	// Check if the player is below the screen
	if (m_pTransform->GetWorldPosition().y > 480)
	{
		FallSequence();
	}
}

void dae::DeathHandlerComponent::OnNotify(Event event, Subject*)
{
	if (event.id == make_sdbm_hash("HealthChanged"))
	{
		m_ByEnemy = std::any_cast<bool>(event.args[1]);
		if (m_ByEnemy)
		{
			DeathSequence();
		}
	}

	if (!m_ByEnemy)
	{
		GetOwner()->GetComponent<dae::physics::Collider>()->IsTrigger = true;
	}
}

void dae::DeathHandlerComponent::FallSequence()
{

	//If fell from the platform
	//Wait until the player is below the screen
	//Play sound
	//Teleport the player to the respawn position
	if (!m_ByEnemy)
	{
		m_pTransform->SetLocalPosition(m_RespawnPosition);
		GetOwner()->GetComponent<dae::physics::Collider>()->IsTrigger = false;
		GetOwner()->GetComponent<dae::physics::RigidBody>()->Velocity = { 0, 0 };
	}
}

void dae::DeathHandlerComponent::DeathSequence()
{
	if (m_ByEnemy)
	{
		auto state = dynamic_cast<LevelState*>(GameLoop::GetInstance().GetCurrentState());

		if (state)
		{
			state->ResetLevel();
			GetOwner()->GetComponent<dae::physics::Collider>()->IsTrigger = false;
			GetOwner()->GetComponent<dae::physics::RigidBody>()->Velocity = { 0, 0 };
		}
	}
}
