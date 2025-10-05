#pragma once
#include "glm.hpp"
#include "PhysicsComponents.h"
#include "GameObject.h"


namespace dae::physics
{
	struct CollisionPoints //This is a struct that contains the points of collision
	{
		glm::vec2 A; //Furthest point of object A
		glm::vec2 B; //Furthest point of object B
		glm::vec2 Normal; //Normal of the collision
		float Depth; //Depth of the collision

	};

	struct Collision //This is a collision struct that contains the colliders that are colliding and the points of collision
	{
		Collision(Collider* colliderA, Collider* colliderB, CollisionPoints points)
			: pColliderA(colliderA), pColliderB(colliderB), points(points)
		{
			pRigidBodyA = colliderA->GetOwner()->GetComponent<RigidBody>();
			pRigidBodyB = colliderB->GetOwner()->GetComponent<RigidBody>();
		}

		RigidBody* pRigidBodyA; //Pointer to the first rigid body
		RigidBody* pRigidBodyB; //Pointer to the second rigid body


		Collider* pColliderA; //Pointer to the first collider
		Collider* pColliderB; //Pointer to the second collider

		CollisionPoints points;
	};

	class Solver //This is a base solver interface, in the physics engine you would have different solvers for different types of collisions
	{
	public:
		Solver() = default;
		virtual ~Solver() = default;

		virtual void SolveCollision(const Collision& points) = 0;
	};
}