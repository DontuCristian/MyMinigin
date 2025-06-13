#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);

		void Update();
		void Render() const;
		void Cleanup();
		void RemoveAll();
		std::string GetName() const { return m_name; };

		std::shared_ptr<GameObject> Find(const std::string& colliderTag); //Finds the objects based on the tag the collider has

		std::vector < std::shared_ptr<GameObject>> GetObjects() const { return m_objects; };

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;  
		Scene& operator=(Scene&& other) = delete;

	private: 
		void Remove(std::shared_ptr<GameObject> object);

		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter; 
	};

}
