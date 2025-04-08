#pragma once
#include "glm.hpp"
#include "PhysicsComponents.h"


namespace dae::physics
{
	struct CollisionPoints
	{
		glm::vec2 A; //Furthest point of object A
		glm::vec2 B; //Furthest point of object B
		glm::vec2 Normal; //Normal of the collision
		float Depth; //Depth of the collision

	};

	struct Collision
	{
		Collider* pColliderA; //Pointer to the first collider
		Collider* pColliderB; //Pointer to the second collider

		CollisionPoints points;
	};
}