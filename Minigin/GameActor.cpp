#include "GameActor.h"
#include "Timer.h"

dae::GameActor::GameActor()
{
	InputManager::GetInstance().SetBinding("MoveLeft", TriggerEvent::Pressed,this, &GameActor::MoveLeft);
	InputManager::GetInstance().SetBinding("MoveRight", TriggerEvent::Pressed, this, &GameActor::MoveRight);
	InputManager::GetInstance().SetBinding("MoveUp", TriggerEvent::Pressed, this, &GameActor::MoveUp);
	InputManager::GetInstance().SetBinding("MoveDown", TriggerEvent::Pressed, this, &GameActor::MoveDown);
}

void dae::GameActor::Update()
{
	GameObject::Update();
}

void dae::GameActor::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void dae::GameActor::Render() const
{
	GameObject::Render();
}

void dae::GameActor::MoveLeft()
{
	m_Direction.x = -1.f;
	m_Direction.y = 0.f;
	glm::vec3 currentPos = GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(currentPos + m_Direction * m_Speed * Timer::GetInstance().GetDeltaTime());
}

void dae::GameActor::MoveRight()
{
	m_Direction.x = 1.f;
	m_Direction.y = 0.f;
	glm::vec3 currentPos = GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(currentPos + m_Direction * m_Speed * Timer::GetInstance().GetDeltaTime());
}

void dae::GameActor::MoveUp()
{
	m_Direction.x = 0.f;
	m_Direction.y = -1.f;
	glm::vec3 currentPos = GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(currentPos + m_Direction * m_Speed * Timer::GetInstance().GetDeltaTime());
}

void dae::GameActor::MoveDown()
{
	m_Direction.x = 0.f;
	m_Direction.y = 1.f;
	glm::vec3 currentPos = GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(currentPos + m_Direction * m_Speed * Timer::GetInstance().GetDeltaTime());
}
