#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include <glm.hpp>


namespace dae
{
	class GameObject;
	class Transform;

	class DeathHandlerComponent : public BComponent, public Observer
	{
	public:
		DeathHandlerComponent(GameObject& obj);
		~DeathHandlerComponent() override = default;
		void Update() override;
		void Render() const override {};
		void OnNotify(Event event, Subject*) override;

	private:

		void DeathSequence();

		Transform* m_pTransform{ nullptr };


		glm::vec2 m_RespawnPosition;
		bool m_ByEnemy{ false };

	};
}

