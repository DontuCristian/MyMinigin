#pragma once
#include <memory>
#include <unordered_map>
#include <typeindex>
#include "ComponentsIncludes.h"

namespace dae
{
	class GameObject final
	{
	public:
		void Update();
		void FixedUpdate();
		void Render() const;

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
			auto component = std::make_unique<Component>(this);
			m_ComponentsMap.insert({ std::type_index(typeid(Component)), std::move(component)});
		}
	
		GameObject();
		virtual ~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:

		std::unordered_map<std::type_index, std::unique_ptr<BComponent>> m_ComponentsMap{};
	};
}
