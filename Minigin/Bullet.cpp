#include "Bullet.h"
#include "GameObject.h"
#include "PhysicsComponents.h"
#include "Collisions.h"
#include "EnemyHealth.h"
#include "Teleporter.h"
#include "Health.h"

dae::Bullet::Bullet(GameObject& obj, glm::vec2 direction, BulletOwner owner):
	BComponent(obj),
	m_Direction{ direction },
	m_Owner{ owner }
{
	obj.AddComponent<physics::RigidBody>();
	obj.GetComponent<physics::RigidBody>()->SetForce(m_Direction, m_Speed);
	obj.GetComponent<physics::RigidBody>()->GravityScale = 0.f;
	obj.AddComponent<physics::Collider>(12.f, 12.f, glm::vec2{-6.f,-6.f});
	obj.GetComponent<physics::Collider>()->SetTag(make_sdbm_hash("Bullet"));

	m_pRigidBody = obj.GetComponent<physics::RigidBody>();

	obj.GetComponent<physics::Collider>()->SetTriggerCallback(
		std::bind(&Bullet::OnTrigger, this, std::placeholders::_1, std::placeholders::_2));

	obj.GetComponent<physics::Collider>()->SetCollisionCallback(
		std::bind(&Bullet::OnCollision, this, std::placeholders::_1, std::placeholders::_2));
}

void dae::Bullet::Update()
{

	m_pRigidBody->SetForce(m_Direction, m_Speed);

	if (m_HitCounter >= 5)
	{
		GetOwner()->FlagForDeletion();
	}
}

void dae::Bullet::Render() const
{
}

void dae::Bullet::OnTrigger(const physics::Collider* other, const physics::CollisionPoints&)
{
    if (other->CompareTag("Player1") || other->CompareTag("Player2"))
    {
        auto* health = other->GetOwnerComponent<Health>();

        // Player1 bullet hits Player2
        if (m_Owner == BulletOwner::Player1 && other->CompareTag("Player2"))
        {
            if (health) health->LoseLife();
            GetOwner()->FlagForDeletion();
        }
        // Player2 bullet hits Player1
        else if (m_Owner == BulletOwner::Player2 && other->CompareTag("Player1"))
        {
            if (health) health->LoseLife();
            GetOwner()->FlagForDeletion();
        }
        // Enemy bullet hits any player
        else if (m_Owner == BulletOwner::Enemy)
        {
            if (health) health->LoseLife();
            GetOwner()->FlagForDeletion();
        }
        return;
    }

    if (other->CompareTag("Enemy") && (m_Owner == BulletOwner::Player1 || m_Owner == BulletOwner::Player2))
    {
        if (other->HasOwnerComponent<EnemyHealth>())
            other->GetOwnerComponent<EnemyHealth>()->TakeDamage();

        GetOwner()->FlagForDeletion();
        return;
    }
}

void dae::Bullet::OnCollision(const physics::Collider* other, const physics::CollisionPoints&)
{
	if (!other->CompareTag("Bullet"))
	{
		m_HitCounter++;
	}
}
