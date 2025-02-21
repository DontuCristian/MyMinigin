#pragma once
#include "BaseComponent.h"
#include "glm.hpp"

namespace dae
{
	class GameObject;
	class Transform;

	class Orbiter final : public BComponent
	{
	public:
		virtual ~Orbiter() = default;
		explicit Orbiter(GameObject& owner);

		Orbiter(const Orbiter& other) = delete;
		Orbiter(Orbiter&& other) = delete;
		Orbiter& operator=(const Orbiter& other) = delete;
		Orbiter& operator=(Orbiter&& other) = delete;

		void Update() override;
		void Render() const override;

		void SetOrbitRadius(float radius);
		void SetOrbitSpeed(float speed);
		void SetOrbitCenter(const glm::vec3& center);

	private:
		glm::vec3 m_OrbitCenter;
		float m_OrbitRadius;
		float m_OrbitSpeed;
		float m_CurrentAngle{ 0.0f };
	};
}

