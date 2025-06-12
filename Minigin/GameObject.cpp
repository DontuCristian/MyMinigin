#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"


dae::Transform* dae::GameObject::GetTransform()
{
	assert(m_pTransform.get() != nullptr);
	return m_pTransform.get();
}

dae::GameObject::GameObject()
{
	m_pTransform = std::make_unique<Transform>(*this);
}
dae::GameObject::~GameObject() = default;

bool dae::GameObject::IsChild(GameObject* obj)
{
	auto it = std::find(m_Children.begin(), m_Children.end(), obj);
	return it != m_Children.end();
}

void dae::GameObject::Update() 
{
	for (auto& [type,component] : m_ComponentsMap)
	{
		component->Update();
	}
}

void dae::GameObject::Render() const
{
	for (auto& [type, component] : m_ComponentsMap)
	{
		component->Render();
	}
}

bool dae::GameObject::SetParent(GameObject* parent, bool keepWorldPos)
{
	if(IsChild(parent) || parent == this || parent == m_Parent)
		return false;

	if (parent == nullptr) 
	{
		m_pTransform->SetLocalPosition(m_pTransform->GetWorldPosition());
	}	
	else 
	{
		if (keepWorldPos)
			m_pTransform->SetLocalPosition(m_pTransform->GetWorldPosition() - parent->m_pTransform->GetWorldPosition());
		m_pTransform->SetPositionDirty();
	}

	//Check if this object is a child
	if (m_Parent)
	{
		m_Parent->RemoveChild(this);
	}
	m_Parent = parent;
	//Check if the nwe parent isn't nullptr
	if (m_Parent)
	{
		m_Parent->AddChild(this);
	}

	return true;

}

void dae::GameObject::RemoveChild(GameObject* obj)
{
	m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), obj), m_Children.end());
}

void dae::GameObject::AddChild(GameObject* obj)
{
	m_Children.push_back(obj);
}

dae::GameObject* dae::GameObject::GetParent()
{
	return m_Parent;
}

bool dae::GameObject::HasParent()
{
	return m_Parent != nullptr;
}
void dae::GameObject::FlagForDeletion()
{
	m_ShouldBeDeleted = true;

	for (auto* child : m_Children)
	{
		child->SetParent(nullptr, true);
	}
}
const std::vector<dae::GameObject*>& dae::GameObject::GetChildren() const
{
	return m_Children;
}
