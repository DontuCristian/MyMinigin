#pragma once
#include "SoundService.h"
#include <memory>
#include <thread>

namespace dae
{
	struct Sound
	{
		std::string path{};
		int channel{}; //Used for knowing which channel to stop or pause
		float volume{};
		bool loops{};
		bool isEffect{};
	};

	class SDL_SoundSystem : public SoundService
	{
	public:

		SDL_SoundSystem();
		~SDL_SoundSystem() override;

		void PlaySound(const std::string& path, SoundId sound, const float volume, bool loops = false) override;
		void PlayMusic(const std::string& path, SoundId sound, const float volume, bool loops = false) override;

		void StopSound(SoundId sound) override;
		void StopMusic(SoundId sound) override;
		void PauseSound(SoundId sound) override;
		void PauseMusic(SoundId sound) override;


		void Update();

	private:
		
		class SDL_SoundSystemImpl;
		std::unique_ptr<SDL_SoundSystemImpl> m_pImpl;
	};
}

