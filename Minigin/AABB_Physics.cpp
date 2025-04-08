#include "AABB_Physics.h"
#include "Collisions.h"
#include "PhysicsComponents.h"


void dae::physics::AABB_Physics::FixedUpdate()
{
	ResolveCollisions();
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
	for (size_t i = 0; i < m_Colliders.size(); ++i)
	{
		for (size_t j = i + 1; j < m_Colliders.size(); ++j)
		{
			CollisionPoints points = TestCollisions(*m_Colliders[i], *m_Colliders[j]);
			if (points.Depth > 0)
			{
				Collision collision;
				collision.pColliderA = m_Colliders[i];
				collision.pColliderB = m_Colliders[j];
				collision.points = points;
			}
		}
	}
}

dae::physics::CollisionPoints TestCollisions(const dae::physics::Collider& a, const dae::physics::Collider& b)
{

	glm::vec2 aMin = a.pTransform->GetLocalPosition() + a.Offset;
	glm::vec2 aMax = aMin + glm::vec2(a.Width, a.Height);
	glm::vec2 bMin = b.pTransform->GetLocalPosition() + b.Offset;
	glm::vec2 bMax = bMin + glm::vec2(b.Width, b.Height);
	dae::physics::CollisionPoints points;
	if (aMax.x < bMin.x || aMin.x > bMax.x || aMax.y < bMin.y || aMin.y > bMax.y)
	{
		points.Depth = 0;
		return points;
	}
	points.Depth = std::min(aMax.x - bMin.x, std::min(bMax.x - aMin.x, std::min(aMax.y - bMin.y, bMax.y - aMin.y)));
	points.Normal = glm::normalize(points.A - points.B);
	return points;


}
