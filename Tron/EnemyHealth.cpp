#include "EnemyHealth.h"
#include "Event.h"

dae::EnemyHealth::EnemyHealth(GameObject& obj, bool isRecogniser, int maxHealth) :
	BComponent(obj), m_MaxHealth(maxHealth), m_CurrentHealth(maxHealth), m_IsRecogniser(isRecogniser)
{
}

void dae::EnemyHealth::TakeDamage()
{
    m_CurrentHealth --;

    if (m_CurrentHealth <= 0)
    {
         Event event(make_sdbm_hash("EnemyDied"));
        event.args[0] = std::any_cast<bool>(m_IsRecogniser);


        NotifyObservers(event);
        GetOwner()->FlagForDeletion();
    }
}
