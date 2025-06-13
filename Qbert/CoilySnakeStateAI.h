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
	private:

		AIMoveCommand* PickMoveCommand();

		Transform* m_pTransform{ nullptr };
		Transform* m_pQBertTransform{ nullptr };

		glm::vec2 m_PlayerPos{};

		std::unique_ptr<AIMoveCommand> m_pMoveRightCommand;
		std::unique_ptr<AIMoveCommand> m_pMoveLeftCommand;
		std::unique_ptr<AIMoveCommand> m_pMoveUpRightCommand;
		std::unique_ptr<AIMoveCommand> m_pMoveUpLeftCommand;

		float m_MoveInterval{ 1.f };
		float m_MoveTimer{ 1.f };
	};
}

