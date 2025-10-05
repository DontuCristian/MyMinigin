#include "TankSolver.h"
#include "Transform.h"
#include <glm.hpp>

void dae::physics::TankSolver::SolveCollision(const Collision& collision)
{
    if (!collision.pColliderA->CompareTag("Bullet") || !collision.pColliderB->CompareTag("Bullet"))
    {
        if (collision.pColliderA->IsTrigger || collision.pColliderB->IsTrigger)
            return;// Only resolve collisions for non-trigger colliders

        glm::vec2 normal = collision.points.Normal;

        if (!collision.pRigidBodyA && !collision.pRigidBodyB)
            return; // Both static, no resolution needed

        if ((collision.pRigidBodyA && !collision.pRigidBodyB) && !collision.pRigidBodyA->IsKinematic)
        {
            // Apply impulse to counteract the force
            float forceCounterImpulse = glm::length(collision.pRigidBodyA->Force);
            collision.pRigidBodyA->AddForce(normal, forceCounterImpulse);
        }
        if ((collision.pRigidBodyB && !collision.pRigidBodyA) && !collision.pRigidBodyB->IsKinematic)
        {
            // Apply impulse to counteract the force
            float forceCounterImpulse = glm::length(collision.pRigidBodyB->Force);
            collision.pRigidBodyA->AddForce(-normal, forceCounterImpulse);
        }
    }
}
