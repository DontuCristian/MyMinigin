#pragma once
#include "Event.h"

namespace dae 
{
	class Subject;
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(Event event, Subject* subject) = 0;

		//Question for Alex on monday.

		//When you delete an observer it will cause an issue, cause the subject didn't remove it from its list
		//I can solve this by having a vector of all of the subjects that we're listening and then remove the observer
		//from their list, but idk if it's a good idea of linking the observers and subjects like that

	};
}

