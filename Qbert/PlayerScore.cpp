#include "PlayerScore.h"
#include "Event.h"
#include <algorithm>
#include "Collisions.h"
#include "Block.h"
#include <iostream>

dae::PlayerScore::PlayerScore(GameObject& obj) :
	BComponent(obj)
{
	m_OnCollisionCallback = std::bind(&PlayerScore::OnCollision, this, std::placeholders::_1, std::placeholders::_2);
	obj.GetComponent<physics::Collider>()->SetCollisionCallback(m_OnCollisionCallback);
}

void dae::PlayerScore::Update()
{
	if (m_CurrentScore >= m_MaxScore)
	{
		Event event(make_sdbm_hash("MaxScoreAchieved"));
		NotifyObservers(event);
	}
}

void dae::PlayerScore::Render() const
{
}

void dae::PlayerScore::IncreaseScore(int ammmount)
{
	m_CurrentScore += ammmount;
	m_CurrentScore = std::clamp(m_CurrentScore, 0, m_MaxScore);

	Event event(make_sdbm_hash("ScoreChanged"));
	event.args[0] = static_cast<std::any>(m_CurrentScore);
	NotifyObservers(event);
}

void dae::PlayerScore::OnCollision(const physics::Collider* other, const physics::CollisionPoints& points)
{
	std::cout << "Player collided with something, HEY!!! " << std::endl;

	if (points.Normal.y < 0 && points.Normal.x == 0)
	{
		auto* block = other->GetOwner()->GetComponent<dae::Block>();

		if (block)
		{
			int score = block->CollidedWithPlayer();
			IncreaseScore(score);
		}
	}
}




