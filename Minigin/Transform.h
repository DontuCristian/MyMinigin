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
		explicit Transform(GameObject& owner);

		Transform(const Transform& other) = delete;
		Transform(Transform&& other) = delete;
		Transform& operator=(const Transform& other) = delete;
		Transform& operator=(Transform&& other) = delete;

		void Update() override;
		void Render() const override;

		const glm::vec2& GetWorldPosition();
		const glm::vec2& GetLocalPosition() const;

		void UpdateWorldPosition();

		void SetLocalPosition(const glm::vec2& pos);
		void SetLocalPosition(float x, float y);

		void SetPositionDirty();

	private:
		glm::vec2 m_LocalPosition;
		glm::vec2 m_WorldPosition;

		bool m_IsPosDirty{ false };
	};
}
