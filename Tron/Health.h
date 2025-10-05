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

		Health(GameObject& obj, int nrOfLives);
		virtual ~Health() = default;

		Health(const Health& other) = delete;
		Health& operator=(const Health& other) = delete;
		Health(Health&& other) noexcept = delete;
		Health& operator=(Health&& other) noexcept = delete;

		void Update() override;
		void Render() const override;

		void LoseLife();

		bool IsDead() const { return m_NrLives <= 0; }
		
	private:
		int m_NrLives{ 1 };
		int m_MaxNrLives{ 1 };

	};
}

