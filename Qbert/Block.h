#pragma once
#include "BaseComponent.h"
#include "Subject.h"
#include <functional>

namespace dae
{
	class SpriteRenderer;
	class GameObject;
	namespace physics
	{
		struct Collision;
	}

	class Block : public BComponent, public Subject
	{
	public:
		Block(GameObject& obj);

		Block(const Block&) = delete;
		Block(Block&&) = delete;

		Block& operator=(const Block&) = delete;
		Block& operator=(Block&&) = delete;

		~Block() override = default;

		void Update() override {};
		void Render() const override {};

		int CollidedWithPlayer();

		void SetIsEdge(bool isEdge) { m_IsEdge = isEdge; }
		bool IsEdge() { return m_IsEdge; }

	private:

		const int m_NrOfColorChanges{ 1 };
		int m_ColorChangeCounter{ 0 };
		bool m_IsEdge{ false };

		SpriteRenderer* m_pSpriteRenderer{ nullptr };
	};
}