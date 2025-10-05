#include "AABB_Physics.h"
#include "Collisions.h"
#include "PhysicsComponents.h"
#include "Timer.h"
#include "Transform.h"


void dae::physics::AABB_Physics::FixedUpdate()
{

	ResolveCollisions();

	for (RigidBody* rb : m_RigidBodies)
	{
		if (!rb->IsKinematic)
		{
			glm::vec2 gravity = rb->Gravity * rb->GravityScale;
			rb->Force += gravity * rb->Mass; // Apply gravity to the force

			rb->Velocity = rb->Force / rb->Mass * Timer::GetInstance().GetFixedDeltaTime(); // Update velocity based on force

			glm::vec2 positionOffset = rb->pTransform->GetLocalPosition() + rb->Velocity;
			rb->pTransform->SetLocalPosition(positionOffset); // Update position based on velocity
		
			// Reset forces for the next frame
			rb->Force = glm::vec2{ 0,0 };
		}
	}
 
}

void dae::physics::AABB_Physics::AddRigidBody(const RigidBody* rb)
{
	m_RigidBodies.push_back(const_cast<RigidBody*>(rb));
}

void dae::physics::AABB_Physics::RemoveRigidBody(const RigidBody* rb)
{
	auto it = std::remove(m_RigidBodies.begin(), m_RigidBodies.end(), const_cast<RigidBody*>(rb));
	m_RigidBodies.erase(it, m_RigidBodies.end());
}

void dae::physics::AABB_Physics::AddCollider(const Collider* col)
{
	m_Colliders.push_back(const_cast<Collider*>(col));
}

void dae::physics::AABB_Physics::RemoveCollider(const Collider* col)
{
	auto it = std::remove(m_Colliders.begin(), m_Colliders.end(), const_cast<Collider*>(col));
	m_Colliders.erase(it, m_Colliders.end());
}

void dae::physics::AABB_Physics::ResolveCollisions()
{
	std::vector<Collision> collisions;
	for (Collider* collider : m_Colliders)
	{
		collider->IsColliding = false;
		collider->CollidingWithThisFrame.clear(); // Clear for this frame
	}

	for (Collider* colliderA : m_Colliders)
	{
		for (Collider* colliderB : m_Colliders)
		{
			if (colliderA == colliderB)
				break;


			CollisionPoints points = TestCollisions(*colliderA, *colliderB);

			bool isColliding = (colliderA->IsColliding && colliderB->IsColliding);

			if (isColliding)
			{
				collisions.emplace_back(colliderA, colliderB, points);

				// Track this collision for both colliders
				colliderA->CollidingWithThisFrame.insert(colliderB);
				colliderB->CollidingWithThisFrame.insert(colliderA);
			}
		}
	}

	for (auto& solver : m_Solvers)
	{
		for (auto& collision : collisions)
		{
			solver->SolveCollision(collision);
		}
	}

	// Callbacks for new collisions
	SendCollisionCallback(collisions);

	// Update last frame set
	for (Collider* collider : m_Colliders)
	{
		collider->CollidingWithLastFrame = std::move(collider->CollidingWithThisFrame);
	}
}

void dae::physics::AABB_Physics::SendCollisionCallback(std::vector<Collision>& collisions)  
{  
	for (Collision& collision : collisions)
	{
		// Only send OnCollision if this is a new collision for A
		if (collision.pColliderA->CollidingWithLastFrame.find(collision.pColliderB) == collision.pColliderA->CollidingWithLastFrame.end())
		{
			collision.pColliderA->OnCollision(collision.pColliderB, collision.points);
		}
		// Only send OnCollision if this is a new collision for B
		if (collision.pColliderB->CollidingWithLastFrame.find(collision.pColliderA) == collision.pColliderB->CollidingWithLastFrame.end())
		{
			collision.pColliderB->OnCollision(collision.pColliderA, collision.points);
		}

		// Triggers
		if (collision.pColliderA->IsTrigger || collision.pColliderB->IsTrigger)
		{
			if (collision.pColliderA->CollidingWithLastFrame.find(collision.pColliderB) == collision.pColliderA->CollidingWithLastFrame.end())
				collision.pColliderA->OnTrigger(collision.pColliderB, collision.points);
			if (collision.pColliderB->CollidingWithLastFrame.find(collision.pColliderA) == collision.pColliderB->CollidingWithLastFrame.end())
				collision.pColliderB->OnTrigger(collision.pColliderA, collision.points);
		}
	}
}

dae::physics::CollisionPoints dae::physics::AABB_Physics::TestCollisions(Collider& a, Collider& b)
{
	// Get the positions of the colliders
	glm::vec2 aPos = a.pTransform->GetWorldPosition() + a.Offset;
	glm::vec2 bPos = b.pTransform->GetWorldPosition() + b.Offset;

	// Calculate the bounds of each collider
	float aLeft = aPos.x;
	float aRight = aPos.x + a.Width;
	float aTop = aPos.y;
	float aBottom = aPos.y + a.Height;

	float bLeft = bPos.x;
	float bRight = bPos.x + b.Width;
	float bTop = bPos.y;
	float bBottom = bPos.y + b.Height;

	// Check for overlap
	bool isColliding = (aLeft < bRight && aRight > bLeft &&
		aTop < bBottom && aBottom > bTop);

	CollisionPoints points{};

	a.IsColliding = false;
	b.IsColliding = false;

	if (isColliding)
	{

		// Calculate the penetration depth
		float overlapX = std::min(aRight, bRight) - std::max(aLeft, bLeft);
		float overlapY = std::min(aBottom, bBottom) - std::max(aTop, bTop);

        points.Depth = std::min(overlapX, overlapY);

		if (overlapX < overlapY)
		{
			points.Normal = (aPos.x < bPos.x) ? glm::vec2(-1.0f, 0.0f) : glm::vec2(1.0f, 0.0f);
		}
		else
		{
			points.Normal = (aPos.y < bPos.y) ? glm::vec2(0.0f, -1.0f) : glm::vec2(0.0f, 1.0f);
		}

		a.IsColliding = true;
		b.IsColliding = true;
	}

	return points;
}

void dae::physics::AABB_Physics::RemoveSolver(int solverIdx)
{
	if (solverIdx >= 0 && solverIdx < static_cast<int>(m_Solvers.size()))
	{
		m_Solvers.erase(m_Solvers.begin() + solverIdx);
	}
}

void dae::physics::AABB_Physics::AddSolver(std::unique_ptr<Solver>&& solver)
{
	m_Solvers.push_back(std::move(solver));
}
