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
		explicit RigidBody(dae::GameObject& obj, bool isKin = false);
		~RigidBody() override;

		void Update() override {};
		void Render() const override {};

		void AddForce(const glm::vec2& direction, float strength);
		void SetForce(const glm::vec2& direction, float strength);

		Transform* pTransform{ nullptr }; // Pointer to the Transform component of the GameObject

		glm::vec2 Gravity{ 0,1 };	// Gravitational direction, the value given should be normalized
		glm::vec2 Force{ 0,0 };	// Net Force
		glm::vec2 Velocity{ 0,0 };

		float Mass{ 1 };	// Mass of the object
		float GravityScale{ 9.81f }; // Scale of the gravity force applied to the object

		bool IsKinematic{ false }; // If true, the object will not be affected by gravity

	};

	using ColliderTag = uint32_t;

	struct Collider : public BComponent
	{
		explicit Collider(dae::GameObject& obj);
		Collider(dae::GameObject& obj, float width, float height, glm::vec2 offset = {0,0}, bool isTrigger = false);
		~Collider() override;

		void Update() override {};
		void Render() const override;

		Transform* pTransform{ nullptr }; // Pointer to the Transform component of the GameObject

		// Track which colliders we were colliding with last frame and this frame
		std::unordered_set<const Collider*> CollidingWithLastFrame;
		std::unordered_set<const Collider*> CollidingWithThisFrame;

		//This is only a Rectangular Collider
		glm::vec2 Offset; // Offset from the object's position
		float Width;
		float Height;
		bool IsTrigger{ false };

		bool IsColliding{ false }; // Is the collider colliding this frame?


		template <size_t N>
		constexpr bool CompareTag(const char(&tag)[N]) const {
			return make_sdbm_hash(tag) == m_Tag;
		}

		void SetTag(ColliderTag tag) 
		{
			m_Tag = tag;
		}

		bool CompareTag(std::string tag) const;

		void SetCollisionCallback(std::function<void(const Collider*, const CollisionPoints&)>&& callback)
		{
			m_OnCollisionCallbacks.push_back(std::move(callback));
		};

		void SetTriggerCallback(std::function<void(const Collider*, const CollisionPoints&)>&& callback)
		{
			m_OnTriggerCallbacks.push_back(std::move(callback));
		};

		void OnCollision(const Collider* other, const CollisionPoints& points);
		void OnTrigger(const Collider* other, const CollisionPoints& points);

		std::vector<std::function<void(const Collider*, const CollisionPoints&)>> m_OnCollisionCallbacks{};
		std::vector<std::function<void(const Collider*, const CollisionPoints&)>> m_OnTriggerCallbacks{};

	private:

		
		ColliderTag m_Tag{ 0 };
	};
}