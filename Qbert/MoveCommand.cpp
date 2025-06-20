#include "MoveCommand.h"
#include "GameObject.h"
#include "Timer.h"
#include "PhysicsComponents.h"
#include "SoundService.h"
#include "ServiceLocator.h"
#include <iostream>
#include "Collisions.h"
#include "SpriteRenderer.h"

dae::MoveCommand::MoveCommand(GameObject& object, glm::vec2 direction, float force):
	CommandObject(object),
	m_Force{ force }
{
	m_Direction = glm::normalize(direction);

	m_OnCollisionCallback = std::bind(&MoveCommand::OnCollision, this, std::placeholders::_1, std::placeholders::_2);
	object.GetComponent<physics::Collider>()->SetCollisionCallback(m_OnCollisionCallback);

	m_pSpriteRenderer = object.GetComponent<SpriteRenderer>();
}

void dae::MoveCommand::Execute()
{
	if (m_isGrounded)
	{
		GetGameObject()->GetComponent<physics::RigidBody>()->AddForce(m_Direction, m_Force);
		ServiceLocator::GetSoundService().PlaySound("../Data/Sounds/QBertJump.mp3", 0, 4);

#ifdef _DEBUG
		//std::cout << "Direction: " << m_Direction.x << ", " << m_Direction.y << std::endl;
#endif

		if(m_Direction.x > 0 && m_Direction.y < -0.98)
			m_pSpriteRenderer->SetRowIdx(0);
		else if (m_Direction.x < 0 && m_Direction.y < -0.98)
			m_pSpriteRenderer->SetRowIdx(1);
		else if (m_Direction.x > 0 && m_Direction.y > -0.98)
			m_pSpriteRenderer->SetRowIdx(2);
		else if (m_Direction.x < 0 && m_Direction.y > -0.98)
			m_pSpriteRenderer->SetRowIdx(3);


		m_pSpriteRenderer->PlayAnim();

		m_isGrounded = false;
	}
}

void dae::MoveCommand::OnCollision(const physics::Collider* other, const physics::CollisionPoints& points)
{
	if (!other->IsTrigger && other->CompareTag("Block"))
	{
		if (points.Normal.y < 0 && points.Normal.x == 0)
		{
			m_pSpriteRenderer->StopAnim();
			m_isGrounded = true;
		}
	}
}
