#pragma once
#include "BaseComponent.h"
#include "Subject.h"
#include <functional>
namespace dae
{
	namespace physics
	{
		struct Collider;
		struct CollisionPoints;
	}

	class GameObject;
	class Health final : public BComponent, public Subject
	{
	public:

		Health(GameObject& obj);
		virtual ~Health() = default;

		Health(const Health& other) = delete;
		Health& operator=(const Health& other) = delete;
		Health(Health&& other) noexcept = delete;
		Health& operator=(Health&& other) noexcept = delete;

		void Update() override;
		void Render() const override;

		void LoseLife(bool byEnemy);
		
	private:
		int m_NrLives{ 3 };
		int m_MaxNrLives{ 3 };

		void OnTrigger(const physics::Collider* other, const physics::CollisionPoints& points);
		std::function<void(const physics::Collider*, const physics::CollisionPoints&)> m_OnTriggerCallback{};
	};
}

