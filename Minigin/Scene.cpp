#include "Scene.h"
#include "GameObject.h"
#include "PhysicsComponents.h"
#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(GameObject* object)
{
	auto itr = std::find_if(m_objects.begin(),
		m_objects.end(),
		[object](auto& element) { return element.get() == object; });

	m_objects.erase(itr);
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

std::vector<GameObject*> dae::Scene::FindAll(const std::string& tag) const
{
	std::vector<GameObject*> foundObjects;
	for (const auto& object : m_objects)
	{
		if (object->HasComponent<physics::Collider>())
		{
			if(object->GetComponent<physics::Collider>()->CompareTag(tag))
			{
				foundObjects.push_back(object.get());
			}
		}
	}
	return foundObjects;
}

GameObject* dae::Scene::Find(const std::string& tag) const
{
	auto it = std::find_if(m_objects.begin(), m_objects.end(),
		[&tag](const std::unique_ptr<GameObject>& obj)
		{
			if (obj->HasComponent<physics::Collider>())
			{
				return obj->GetComponent<physics::Collider>()->CompareTag(tag);
			}
			return false;
		});

	if (it != m_objects.end())
		return it->get();

	return nullptr;
}

void Scene::Update()
{
	for (size_t i = 0; i < m_objects.size(); ++i)
	{
		if (m_objects[i]) m_objects[i]->Update();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void Scene::Cleanup()
{
	for (auto& object : m_objects)
	{
		if (object != nullptr)
		{
			if (object->ShouldBeDeleted())
			{
				Remove(object.get());
			}
		}
	}
}

