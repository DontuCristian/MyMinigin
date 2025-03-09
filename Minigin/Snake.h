#pragma once
#include "GameActor.h"

namespace dae
{
	class Snake : public GameActor
	{
	public:

		Snake();
		virtual ~Snake() = default;

		Snake(const Snake& other) = delete;
		Snake(Snake&& other) = delete;
		Snake& operator=(const Snake& other) = delete;
		Snake& operator=(Snake&& other) = delete;

		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render() const;

		void MoveLeft();
		void MoveRight();
		void MoveUp();
		void MoveDown();

	private:


		float m_Speed = 600.f;
		glm::vec3 m_Direction{};
	};
}

