#pragma
#include "CoilyState.h"
#include "AIMoveCommand.h"
#include <memory>

namespace dae
{
	class GameObject;
	class Transform;

	class CoilyBallState final : public CoilyState
	{
	public:
		CoilyBallState() = default;
		virtual ~CoilyBallState() = default;

		void Enter(GameObject& obj, Transform*) override;
		CoilyState* Update() override;
		void Exit()	override;

		void OnCollision(const physics::Collider* other, const physics::CollisionPoints& points);
	private:

		Transform* m_pTransform{ nullptr };

		std::unique_ptr<AIMoveCommand> m_pMoveRightCommand;
		std::unique_ptr<AIMoveCommand> m_pMoveLeftCommand;
		std::unique_ptr<AIMoveCommand> m_pMoveUpRightCommand;
		std::unique_ptr<AIMoveCommand> m_pMoveUpLeftCommand;

		std::function<void(const physics::Collider*, const physics::CollisionPoints&)> m_OnCollisionCallback{};

		float m_MoveInterval{ 2.f };
		float m_MoveTimer{ 2.f };

		bool m_IsAtEdge{ false };
	};
}