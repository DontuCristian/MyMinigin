#include "Timer.h"

void dae::Timer::Init()
{
	m_DeltaTime = 0.0f ;
	m_FPS = 0.0f;
	m_FrameCount = 0.0f;
	m_FixedFrameCount = 0.0f;
	m_StartTime = std::chrono::high_resolution_clock::now();
}

void dae::Timer::Update()
{
	const auto currentTime{ std::chrono::high_resolution_clock::now() };
	m_DeltaTime = std::chrono::duration<float>(currentTime - m_StartTime).count();
	m_StartTime = currentTime;

	m_FrameCount++;
	m_FPS = 1.0f / m_DeltaTime;
}

void dae::Timer::FixedUpdate()
{
	m_FixedFrameCount++;
}
