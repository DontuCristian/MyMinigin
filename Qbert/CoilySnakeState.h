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
	private:

		Transform* m_pTransform{ nullptr };
	};
}
