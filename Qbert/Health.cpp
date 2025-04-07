#include "Health.h"
#include "Event.h"
#include "ServiceLocator.h"
#include <algorithm>

dae::Health::Health(GameObject& obj):
	BComponent(obj)
{
}

void dae::Health::Update()
{
	if (m_CurrentHealth <= 0)
	{
		Event event(make_sdbm_hash("PlayerDied"));
		NotifyObservers(event);
	}
}

void dae::Health::Render() const
{
	//The health component doesn't render anything
}

void dae::Health::ReduceHealth(int ammmount)
{
	m_CurrentHealth -= ammmount;
	m_CurrentHealth = std::clamp(m_CurrentHealth,0,m_MaxHealth);

	Event event(make_sdbm_hash("HealthChanged"));
	event.args[0] = static_cast<std::any>(m_CurrentHealth);
	NotifyObservers(event);

	ServiceLocator::GetSoundService().PlaySound("../Data/Sounds/QBertJump.mp3", 0.5f, false);
}
