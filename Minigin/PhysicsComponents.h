#pragma once
#include "BaseComponent.h"
#include "glm.hpp"

namespace dae::physics
{
	struct RigidBody : public BComponent
	{
		RigidBody(dae::GameObject& obj);
		~RigidBody() override = default;

		void AddForce(const glm::vec2& direction, float strength);

		dae::Transform* pTransform; // Pointer to the Transform component of our owner
		
		glm::vec2 Gravity;	// Gravitational acceleration
		glm::vec2 Force;	// Net Force
		glm::vec2 Velocity;
			
		float Mass;	// Mass of the object

	};

	struct Collider : public BComponent
	{
		Collider(dae::GameObject& obj);
		~Collider() override = default;	

		dae::Transform* pTransform; // Pointer to the Transform component of our owner

		//This is only a Rectangular Collider
		glm::vec2 Offset; // Offset from the object's position
		float Width;
		float Height;
	};
}