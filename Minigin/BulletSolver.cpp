#include "BulletSolver.h"
#include "Bullet.h"
#include "Transform.h"

void dae::physics::BulletSolver::SolveCollision(const Collision& collision)
{
    if (collision.pColliderA->CompareTag("Bullet") || collision.pColliderB->CompareTag("Bullet"))
    {
        if (collision.pColliderA->IsTrigger || collision.pColliderB->IsTrigger)
            return;// Only resolve collisions for non-trigger colliders

        if (!collision.pRigidBodyA && !collision.pRigidBodyB)
            return; // Both static, no resolution needed

        if ((collision.pRigidBodyA && !collision.pRigidBodyB) && !collision.pRigidBodyA->IsKinematic)
        {
            // Calculate the reflection vector using the collision normal
            auto velocity = collision.pRigidBodyA->Velocity;
            auto normal = collision.points.Normal;
            auto reflection = velocity - 2.f * glm::dot(velocity, normal) * normal;

            // Set the new direction for the bullet
            collision.pRigidBodyA->GetOwnerComponent<Bullet>()->SetDirection(glm::normalize(reflection));

            // Adjust the bullet's position to prevent overlap
            auto penetrationDepth = collision.points.Depth; // Depth of the collision
            auto correction = normal * penetrationDepth;    // Move the bullet out of the wall
            collision.pRigidBodyA->pTransform->SetLocalPosition(
                collision.pRigidBodyA->pTransform->GetLocalPosition() + correction
            );
        }

        if ((collision.pRigidBodyB && !collision.pRigidBodyA) && !collision.pRigidBodyB->IsKinematic)
        {
            // Calculate the reflection vector using the collision normal
            auto velocity = collision.pRigidBodyB->Velocity;
            auto normal = collision.points.Normal;
            auto reflection = velocity - 2.f * glm::dot(velocity, normal) * normal;

            // Set the new direction for the bullet
            collision.pRigidBodyB->GetOwnerComponent<Bullet>()->SetDirection(glm::normalize(reflection));

            // Adjust the bullet's position to prevent overlap
            auto penetrationDepth = collision.points.Depth; // Depth of the collision
            auto correction = normal * penetrationDepth;    // Move the bullet out of the wall
            collision.pRigidBodyB->pTransform->SetLocalPosition(
                collision.pRigidBodyB->pTransform->GetLocalPosition() + correction
            );
        }
    }
}
