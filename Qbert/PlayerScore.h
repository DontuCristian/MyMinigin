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
    class PlayerScore : public BComponent, public Subject
    {
		public:

			PlayerScore(GameObject& obj);
			virtual ~PlayerScore() = default;

			PlayerScore(const PlayerScore& other) = delete;
			PlayerScore& operator=(const PlayerScore& other) = delete;
			PlayerScore(PlayerScore&& other) noexcept = delete;
			PlayerScore& operator=(PlayerScore&& other) noexcept = delete;

			void Update() override;
			void Render() const override;

			void IncreaseScore(int ammmount);

		private:
			int m_CurrentScore{ 0 };
			int m_MaxScore{10000};

			std::function<void(const physics::Collider*, const physics::CollisionPoints&)> m_OnCollisionCallback{};
			void OnCollision(const physics::Collider* other, const physics::CollisionPoints& points);
    };
}

