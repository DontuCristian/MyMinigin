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
		Block(GameObject& obj, int nrOfColorChanges, bool revertsBack);

		Block(const Block&) = delete;
		Block(Block&&) = delete;

		Block& operator=(const Block&) = delete;
		Block& operator=(Block&&) = delete;

		~Block() override = default;

		void Update() override {};
		void Render() const override {};

		int CollidedWithPlayer();
		void CollidedWithSlickOrSam();

		void SetIsEdge(bool isEdge) { m_IsEdge = isEdge; }
		bool IsEdge() const { return m_IsEdge; }

	private:

		const int m_NrOfColorChanges;
		int m_ColorChangeCounter{ 0 };
		bool m_IsEdge{ false };
		const bool m_RevertsBack;

		SpriteRenderer* m_pSpriteRenderer{ nullptr };
	};
}