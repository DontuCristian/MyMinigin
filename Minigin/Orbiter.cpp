#include "Orbiter.h"
#include "GameObject.h"
#include "Timer.h"
#include "Transform.h"

dae::Orbiter::Orbiter(GameObject& owner):
	BComponent(owner),
	m_OrbitCenter{},
	m_OrbitRadius{10},
	m_OrbitSpeed{90}
{
}

void dae::Orbiter::Update()
{
	m_CurrentAngle += m_OrbitSpeed * Timer::GetInstance().GetDeltaTime();

	if (m_CurrentAngle > 360.0f)
		m_CurrentAngle -= 360.0f;

	float radians = glm::radians(m_CurrentAngle);
	glm::vec3 newPos;
	newPos.x = m_OrbitCenter.x + m_OrbitRadius * cos(radians);
	newPos.y = m_OrbitCenter.y + m_OrbitRadius * sin(radians);
	newPos.z = m_OrbitCenter.z;

	BComponent::GetOwner()->GetTransform()->SetLocalPosition(newPos);
}

void dae::Orbiter::Render() const
{
	// Orbiter component does not render anything
}

void dae::Orbiter::SetOrbitCenter(const glm::vec3& center)
{
	m_OrbitCenter = center;
}
void dae::Orbiter::SetOrbitRadius(float radius)
{
	m_OrbitRadius = radius;
}

void dae::Orbiter::SetOrbitSpeed(float speed)
{
	m_OrbitSpeed = speed;
}
