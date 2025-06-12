#pragma once
#include "CoilyState.h"
#include "AIMoveCommand.h"
#include <memory>

namespace dae
{
	class GameObject;
	class Transform;

	class CoilySnakeStateAI final : public CoilyState
	{
	public:
		CoilySnakeStateAI() = default;
		virtual ~CoilySnakeStateAI() = default;

		void Enter(GameObject& obj, Transform* playerTransform) override;
		CoilyState* Update() override;
		void Exit()	override;

		void SetPlayerTransform(Transform* transform) { m_pQBertTransform = transform; }

		void OnCollision(const physics::Collider* other, const physics::CollisionPoints& points);
	private:

		AIMoveCommand* PickMoveCommand();

		Transform* m_pTransform{ nullptr };
		Transform* m_pQBertTransform{ nullptr };

		std::unique_ptr<AIMoveCommand> m_pMoveRightCommand;
		std::unique_ptr<AIMoveCommand> m_pMoveLeftCommand;
		std::unique_ptr<AIMoveCommand> m_pMoveUpRightCommand;
		std::unique_ptr<AIMoveCommand> m_pMoveUpLeftCommand;

		std::function<void(const physics::Collider*, const physics::CollisionPoints&)> m_OnCollisionCallback{};

		float m_MoveInterval{ 2.f };
		float m_MoveTimer{ 2.f };
	};
}

