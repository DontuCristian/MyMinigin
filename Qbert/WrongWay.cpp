#include "WrongWay.h"
#include "GameObject.h"
#include "Transform.h"
#include "Timer.h"
#include <glm.hpp>
#include <random>

dae::WrongWay::WrongWay(GameObject& obj, const glm::vec2& upDir, const glm::vec2& downDir, float moveInterval) :
	BComponent(obj),
	m_MoveInterval{moveInterval}
{
	m_pMoveDownCommand = std::make_unique<dae::AIMoveCommand>(obj, downDir/*glm::vec2{  -2.5, 2.5 }*/, 120.5f);
	m_pMoveUpCommand = std::make_unique<dae::AIMoveCommand>(obj, upDir /*glm::vec2{-2.5, -2.5}*/ , 120.5f);

	m_MoveTimer = m_MoveInterval;
}


void dae::WrongWay::Update()
{
	m_MoveTimer -= Timer::GetInstance().GetDeltaTime();

	if (m_MoveTimer <= 0.f)
	{
		static std::default_random_engine engine{ std::random_device{}() };
		static std::uniform_int_distribution<int> dist(0, 1);

		//Switch between moving left and right
		if (m_HasMovedOnce)
		{
			if (dist(engine) == 0)
			{
				m_pMoveUpCommand->Execute();
			}
			else
			{
				m_pMoveDownCommand->Execute();
			}
		}
		else
		{
			m_pMoveUpCommand->Execute();
			m_HasMovedOnce = true;
		}

		m_MoveTimer = m_MoveInterval;
	}

	if (GetOwnerTransform()->GetWorldPosition().y >= 480)
	{
		GetOwner()->FlagForDeletion();
	}
}