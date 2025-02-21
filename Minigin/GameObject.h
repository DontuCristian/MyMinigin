#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <typeindex>
#include <assert.h>
#include <string>

namespace dae
{
	class Transform;
	class BComponent;

	class GameObject final
	{
	public:
		void Update();
		void FixedUpdate();
		void Render() const;

		bool SetParent(GameObject* parent, bool keepWorldPos);


		//========================
		// Templates
		// =======================
		
		//Components

		//The GameObject owns the components and manages it's lifetime that's why we 
		//return a normal pointer
		template <typename Component>
		Component* GetComponent() const
		{
			auto it = m_ComponentsMap.find(std::type_index(typeid(Component)));

			if (it != m_ComponentsMap.end())
			{
				auto test = static_cast<Component*>(it->second.get());

				test;
				return static_cast<Component*>(it->second.get());
			}
			return nullptr;
		}

		template <typename Component>
		void AddComponent()
		{
			auto component = std::make_unique<Component>(*this);
			m_ComponentsMap.insert({ std::type_index(typeid(Component)), std::move(component) });
		}

		template <typename Component>
		bool HasComponent()
		{
			auto it = m_ComponentsMap.find(std::type_index(typeid(Component)));

			if (it != m_ComponentsMap.end())
			{
				return true;
			}

			return false;
		}

		template <typename Component>
		void RemoveComponent()
		{
			m_ComponentsMap.erase(std::type_index(typeid(Component)));
		}




		Transform* GetTransform();
		GameObject* GetParent();
		bool HasParent();
	
		GameObject();
		virtual ~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:

		//Components
		std::unique_ptr<Transform> m_Transform;
		std::unordered_map<std::type_index, std::unique_ptr<BComponent>> m_ComponentsMap{};

		//For the scene graph
		GameObject* m_Parent{};
		std::vector<GameObject*> m_Children{};

		//Private members
		bool IsChild(GameObject* obj);
		void RemoveChild(GameObject* obj);
		void AddChild(GameObject* obj);
	};
}
