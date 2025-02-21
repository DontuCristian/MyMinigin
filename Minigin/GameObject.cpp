#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "ComponentsIncludes.h"


dae::Transform* dae::GameObject::GetTransform()
{
	assert(m_Transform.get() != nullptr);
	return m_Transform.get();
}

dae::GameObject::GameObject()
{
    m_Transform = std::make_unique<Transform>(*this);
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

void dae::GameObject::FixedUpdate() 
{
	for (auto& [type, component] : m_ComponentsMap)
	{
		//Fix it, for later me to deal with it
		//component->FixedUpdate();
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
		m_Transform->SetLocalPosition(m_Transform->GetWorldPosition());
	}	
	else 
	{
		if (keepWorldPos)
			m_Transform->SetLocalPosition(m_Transform->GetWorldPosition() - parent->m_Transform->GetWorldPosition());
		m_Transform->SetPositionDirty();
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
