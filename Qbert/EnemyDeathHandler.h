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

	class GameObject;
	class Transform;

	class EnemyDeathHandlerComponent : public BComponent
	{
	public:
		EnemyDeathHandlerComponent(GameObject& obj);
		~EnemyDeathHandlerComponent() override = default;
		void Update() override;
		void Render() const override {};

	private:

		void DeathSequence();

		Transform* m_pTransform{ nullptr };

		void OnCollision(const physics::Collider* other, const physics::CollisionPoints& points);
		std::function<void(const physics::Collider*, const physics::CollisionPoints&)> m_OnCollisionCallback{};

	};
}
