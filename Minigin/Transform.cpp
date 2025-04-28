#include "Transform.h"
#include "GameObject.h"

dae::Transform::Transform(GameObject& owner):
	BComponent(owner),
	m_LocalPosition{ 0, 0 },
	m_WorldPosition{ 0, 0 }
{
}

void dae::Transform::Update()
{
}

void dae::Transform::Render() const
{
}

const glm::vec2& dae::Transform::GetWorldPosition()
{
	if (m_IsPosDirty)
		UpdateWorldPosition();
	return m_WorldPosition;
}

const glm::vec2& dae::Transform::GetLocalPosition() const
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

void dae::Transform::SetLocalPosition(const glm::vec2& pos)
{
	m_LocalPosition = pos;
	SetPositionDirty();
}

void dae::Transform::SetLocalPosition(float x, float y)
{
	glm::vec2 pos{x,y};
	SetLocalPosition(pos);
}

void dae::Transform::SetPositionDirty()
{
	m_IsPosDirty = true;
	for (auto& child : GetOwner()->GetChildren())
	{
		child->GetTransform()->SetPositionDirty();
	}
}
