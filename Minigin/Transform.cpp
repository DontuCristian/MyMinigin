#include "Transform.h"
#include "GameObject.h"

dae::Transform::Transform(GameObject& obj):
	BComponent(obj),
	m_LocalPosition{ 0, 0, 0 },
	m_WorldPosition{ 0, 0, 0 }
{
}

void dae::Transform::Update()
{
}

void dae::Transform::Render() const
{
}

const glm::vec3& dae::Transform::GetWorldPosition()
{
	if (m_IsPosDirty)
		UpdateWorldPosition();
	return m_WorldPosition;
}

const glm::vec3& dae::Transform::GetLocalPosition() const
{
	return m_LocalPosition;
}

void dae::Transform::UpdateWorldPosition()
{
	if (m_IsPosDirty)
	{
		//Don't have to delete it as this function doesn't own it
		//The scene owns int
		GameObject* parent = BComponent::GetOwner()->GetParent();

		if (parent == nullptr)
			m_WorldPosition = m_LocalPosition;
		else
			m_WorldPosition = parent->GetTransform()->GetWorldPosition() + m_LocalPosition;

		//Just to be safe
		parent = nullptr;
	}
	m_IsPosDirty = false;
}

void dae::Transform::SetLocalPosition(const glm::vec3& pos)
{
	m_LocalPosition = pos;
	SetPositionDirty();
}
