#pragma once
#include "BaseComponent.h"
#include "Subject.h"
namespace dae 
{
	class GameObject;
    class Score : public BComponent, public Subject
    {
		public:

			Score(GameObject& obj);
			virtual ~Score() = default;

			Score(const Score& other) = delete;
			Score& operator=(const Score& other) = delete;
			Score(Score&& other) noexcept = delete;
			Score& operator=(Score&& other) noexcept = delete;

			void Update() override;
			void Render() const override;

			void IncreaseScore(int ammmount);

		private:
			int m_CurrentScore{ 0 };
			int m_MaxScore{1000};
    };
}

