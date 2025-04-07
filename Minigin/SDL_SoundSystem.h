#pragma once
#include "SoundService.h"
#include <memory>
#include <thread>

namespace dae
{
	struct Sound
	{
		SoundId path;
		float volume;
		bool loops;
		bool isEffect;
	};

	class SDL_SoundSystem : public SoundService
	{
	public:

		SDL_SoundSystem();
		~SDL_SoundSystem() override;

		void PlaySound(const SoundId& sound, const float volume, bool loops = false) override;
		void PlayMusic(const SoundId& sound, const float volume, bool loops = false) override;

		void Update();

	private:
		
		class SDL_SoundSystemImpl;
		std::unique_ptr<SDL_SoundSystemImpl> m_pImpl;

		std::jthread m_SoundThread;
	};
}

