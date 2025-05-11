#pragma once
#include "PhysicsService.h"
#include "Collisions.h"

namespace dae::physics
{
	class Null_PhysicsService final : public PhysicsService
	{
	public:
		Null_PhysicsService() = default;
		~Null_PhysicsService() override = default;


		void FixedUpdate() override { /*No operation*/ }
		void AddRigidBody(const RigidBody* ) override { /*No operation*/ }
		void RemoveRigidBody(const RigidBody* ) override { /*No operation*/ }
		void AddCollider(const Collider* ) override { /*No operation*/ }
		void RemoveCollider(const Collider* ) override { /*No operation*/ }
		void AddSolver(std::unique_ptr<Solver>&&) override { /*No operation*/ }
		void RemoveSolver(int) override { /*No operation*/ }
		void ResolveCollisions() override { /*No operation*/ }
		CollisionPoints TestCollisions(Collider& , Collider& ) override 
		{ 
			/*No operation*/ 
			return CollisionPoints{};
		}
		void SendCollisionCallback(std::vector<Collision>&) override { /*No operation*/ };
		
	};
}

