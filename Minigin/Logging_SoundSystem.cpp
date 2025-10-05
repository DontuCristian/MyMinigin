#include "Logging_SoundSystem.h"
#include <iostream>

dae::Logging_SoundSystem::Logging_SoundSystem(std::unique_ptr<SoundService>&& pSoundSystem):
	SoundService()
{
	m_pSoundSystem = std::move(pSoundSystem);
}

void dae::Logging_SoundSystem::PlaySound(const std::string& path, SoundId sound, const float volume, bool loops)
{
	m_pSoundSystem->PlaySound(path, sound, volume, loops);

	std::cout << "Playing sound: " << sound << " with volume: " << volume << (loops ? " (looping)" : "") << std::endl;
}
void dae::Logging_SoundSystem::PlayMusic(const std::string& path, SoundId sound, const float volume, bool loops)
{
	m_pSoundSystem->PlayMusic(path, sound, volume, loops);
	std::cout << "Playing music: " << sound << " with volume: " << volume << (loops ? " (looping)" : "") << std::endl;
}
