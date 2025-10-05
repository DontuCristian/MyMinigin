#include "Health.h"
#include "Event.h"
#include "ServiceLocator.h"
#include "PhysicsComponents.h"
#include "Collisions.h"
#include <algorithm>
#include "LevelBuilder.h"
#include "GridMover.h"

dae::Health::Health(GameObject& obj, int nrOfLives):
	BComponent(obj),
	m_MaxNrLives(nrOfLives),
	m_NrLives(nrOfLives)
{
}
void dae::Health::Update()
{
}

void dae::Health::Render() const
{
	//The health component doesn't render anything
}

void dae::Health::LoseLife()
{
	m_NrLives --;
	m_NrLives = std::clamp(m_NrLives,0,m_MaxNrLives);

	Event healthChangedEvent(make_sdbm_hash("HealthChanged"));
	healthChangedEvent.args[0] = static_cast<std::any>(m_NrLives);
	NotifyObservers(healthChangedEvent);

	auto& levelBuilder = LevelBuilder::GetInstance();
	levelBuilder.ResetLevel("Level");

	if (m_NrLives <= 0)
	{
		GetOwnerComponent<GridMover>()->MoveTo(-100,-100);
	}
}
