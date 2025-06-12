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

	class AIMoveCommand final :public CommandObject
	{
	public:
		AIMoveCommand(GameObject& object, glm::vec2 direction, float force);
		virtual ~AIMoveCommand() {};

		void Execute() override;

		AIMoveCommand(const AIMoveCommand& other) = delete;
		AIMoveCommand(AIMoveCommand&& other) = delete;
		AIMoveCommand& operator=(const AIMoveCommand& other) = delete;
		AIMoveCommand& operator=(AIMoveCommand&& other) = delete;

		void OnCollision(const physics::Collider* other, const physics::CollisionPoints& points);

	private:
		glm::vec2 m_Direction{};
		float m_Force{};
		bool m_isGrounded{};

		SpriteRenderer* m_pSpriteRenderer{ nullptr };

		std::function<void(const physics::Collider*, const physics::CollisionPoints&)> m_OnCollisionCallback{};
	};
}

