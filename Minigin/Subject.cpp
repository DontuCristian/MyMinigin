#include "Subject.h"
#include "Observer.h"
#include <cassert>

void dae::Subject::AddObserver(Observer* observer)
{
	assert(observer != nullptr);

    m_Observers.push_back(observer);
}

void dae::Subject::RemoveObserver(Observer* observer)
{
    assert(observer != nullptr);

    auto it = std::remove(m_Observers.begin(), m_Observers.end(), observer);
    if (it != m_Observers.end())
    {
        m_Observers.erase(it, m_Observers.end());
    }
}

void dae::Subject::NotifyObservers(Event event)
{
    for (const auto& observer : m_Observers)
    {
        observer->OnNotify(event, this);
    }
}
