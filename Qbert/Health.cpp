#include "Health.h"
#include "Event.h"
#include "ServiceLocator.h"
#include "PhysicsComponents.h"
#include "Collisions.h"
#include <algorithm>

dae::Health::Health(GameObject& obj):
	BComponent(obj)
{
	m_OnTriggerCallback = std::bind(&Health::OnTrigger, this, std::placeholders::_1, std::placeholders::_2);
	obj.GetComponent<physics::Collider>()->SetTriggerCallback(m_OnTriggerCallback);
}

void dae::Health::Update()
{
	if (m_NrLives <= 0)
	{
		Event event(make_sdbm_hash("PlayerDied"));
		NotifyObservers(event);
	}
}

void dae::Health::Render() const
{
	//The health component doesn't render anything
}

void dae::Health::LoseLife()
{
	m_NrLives --;
	m_NrLives = std::clamp(m_NrLives,0,m_MaxNrLives);

	Event event(make_sdbm_hash("HealthChanged"));
	event.args[0] = static_cast<std::any>(m_NrLives);
	NotifyObservers(event);

	ServiceLocator::GetSoundService().PlaySound("../Data/Sounds/QBertJump.mp3", 0,0.5f, false);
}

void dae::Health::OnTrigger(const physics::Collider* other, const physics::CollisionPoints&)
{
	if (other->CompareTag("KillZone"))
	{
		LoseLife();
	}
}
