#pragma once
#include <memory>

namespace dae::physics
{
	// Forward declarations so that a different Physics service can define it's own values for these types
	// depending on those implementations
	struct Collider;
	struct RigidBody;
	struct CollisionPoints;

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

		virtual CollisionPoints TestCollisions(const Collider& a, const Collider& b) = 0;


	};
}


