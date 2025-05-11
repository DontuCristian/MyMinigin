#pragma once
#include "PhysicsService.h"
#include <vector>


namespace dae::physics
{
	struct RigidBody;
	struct Collider;
	struct Collision;
	class Solver;

	class AABB_Physics : public PhysicsService
	{
	public:
		AABB_Physics() = default;
		~AABB_Physics() override = default;

		void FixedUpdate() override;

		void AddRigidBody(const RigidBody* rb) override;
		void RemoveRigidBody(const RigidBody* rb) override;
		void AddCollider(const Collider* col) override;
		void RemoveCollider(const Collider* col) override;

		void AddSolver(std::unique_ptr<Solver>&& solver) override;
		void RemoveSolver(int solverIdx) override;

		void ResolveCollisions() override;
		void SendCollisionCallback(std::vector<Collision>& collisions) override;

		CollisionPoints TestCollisions(Collider& a, Collider& b) override;

	private:
		std::vector<RigidBody*> m_RigidBodies;
		std::vector<Collider*> m_Colliders;

		std::vector<std::unique_ptr<Solver>> m_Solvers; // List of solvers to be used for collision resolution
	};
}