#include "EnemyDeathHandler.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collisions.h"
#include "PhysicsComponents.h"
#include "Scene.h"

dae::EnemyDeathHandlerComponent::EnemyDeathHandlerComponent(GameObject& obj):
	BComponent(obj)
{
	m_OnCollisionCallback = std::bind(&EnemyDeathHandlerComponent::OnCollision, this, std::placeholders::_1, std::placeholders::_2);
	obj.GetComponent<physics::Collider>()->SetCollisionCallback(m_OnCollisionCallback);

	m_pTransform = obj.GetTransform();
}
void dae::EnemyDeathHandlerComponent::Update()
{
	// Check if the player is below the screen
	if (m_pTransform->GetWorldPosition().y > 480 || m_pTransform->GetWorldPosition().y < 0)
	{
		if (m_pTransform->GetWorldPosition().y > 640 || m_pTransform->GetWorldPosition().x < 0)
		{
			DeathSequence();
		}
	}
}

void dae::EnemyDeathHandlerComponent::DeathSequence()
{

	//If fell from the platform
	//Wait until the player is below the screen
	//Play sound
	//Teleport the player to the respawn position
	GetOwner()->GetComponent<dae::physics::Collider>()->IsTrigger = false;
	GetOwner()->GetComponent<dae::physics::RigidBody>()->Velocity = { 0, 0 };

}

void dae::EnemyDeathHandlerComponent::OnCollision(const physics::Collider* other, const physics::CollisionPoints& points)
{
	if (other->CompareTag("KillZone"))
	{
		auto* pRB = other->GetOwner()->GetComponent<physics::RigidBody>();

		//Checks if the normal and the gravity vector are parallel and opposite
		if (glm::dot(points.Normal, pRB->Gravity) == -1)
		{

			GetOwner()->GetComponent<dae::physics::Collider>()->IsTrigger = true;
			GetOwner()->GetComponent<dae::physics::RigidBody>()->Velocity = { 0, 0 };

		}
	}
}