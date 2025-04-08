#pragma once
#include "SoundService.h"
#include <memory>

namespace dae
{
	class Logging_SoundSystem final : public SoundService
	{

	public:
		Logging_SoundSystem(std::unique_ptr<SoundService>&& pSoundSystem);
		~Logging_SoundSystem() override = default;

		void PlaySound(const std::string& path, SoundId sound, const float volume, bool loops = false) override;
		void PlayMusic(const std::string& path, SoundId sound, const float volume, bool loops = false) override;
		void StopSound(SoundId sound) override;
		void StopMusic(SoundId sound) override;
		void PauseSound(SoundId sound) override;
		void PauseMusic(SoundId sound) override;

	private:
		std::unique_ptr<SoundService> m_pSoundSystem;
	};
}

