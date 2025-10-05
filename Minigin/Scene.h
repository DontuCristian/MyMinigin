#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;

	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::unique_ptr<GameObject> object);

		void Update();
		void Render() const;
		void Cleanup();

		void RemoveAll();

		std::string GetName() const { return m_name; };

		std::vector<GameObject*>  FindAll(const std::string& tag) const; //This only works if the game objects has a collider component with that tag on it
		GameObject* Find(const  std::string& tag) const; //This only works if the game object has a collider component with that tag on it

		//std::vector<std::unique_ptr<GameObject>> GetObjects() const { return m_objects; };

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		void Remove(GameObject* object);

		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector < std::unique_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter;
	};

}
