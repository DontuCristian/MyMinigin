#pragma once
#include <BaseComponent.h>
#include <functional>

namespace dae
{
    namespace physics
    {
        struct Collider;
        struct CollisionPoints;
		struct RigidBody;
    }

    class Platform : public BComponent
    {
    public:
        Platform(GameObject& obj);
        ~Platform() override = default;
        void Update() override;
        void Render() const override {};

        void LiftPlayer();

    private:

        GameObject* m_pPlayer{ nullptr };

        void OnCollision(const physics::Collider* other, const physics::CollisionPoints& points);
        std::function<void(const physics::Collider*, const physics::CollisionPoints&)> m_OnCollisionCallback{};

		bool m_LeftSide{ true };
    };
}

