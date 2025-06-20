#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <typeindex>
#include <assert.h>
#include <string>
#include "EngineComponentsIncludes.h"

namespace dae
{
	class GameObject
	{
	public:
		virtual void Update();
		virtual void Render() const;

		bool SetParent(GameObject* parent, bool keepWorldPos);


		//========================
		// Templates
		// =======================
		
		//Components

		//The GameObject owns the components and manages it's lifetime that's why we 
		//return a normal pointer
		template <typename Component>
			requires std::derived_from<Component, BComponent>
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

		template <typename Component, typename... Args>
			requires std::derived_from<Component, BComponent>
		void AddComponent(Args... args) //Adds a component of that type to the object, the component is default initialized
		{
			auto component = std::make_unique<Component>(*this, std::forward<Args>(args)...);
			m_ComponentsMap.insert({ std::type_index(typeid(Component)), std::move(component) });
		}

		template <typename Component>
			requires std::derived_from<Component, BComponent>
		bool HasComponent() const //Checks if a component of the given type exists
		{
			auto it = m_ComponentsMap.find(std::type_index(typeid(Component)));

			if (it != m_ComponentsMap.end())
			{
				return true;
			}

			return false;
		}

		template <typename Component>
		void RemoveComponent() //Removes the first component of the given type
		{
			m_ComponentsMap.erase(std::type_index(typeid(Component)));
		}




		Transform* GetTransform(); //Returns a pointer to the transform component
		GameObject* GetParent(); //Returns the object's parent
		const std::vector<GameObject*>& GetChildren() const;
		bool HasParent(); //Checks if a parent exists

		void FlagForDeletion();  // Flag for deleteion and remove all children
		bool ShouldBeDeleted() const { return m_ShouldBeDeleted; } //Checks if the object should be deleted
	
		GameObject();
		virtual ~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) noexcept = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) noexcept = delete;

	private:

		//Components
		std::unique_ptr<Transform> m_pTransform;
		std::unordered_map<std::type_index, std::unique_ptr<BComponent>> m_ComponentsMap{};

		//For the scene graph
		GameObject* m_Parent{};
		std::vector<GameObject*> m_Children{};

		bool m_ShouldBeDeleted{ false }; //Flag to mark the object for deletion

		//Private members
		bool IsChild(GameObject* obj);
		void RemoveChild(GameObject* obj);
		void AddChild(GameObject* obj);
	};
}
