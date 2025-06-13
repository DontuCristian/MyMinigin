#pragma once
#include "Command.h"
#include "glm.hpp"
#include <functional>


namespace dae
{
	namespace physics
	{
		struct Collider;
		struct CollisionPoints;
	}

	class GameObject;
	class SpriteRenderer;
	
	class MoveCommand final : public CommandObject
	{
	public:

		MoveCommand(GameObject& object, glm::vec2 direction, float force);
		virtual ~MoveCommand() {};

		virtual void Execute() override;

		MoveCommand(const MoveCommand& other) = delete;
		MoveCommand(MoveCommand&& other) = delete;
		MoveCommand& operator=(const MoveCommand& other) = delete;
		MoveCommand& operator=(MoveCommand&& other) = delete;

		void OnCollision(const physics::Collider* other, const physics::CollisionPoints& points);

		static bool IsGrounded() { return m_isGrounded; }

	protected:

		glm::vec2 m_Direction{};
		float m_Force{};
		inline static bool m_isGrounded{};

		SpriteRenderer* m_pSpriteRenderer{ nullptr };

		std::function<void(const physics::Collider*, const physics::CollisionPoints&)> m_OnCollisionCallback{};
	};
}

