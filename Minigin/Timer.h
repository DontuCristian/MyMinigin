#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
	class Timer final : public Singleton<Timer>
	{
	public:

		void Init();

		void Update();
		void FixedUpdate();

		//Getters
		float GetDeltaTime() const { return m_DeltaTime; }
		float GetFixedDeltaTime() const { return m_FixedDeltaTime; }
		float GetFPS() const { return m_FPS; }
		float GetFrameCount() const { return m_FrameCount; }
		float GetFixedFrameCount() const { return m_FixedFrameCount; }
		std::chrono::high_resolution_clock::time_point GetThisMoment() const { return std::chrono::high_resolution_clock::now(); }
		int GetTimeStep() const;

	private:

		//=====================
		//Datamembers
		//=====================
		float m_DeltaTime{};
		//The number of update calls since the start of the game
		float m_FrameCount{};
		//The number of fixed update calls since the start of the game
		float m_FixedFrameCount{};
		//The number of frames per second
		float m_FPS{};
		//The time of the last update call
		std::chrono::high_resolution_clock::time_point m_StartTime;

		float m_Lag{};

		//=====================
		//CONSTANTS
		//=====================
		const float m_FixedDeltaTime{ 1.0f / 60.0f };
	};
}

