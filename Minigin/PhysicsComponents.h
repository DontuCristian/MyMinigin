#pragma once
#include "BaseComponent.h"
#include "glm.hpp"
#include "Renderer.h"
#include "Hash.h"
#include <functional>
#include <unordered_set>
#include <iostream>

namespace dae
{
	class Transform;
}

namespace dae::physics
{
	struct CollisionPoints;

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

	using ColliderTag = uint32_t;

	struct Collider : public BComponent
	{
		Collider(dae::GameObject& obj);
		~Collider() override;	

		void Update() override {};
		void Render() const override;

		dae::Transform* pTransform; // Pointer to the Transform component of our owner

		//This is only a Rectangular Collider
		glm::vec2 Offset; // Offset from the object's position
		float Width;
		float Height;
		bool IsTrigger{ false };

		bool WasColliding{ false }; // Was the collider colliding last frame?
		bool IsColliding{ false }; // Is the collider colliding this frame?

		bool WasCollidingTrigger{ false }; // Was the collider colliding with a trigger last frame?
		bool IsCollidingTrigger{ false }; // Is the collider colliding with a trigger this frame?

		bool CompareTag(const std::string& tag) const;

		void SetTag(const std::string& tag);

		void SetCollisionCallback(std::function<void(const Collider*, const CollisionPoints&)>& callback)
		{ 
			m_OnCollisionCallbacks.push_back(callback);
		};

		void SetTriggerCallback(std::function<void(const Collider*, const CollisionPoints&)>& callback)
		{ 
			m_OnTriggerCallbacks.push_back(callback);
		};

		void OnCollision(const Collider* other, const CollisionPoints& points)
		{ 
			if (other->IsTrigger == false)
			{
				for (const auto& onCollision : m_OnCollisionCallbacks)
				{
					if (onCollision && (!WasColliding && IsColliding))
					{
#ifdef _DEBUG
						//std::cout << "WasColliding: " << WasColliding << " IsColliding: " << IsColliding << std::endl;
						std::cout << "OnCollision called! " << std::endl;
#endif // _DEBUG
						onCollision(other, points);
					}
				}

			}
		}

		void OnTrigger(const Collider* other, const CollisionPoints& points)
		{
			if (other->IsTrigger == true)
			{
				for (const auto& onTrigger : m_OnTriggerCallbacks)
				{
#ifdef _DEBUG
					std::cout << "WasCollidingTrigger: " << WasCollidingTrigger << " IsCollidingTrigger: " << IsCollidingTrigger << std::endl;
#endif // _DEBUG


					if (onTrigger && (!WasCollidingTrigger && IsCollidingTrigger))
					{
						#ifdef _DEBUG
					std::cout << "OnTrigger called! " << std::endl;
#endif // _DEBUG
						onTrigger(other, points);
					}
				}
			}
		}

	private:

		std::vector<std::function<void(const Collider*, const CollisionPoints&)>> m_OnCollisionCallbacks;
		std::vector<std::function<void(const Collider*, const CollisionPoints&)>> m_OnTriggerCallbacks;

		ColliderTag m_Tag{ 0 };
	};
}