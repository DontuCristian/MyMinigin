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

		void UpdateScore(physics::Collision&);
	private:

		const int m_NrOfColorChanges{ 1 };
		int m_ColorChangeCounter{ 0 };

		SpriteRenderer* m_pSpriteRenderer{ nullptr };
		std::function<void(physics::Collision&)> m_OnCollisionCallback{};
	};
}