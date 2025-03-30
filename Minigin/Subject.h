#pragma once
#include <vector>

namespace dae
{
	struct Event;
	class Observer;
	class Subject
	{
	public:
		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);

	protected:

		void NotifyObservers(Event event);

	private:
		std::vector<Observer*> m_Observers;
	};
}

