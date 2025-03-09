#pragma once
#include "GameActor.h"
namespace dae
{
	class QBert : public GameActor
	{
	public:
		QBert();
		virtual ~QBert() = default;

		QBert(const QBert& other) = delete;
		QBert(QBert&& other) = delete;
		QBert& operator=(const QBert& other) = delete;
		QBert& operator=(QBert&& other) = delete;

		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render() const;

		void MoveLeft();
		void MoveRight();
		void MoveUp();
		void MoveDown();

	private:

		float m_Speed = 300.f;
		glm::vec3 m_Direction{};
	};
}

