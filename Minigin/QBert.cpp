#include "QBert.h"
#include "Timer.h"

dae::QBert::QBert()
{
	InputManager::GetInstance().SetBinding("QBertMoveLeft", TriggerEvent::Pressed, this, &QBert::MoveLeft);
	InputManager::GetInstance().SetBinding("QBertMoveRight", TriggerEvent::Pressed, this, &QBert::MoveRight);
	InputManager::GetInstance().SetBinding("QBertMoveUp", TriggerEvent::Pressed, this, &QBert::MoveUp);
	InputManager::GetInstance().SetBinding("QBertMoveDown", TriggerEvent::Pressed, this, &QBert::MoveDown);
}

void dae::QBert::Update()
{
	GameObject::Update();
}

void dae::QBert::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void dae::QBert::Render() const
{
	GameObject::Render();
}

void dae::QBert::MoveLeft()
{
	m_Direction.x = -1.f;
	m_Direction.y = 0.f;
	glm::vec3 currentPos = GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(currentPos + m_Direction * m_Speed * Timer::GetInstance().GetDeltaTime());
}

void dae::QBert::MoveRight()
{
	m_Direction.x = 1.f;
	m_Direction.y = 0.f;
	glm::vec3 currentPos = GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(currentPos + m_Direction * m_Speed * Timer::GetInstance().GetDeltaTime());
}

void dae::QBert::MoveUp()
{
	m_Direction.x = 0.f;
	m_Direction.y = -1.f;
	glm::vec3 currentPos = GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(currentPos + m_Direction * m_Speed * Timer::GetInstance().GetDeltaTime());
}

void dae::QBert::MoveDown()
{
	m_Direction.x = 0.f;
	m_Direction.y = 1.f;
	glm::vec3 currentPos = GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(currentPos + m_Direction * m_Speed * Timer::GetInstance().GetDeltaTime());
}
