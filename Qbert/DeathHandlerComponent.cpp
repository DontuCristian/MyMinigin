#include "DeathHandlerComponent.h"
#include "GameObject.h"
#include "Transform.h"

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
		DeathSequence();
	}
}

void dae::DeathHandlerComponent::OnNotify(Event event, Subject*)
{
	if (event.id == make_sdbm_hash("PlayerKilledByEnemy"))
	{
		m_ByEnemy = true;
	}

	if (event.id == make_sdbm_hash("PlayerKilledByKillZone"))
	{
		m_ByEnemy = false;
		GetOwner()->GetComponent<dae::physics::Collider>()->IsTrigger = true;
	}
}

void dae::DeathHandlerComponent::DeathSequence()
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
