#pragma once
#include <glm.hpp>
#include "BaseComponent.h"

namespace dae
{
	class GameObject;

	class Transform final : public BComponent
	{
	public:
		virtual ~Transform() = default;
		explicit Transform(GameObject& obj);

		Transform(const Transform& other) = delete;
		Transform(Transform&& other) = delete;
		Transform& operator=(const Transform& other) = delete;
		Transform& operator=(Transform&& other) = delete;

		void Update() override;
		void Render() const override;

		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition() const;

		void UpdateWorldPosition();

		void SetLocalPosition(const glm::vec3& pos);
		void SetLocalPosition(float x, float y, float z);
		void SetPositionDirty() { m_IsPosDirty = true; }

	private:
		glm::vec3 m_LocalPosition;
		glm::vec3 m_WorldPosition;

		bool m_IsPosDirty{ false };
	};
}
