#include "Score.h"
#include "Event.h"
#include <algorithm>

dae::Score::Score(GameObject& obj) :
	BComponent(obj)
{
}

void dae::Score::Update()
{
	if (m_CurrentScore >= m_MaxScore)
	{
		Event event(make_sdbm_hash("MaxScoreAchieved"));
		NotifyObservers(event);
	}
}

void dae::Score::Render() const
{
	//The health component doesn't render anything
}

void dae::Score::IncreaseScore(int ammmount)
{
	m_CurrentScore += ammmount;
	m_CurrentScore = std::clamp(m_CurrentScore, 0, m_MaxScore);

	Event event(make_sdbm_hash("ScoreChanged"));
	event.args[0] = static_cast<std::any>(m_CurrentScore);
	NotifyObservers(event);
}
