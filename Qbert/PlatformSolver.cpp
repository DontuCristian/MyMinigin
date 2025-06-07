#include "PlatformSolver.h"
#include "Collisions.h"
#include "PhysicsComponents.h"
#include "Transform.h"
#include "GameObject.h"
#include "AABB_Physics.h"
#include <iostream>

void dae::physics::PlatformSolver::SolveCollision(const Collision& collision)
{
    if (collision.pColliderA->IsTrigger || collision.pColliderB->IsTrigger)
        return;// Only resolve collisions for non-trigger colliders

    glm::vec2 normal = collision.points.Normal;

    if (!collision.pRigidBodyA && !collision.pRigidBodyB)
        return; // Both static, no resolution needed

    glm::vec2 velocityA = collision.pRigidBodyA ? collision.pRigidBodyA->Velocity : glm::vec2(0.0f);
    glm::vec2 velocityB = collision.pRigidBodyB ? collision.pRigidBodyB->Velocity : glm::vec2(0.0f);

    glm::vec2 relativeVelocity = velocityA - velocityB;
    float velocityAlongNormal = glm::dot(relativeVelocity, normal);

    if (velocityAlongNormal > 0)
        return;

    if (collision.points.Normal.y < 0 && collision.points.Normal.x == 0)
    {
        if (collision.pRigidBodyA) {

            // Apply impulse to counteract gravity if collision is mostly vertical
            float gravityCounterImpulse = collision.pRigidBodyA->GravityScale;
            collision.pRigidBodyA->AddForce(-collision.pRigidBodyA->Gravity, gravityCounterImpulse);
            collision.pRigidBodyA->Velocity = glm::vec2(0.0f, 0.0f);

            // Slightly push the character out to prevent sticking
            //collision.pColliderA->pTransform->SetLocalPosition(collision.pColliderA->pTransform->GetLocalPosition() + normal * collision.points.Depth);

        }
        if (collision.pRigidBodyB) {
            // Apply impulse to counteract gravity if collision is mostly vertical
            float gravityCounterImpulse = collision.pRigidBodyB->GravityScale;
            collision.pRigidBodyB->AddForce(-collision.pRigidBodyB->Gravity, gravityCounterImpulse);
            collision.pRigidBodyB->Velocity = glm::vec2(0.0f, 0.0f);

            // Slightly push the character out to prevent sticking
            // collision.pColliderB->pTransform->SetLocalPosition(collision.pColliderB->pTransform->GetLocalPosition() + normal * collision.points.Depth);
        }

        //Center the carachter on the collider
        if (collision.pRigidBodyA && !collision.pRigidBodyB)
        {
            auto colBPosX = collision.pColliderB->pTransform->GetWorldPosition().x;
            auto colAPosY = collision.pColliderA->pTransform->GetWorldPosition().y;

            collision.pColliderA->pTransform->SetLocalPosition(colBPosX, colAPosY);
        }
        if (collision.pRigidBodyB && !collision.pRigidBodyB)
        {
            auto colBPosX = collision.pColliderA->pTransform->GetWorldPosition().x;
            auto colAPosY = collision.pColliderB->pTransform->GetWorldPosition().y;

            collision.pColliderB->pTransform->SetLocalPosition(colBPosX, colAPosY);
        }
    }
}