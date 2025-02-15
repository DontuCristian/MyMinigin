#pragma once
#include <glm.hpp>
#include "BaseComponent.h"

namespace dae
{
	class GameObject;

	class Transform final : public BComponent
	{
	public:
		Transform(GameObject* obj);
		virtual ~Transform() = default;

		Transform(const Transform& other) = delete;
		Transform(Transform&& other) = delete;
		Transform& operator=(const Transform& other) = delete;
		Transform& operator=(Transform&& other) = delete;

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);

	private:
		glm::vec3 m_Position;
	};
}
