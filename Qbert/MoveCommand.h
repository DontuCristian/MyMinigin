#pragma once
#include "Command.h"
#include "glm.hpp"


namespace dae
{
	class GameObject;
	
	class MoveCommand final : public CommandObject
	{
	public:

		MoveCommand(GameObject& object, glm::vec2 direction, float speed);
		virtual ~MoveCommand() {};

		virtual void Execute() override;

		MoveCommand(const MoveCommand& other) = delete;
		MoveCommand(MoveCommand&& other) = delete;
		MoveCommand& operator=(const MoveCommand& other) = delete;
		MoveCommand& operator=(MoveCommand&& other) = delete;

	private:

		glm::vec2 m_Direction{};
		float m_Force{};
	};
}

