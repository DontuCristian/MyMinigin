#include "CoilySnakeState.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collisions.h"
#include "PhysicsComponents.h"
#include "InputManager.h"
#include <glm.hpp>
#include <random>

void dae::CoilySnakeState::Enter(GameObject& obj, Transform*)
{

	m_OnCollisionCallback = std::bind(&CoilySnakeState::OnCollision, this, std::placeholders::_1, std::placeholders::_2);
	obj.GetComponent<physics::Collider>()->SetCollisionCallback(m_OnCollisionCallback);

	m_pTransform = obj.GetTransform();

	InputManager::GetInstance().AddAction("MoveUpLeft", XINPUT_GAMEPAD_DPAD_LEFT, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(obj, glm::vec2{ -1,-5.4 }, 250.f), 1);
	InputManager::GetInstance().AddAction("MoveDownRight", XINPUT_GAMEPAD_DPAD_RIGHT, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(obj, glm::vec2{ 1,-2.5 }, 120.5f), 1);
	InputManager::GetInstance().AddAction("MoveUpRight", XINPUT_GAMEPAD_DPAD_UP, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(obj, glm::vec2{ 1,-5.4 }, 250.f), 1);
	InputManager::GetInstance().AddAction("MoveDownLeft", XINPUT_GAMEPAD_DPAD_DOWN, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(obj, glm::vec2{ -1,-2.5 }, 120.5f), 1);
}

dae::CoilyState* dae::CoilySnakeState::Update()
{
	return nullptr;
}

void dae::CoilySnakeState::Exit()
{
	m_pTransform = nullptr;
}

void dae::CoilySnakeState::OnCollision(const physics::Collider*, const physics::CollisionPoints&)
{

}