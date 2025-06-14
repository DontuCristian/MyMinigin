#include "CoilyBallState.h"
#include "GameObject.h"
#include "Transform.h"
#include "Timer.h"
#include "Block.h"
#include "Collisions.h"
#include "PhysicsComponents.h"
#include "CoilySnakeState.h"
#include "CoilySnakeStateAI.h" 
#include "Coily.h"
#include <glm.hpp>
#include <random>

dae::CoilyBallState::CoilyBallState(bool controlledByHuman):
	m_ControlledByHuman{controlledByHuman}
{

}

void dae::CoilyBallState::Enter(GameObject& obj, Transform*)
{

	m_OnCollisionCallback = std::bind(&CoilyBallState::OnCollision, this, std::placeholders::_1, std::placeholders::_2);
	obj.GetComponent<physics::Collider>()->SetCollisionCallback(m_OnCollisionCallback);

	m_pMoveRightCommand = std::make_unique<dae::AIMoveCommand>(obj, glm::vec2{ 1,-2.5 }, 120.5f);
	m_pMoveLeftCommand = std::make_unique<dae::AIMoveCommand>(obj, glm::vec2{ -1,-2.5 }, 120.5f);

	m_pTransform = obj.GetTransform();
}

dae::CoilyState* dae::CoilyBallState::Update()
{
	m_MoveTimer -= Timer::GetInstance().GetDeltaTime();


	if (m_IsAtEdge && !m_ControlledByHuman)
	{
		return new CoilySnakeStateAI();
	}
	else if (m_IsAtEdge && m_ControlledByHuman)
	{
		return new CoilySnakeState();
	}

	if (m_MoveTimer <= 0.f)
	{
		static std::default_random_engine engine{ std::random_device{}() };
		static std::uniform_int_distribution<int> dist(0, 1);

		//Switch between moving left and right
		if (dist(engine) == 0)
		{
			m_pMoveLeftCommand->Execute();
		}
		else
		{
			m_pMoveRightCommand->Execute();
		}

		m_MoveTimer = m_MoveInterval;
	}

	return nullptr;
}

void dae::CoilyBallState::Exit()
{
}

void dae::CoilyBallState::OnCollision(const physics::Collider* other, const physics::CollisionPoints& points)
{

	if (other->CompareTag("Block"))
	{
		if (points.Normal.y < 0 && points.Normal.x == 0)
		{

			auto* block = other->GetOwner()->GetComponent<Block>();

			m_IsAtEdge = block->IsEdge();
		}
	}

}