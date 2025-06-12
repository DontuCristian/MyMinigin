#include "CoilySnakeStateAI.h"
#include "GameObject.h"
#include "Transform.h"
#include "Timer.h"
#include "Block.h"
#include "Collisions.h"
#include "PhysicsComponents.h"
#include <glm.hpp>
#include <random>


void dae::CoilySnakeStateAI::Enter(GameObject& obj, Transform* playerTransform)
{
	m_OnCollisionCallback = std::bind(&CoilySnakeStateAI::OnCollision, this, std::placeholders::_1, std::placeholders::_2);
	obj.GetComponent<physics::Collider>()->SetCollisionCallback(m_OnCollisionCallback);

	m_pMoveRightCommand = std::make_unique<dae::AIMoveCommand>(obj, glm::vec2{ 1,-2.5 }, 120.5f);
	m_pMoveLeftCommand = std::make_unique<dae::AIMoveCommand>(obj, glm::vec2{ -1,-2.5 }, 120.5f);
	m_pMoveUpRightCommand = std::make_unique<dae::AIMoveCommand>(obj, glm::vec2{ 1,-5.4 }, 250.f);
	m_pMoveUpLeftCommand = std::make_unique<dae::AIMoveCommand>(obj, glm::vec2{ -1,-5.4 }, 250.f);

	m_pTransform = obj.GetTransform();

	m_pQBertTransform = playerTransform;
}

dae::CoilyState* dae::CoilySnakeStateAI::Update()
{
	m_MoveTimer -= Timer::GetInstance().GetDeltaTime();


	if (m_MoveTimer <= 0.f)
	{
		static std::default_random_engine engine{ std::random_device{}() };
		static std::uniform_int_distribution<int> dist(0, 1);

		auto moveCommand = PickMoveCommand();

		moveCommand->Execute();

		m_MoveTimer = m_MoveInterval;
	}

	return nullptr;

}

dae::AIMoveCommand* dae::CoilySnakeStateAI::PickMoveCommand()
{
	if (m_pTransform && m_pQBertTransform)
	{
		//Direction to the player
		glm::vec2 direction = glm::normalize(m_pQBertTransform->GetWorldPosition() - m_pTransform->GetWorldPosition());

		//Predetermined movement directions
		glm::vec2 right = glm::normalize(glm::vec2{ 1,1 });
		glm::vec2 left = glm::normalize(glm::vec2{ -1,1 });
		glm::vec2 upRight = glm::normalize(glm::vec2{ 1,-1 });
		glm::vec2 upLeft = glm::normalize(glm::vec2{ -1,-1 });

		//Calculate the dot between the direction to the player and the predetermined movement directions
		float dotRight = glm::dot(direction, right);
		float dotLeft = glm::dot(direction, left);
		float dotUpRight = glm::dot(direction, upRight);
		float dotUpLeft = glm::dot(direction, upLeft);

		if (direction.y < 0.5f)
		{
			return (dotUpRight > dotUpLeft) ? m_pMoveUpRightCommand.get() : m_pMoveUpLeftCommand.get();
		}

		float maxDot = dotRight;
		auto* bestCommand = m_pMoveRightCommand.get();

		if (dotLeft > maxDot) {
			maxDot = dotLeft;
			bestCommand = m_pMoveLeftCommand.get();
		}
		if (dotUpRight > maxDot) {
			maxDot = dotUpRight;
			bestCommand = m_pMoveUpRightCommand.get();
		}
		if (dotUpLeft > maxDot) {
			maxDot = dotUpLeft;
			bestCommand = m_pMoveUpLeftCommand.get();
		}

		return bestCommand;
	}

	return nullptr;
}

void dae::CoilySnakeStateAI::Exit()
{
}

void dae::CoilySnakeStateAI::OnCollision(const physics::Collider*, const physics::CollisionPoints&)
{

}