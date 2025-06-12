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

    if ((collision.pRigidBodyA && !collision.pRigidBodyB) && !collision.pRigidBodyA->IsKinematic) 
    {
        if (collision.pColliderA->CompareTag("WrongWay"))
        {
            auto dot = glm::dot(normal, collision.pRigidBodyA->Gravity);
            dot;
        }

        if (glm::dot(normal, collision.pRigidBodyA->Gravity) == -1)
        {
            // Apply impulse to counteract gravity if collision is mostly vertical
            float gravityCounterImpulse = collision.pRigidBodyA->GravityScale;
            collision.pRigidBodyA->AddForce(-collision.pRigidBodyA->Gravity, gravityCounterImpulse);
            collision.pRigidBodyA->Velocity = glm::vec2(0.0f, 0.0f);

            // Slightly push the character out to prevent sticking
            //collision.pColliderA->pTransform->SetLocalPosition(collision.pColliderA->pTransform->GetLocalPosition() + normal * (collision.points.Depth*0.2f));

            //Center the character on the collider
            auto colBPosX = collision.pColliderB->pTransform->GetWorldPosition().x;
            auto colAPosY = collision.pColliderA->pTransform->GetWorldPosition().y;

            collision.pColliderA->pTransform->SetLocalPosition(colBPosX, colAPosY);
        }
    }
    if ((collision.pRigidBodyB && !collision.pRigidBodyA) && !collision.pRigidBodyB->IsKinematic)
    {
            auto dot = glm::dot(normal, collision.pRigidBodyB->Gravity);
            dot;       

        if (glm::dot(normal, collision.pRigidBodyB->Gravity) == -1)
        {
            // Apply impulse to counteract gravity if collision is mostly vertical
            float gravityCounterImpulse = collision.pRigidBodyB->GravityScale;
            collision.pRigidBodyB->AddForce(-collision.pRigidBodyB->Gravity, gravityCounterImpulse);
            collision.pRigidBodyB->Velocity = glm::vec2(0.0f, 0.0f);

            // Slightly push the character out to prevent sticking
            //collision.pColliderB->pTransform->SetLocalPosition(collision.pColliderB->pTransform->GetLocalPosition() + normal * collision.points.Depth);
            
            //Center the character on the collider
            auto colBPosX = collision.pColliderA->pTransform->GetWorldPosition().x;
            auto colAPosY = collision.pColliderB->pTransform->GetWorldPosition().y;

            collision.pColliderB->pTransform->SetLocalPosition(colBPosX, colAPosY);
        
        }
    }

}