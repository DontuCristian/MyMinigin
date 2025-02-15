#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::GameObject()
{
	AddComponent<Transform>();
}
dae::GameObject::~GameObject() = default;

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
		component->FixedUpdate();
	}
}

void dae::GameObject::Render() const
{
	for (auto& [type, component] : m_ComponentsMap)
	{
		component->Render();
	}
}
