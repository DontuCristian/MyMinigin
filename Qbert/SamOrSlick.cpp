#include "SamOrSlick.h"
#include "GameObject.h"
#include "Transform.h"
#include "Timer.h"
#include "Collisions.h"
#include "Block.h"
#include "PhysicsComponents.h"
#include <glm.hpp>
#include <random>

dae::SamOrSlick::SamOrSlick(GameObject& obj) :
	BComponent(obj)
{
	m_pMoveRightCommand = std::make_unique<dae::AIMoveCommand>(obj, glm::vec2{ 1,-2.5 }, 120.5f);
	m_pMoveLeftCommand = std::make_unique<dae::AIMoveCommand>(obj, glm::vec2{ -1,-2.5 }, 120.5f);

	m_MoveTimer = m_MoveInterval;

	m_OnCollisionCallback = std::bind(&SamOrSlick::OnCollision, this, std::placeholders::_1, std::placeholders::_2);
	obj.GetComponent<physics::Collider>()->SetCollisionCallback(m_OnCollisionCallback);
}


void dae::SamOrSlick::Update()
{
	m_MoveTimer -= Timer::GetInstance().GetDeltaTime();

	if (m_MoveTimer <= 0.f)
	{
		static std::default_random_engine engine{ std::random_device{}() };
		static std::uniform_int_distribution<int> dist(0, 1);

		//Switch between moving left and right
		if (dist(engine) == 0)
		{
			m_pMoveRightCommand->Execute();
		}
		else
		{
			m_pMoveLeftCommand->Execute();
		}

		m_MoveTimer = m_MoveInterval;
	}

	if (GetOwnerTransform()->GetWorldPosition().y >= 480)
	{
		GetOwner()->FlagForDeletion();
	}
}

void dae::SamOrSlick::OnCollision(const physics::Collider* other, const physics::CollisionPoints& points)
{
	if (other->CompareTag("Block"))
	{
		if (points.Normal.y < 0 && points.Normal.x == 0)
		{
			auto* block = other->GetOwner()->GetComponent<dae::Block>();

			if (block)
			{
				block->CollidedWithSlickOrSam();
			}
		}
	}
}