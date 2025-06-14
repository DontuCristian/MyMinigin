#include "EnemyDeathHandler.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collisions.h"
#include "PhysicsComponents.h"
#include "Scene.h"
#include "Event.h"

dae::EnemyDeathHandlerComponent::EnemyDeathHandlerComponent(GameObject& obj):
	BComponent(obj)
{
	m_OnCollisionCallback = std::bind(&EnemyDeathHandlerComponent::OnCollision, this, std::placeholders::_1, std::placeholders::_2);
	obj.GetComponent<physics::Collider>()->SetCollisionCallback(m_OnCollisionCallback);

	m_pTransform = obj.GetTransform();
}
void dae::EnemyDeathHandlerComponent::Update()
{
	// Check if the enemy is below the screen
	if (m_pTransform->GetWorldPosition().y > 480 || m_pTransform->GetWorldPosition().y < 0)
	{
		if (m_pTransform->GetWorldPosition().y > 640 || m_pTransform->GetWorldPosition().x < 0)
		{
			OnDeath();
			GetOwner()->FlagForDeletion();
		}
	}
}

void dae::EnemyDeathHandlerComponent::OnDeath()
{

	Event enemyDied(make_sdbm_hash("EnemyDied"));
	enemyDied.args[0] = static_cast<std::any>(GetOwner()->GetComponent<physics::Collider>());
}



void dae::EnemyDeathHandlerComponent::OnCollision(const physics::Collider* other, const physics::CollisionPoints& points)
{
	if (other->CompareTag("KillZone"))
	{
		//Checks if the normal and the gravity vector are parallel and opposite
		if (points.Normal.y < 0 && points.Normal.x == 0)
		{
			GetOwner()->GetComponent<dae::physics::Collider>()->IsTrigger = true;
			OnDeath();
		}
	}
}