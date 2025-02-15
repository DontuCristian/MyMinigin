#include "Transform.h"
#include "GameObject.h"

dae::Transform::Transform(GameObject* obj):
	BComponent(obj),
	m_Position{ 0,0,0 }
{
}

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}
