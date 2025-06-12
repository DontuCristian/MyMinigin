#pragma once
#include "CoilyState.h"
#include "MoveCommand.h"
#include <memory>

namespace dae
{
	class GameObject;
	class Transform;



	class CoilySnakeState final : public CoilyState
	{
	public:
		CoilySnakeState() = default;
		virtual ~CoilySnakeState() = default;

		void Enter(GameObject& obj, Transform*) override;
		CoilyState* Update() override;
		void Exit()	override;

		void OnCollision(const physics::Collider* other, const physics::CollisionPoints& points);
	private:

		Transform* m_pTransform{ nullptr };

		std::function<void(const physics::Collider*, const physics::CollisionPoints&)> m_OnCollisionCallback{};

	};
}
