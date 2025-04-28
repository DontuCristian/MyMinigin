#pragma once
#include <memory>
#include <vector>

namespace dae::physics
{
	// Forward declarations so that a different Physics service can define it's own values for these types
	// depending on those implementations
	struct Collider;
	struct RigidBody;
	struct CollisionPoints;
	struct Collision;

	class Solver;

	class PhysicsService
	{

	public:
		PhysicsService() = default;
		virtual ~PhysicsService() = default;

		virtual void FixedUpdate() = 0;
		
		virtual void AddRigidBody(const RigidBody* rb) = 0;
		virtual void RemoveRigidBody(const RigidBody* rb) = 0;

		virtual void AddCollider(const Collider* col) = 0;
		virtual void RemoveCollider(const Collider* col) = 0;

		virtual void ResolveCollisions() = 0;

		virtual void AddSolver(std::unique_ptr<Solver>&& solver) = 0;
		virtual void RemoveSolver(int solverIdx) = 0;

		virtual CollisionPoints TestCollisions(const Collider& a, const Collider& b) = 0;

		virtual void SendCollisionCallback(std::vector<Collision>& collisions) = 0;


	};
}


