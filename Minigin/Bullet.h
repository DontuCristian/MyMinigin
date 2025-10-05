#pragma once
#include <BaseComponent.h>
#include <glm.hpp>

namespace dae
{
    namespace physics
    {
        struct RigidBody;
		struct Collider;
		struct CollisionPoints;
    }

    enum class BulletOwner
    {
        Player1,
        Player2,
        Enemy
    };

    class Bullet final : public BComponent
    {

    public:
        Bullet(GameObject& obj, glm::vec2 direction, BulletOwner owner);
        ~Bullet() override = default;
        Bullet(const Bullet& other) = delete;
        Bullet(Bullet&& other) noexcept = delete;
        Bullet& operator=(const Bullet& other) = delete;
        Bullet& operator=(Bullet&& other) noexcept = delete;

        void Update() override;
	    void Render() const override;

		void SetDirection(const glm::vec2& direction) 
        {
			if (direction == glm::vec2{ 0,0 }) return; // Avoid setting zero direction

            m_Direction = glm::normalize(direction); 
        }

    private:

        void OnTrigger(const physics::Collider* other, const physics::CollisionPoints& points);
        void OnCollision(const physics::Collider* other, const physics::CollisionPoints& points);

		float m_Speed { 100.f };
		glm::vec2 m_Direction {};

		physics::RigidBody* m_pRigidBody{ nullptr };

		float m_HitCounter{ 0.f };
        BulletOwner m_Owner; // True if shot by player, false if shot by enemy

    };
}

