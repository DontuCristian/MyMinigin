#pragma once
#include "BaseComponent.h"
#include "glm.hpp"
#include "Renderer.h"
#include <functional>

namespace dae
{
	class Transform;
}

namespace dae::physics
{
	struct Collision;

	struct RigidBody : public BComponent
	{
		RigidBody(dae::GameObject& obj);
		~RigidBody() override;

		void Update() override {};
		void Render() const override {};

		void AddForce(const glm::vec2& direction, float strength);
		void SetForce(const glm::vec2& direction, float strength);

		dae::Transform* pTransform; // Pointer to the Transform component of our owner
		
		glm::vec2 Gravity{0,1};	// Gravitational direction, the value given should be normalized
		glm::vec2 Force{0,0};	// Net Force
		glm::vec2 Velocity{0,0};
			
		float Mass{1};	// Mass of the object
		float GravityScale{9.81f}; // Scale of the gravity force applied to the object

	};

	struct Collider : public BComponent
	{
		Collider(dae::GameObject& obj);
		~Collider() override;	

		void Update() override {};
		void Render() const override {  };

		dae::Transform* pTransform; // Pointer to the Transform component of our owner

		//This is only a Rectangular Collider
		glm::vec2 Offset; // Offset from the object's position
		float Width;
		float Height;

		void SetCollisionCallback(std::function<void(Collision&)>& callback) { m_OnCollision = callback; };

		void OnCollision(Collision& collision) 
		{ 
			if(m_OnCollision) 
				m_OnCollision(collision); 
		}

	private:
		std::function<void(Collision&)> m_OnCollision{};
	};
}