#include "Snake.h"
#include "Timer.h"

dae::Snake::Snake()
{
	InputManager::GetInstance().SetBinding("SnakeMoveLeft", TriggerEvent::Pressed, this, &Snake::MoveLeft);
	InputManager::GetInstance().SetBinding("SnakeMoveRight", TriggerEvent::Pressed, this, &Snake::MoveRight);
	InputManager::GetInstance().SetBinding("SnakeMoveUp", TriggerEvent::Pressed, this, &Snake::MoveUp);
	InputManager::GetInstance().SetBinding("SnakeMoveDown", TriggerEvent::Pressed, this, &Snake::MoveDown);
}

void dae::Snake::Update()
{
	GameObject::Update();
}

void dae::Snake::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void dae::Snake::Render() const
{
	GameObject::Render();
}

void dae::Snake::MoveLeft()
{
	m_Direction.x = -1.f;
	m_Direction.y = 0.f;
	glm::vec3 currentPos = GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(currentPos + m_Direction * m_Speed * Timer::GetInstance().GetDeltaTime());
}

void dae::Snake::MoveRight()
{
	m_Direction.x = 1.f;
	m_Direction.y = 0.f;
	glm::vec3 currentPos = GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(currentPos + m_Direction * m_Speed * Timer::GetInstance().GetDeltaTime());
}

void dae::Snake::MoveUp()
{
	m_Direction.x = 0.f;
	m_Direction.y = -1.f;
	glm::vec3 currentPos = GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(currentPos + m_Direction * m_Speed * Timer::GetInstance().GetDeltaTime());
}

void dae::Snake::MoveDown()
{
	m_Direction.x = 0.f;
	m_Direction.y = 1.f;
	glm::vec3 currentPos = GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(currentPos + m_Direction * m_Speed * Timer::GetInstance().GetDeltaTime());
}
