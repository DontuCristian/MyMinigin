#include "SDL_SoundSystem.h"
#include <SDL_mixer.h>
#include <SDL.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>

class dae::SDL_SoundSystem::SDL_SoundSystemImpl
{
public:
	SDL_SoundSystemImpl();
	~SDL_SoundSystemImpl();
	void PlaySound(const dae::SoundId& sound, const float volume, bool loops = false);
	void PlayMusic(const dae::SoundId& sound, const float volume, bool loops = false);
	void Update();

private:
	std::vector<Sound> m_SoundEvents;
	std::mutex m_Mutex;
	std::condition_variable m_CondVar;
	std::jthread m_SoundThread;
};

dae::SDL_SoundSystem::SDL_SoundSystem() :
	m_pImpl(std::make_unique<SDL_SoundSystemImpl>())
{
}

dae::SDL_SoundSystem::~SDL_SoundSystem()
{
}

void dae::SDL_SoundSystem::PlaySound(const SoundId& sound, const float volume, bool loops)
{
	m_pImpl->PlaySound(sound, volume, loops);
}

void dae::SDL_SoundSystem::PlayMusic(const SoundId& sound, const float volume, bool loops)
{
	m_pImpl->PlayMusic(sound, volume, loops);
}

void dae::SDL_SoundSystem::Update()
{
	m_pImpl->Update();
}

dae::SDL_SoundSystem::SDL_SoundSystemImpl::SDL_SoundSystemImpl()
{
	if (SDL_Init(SDL_INIT_AUDIO) == -1) {
		std::cout << "SDL_Init: " << SDL_GetError() << "\n";
		exit(1);
	}

	if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
	}

	//Create a thread to handle sound events when you create the sound system
	m_SoundThread = std::jthread([this]()
		{
			while (true)
			{
				Update();
			}
		});
}

dae::SDL_SoundSystem::SDL_SoundSystemImpl::~SDL_SoundSystemImpl()
{
	Mix_Quit();
}

void dae::SDL_SoundSystem::SDL_SoundSystemImpl::Update()
{
	std::unique_lock<std::mutex> lock(m_Mutex);
	// Wait for a sound event to be added
	m_CondVar.wait(lock, [this] { return !m_SoundEvents.empty(); });

	// Process all sound events
	for (auto& sound : m_SoundEvents)
	{
		if (sound.isEffect)
		{
			//Set the volume
			Mix_Volume(-1, static_cast<int>(sound.volume * MIX_MAX_VOLUME));

			//Load the audio chunk and play it
			Mix_Chunk* chunk = Mix_LoadWAV(sound.path.c_str());
			if (!chunk)
			{
				std::cout << "Failed to load sound effect: " << Mix_GetError() << "\n";
				continue;
			}
			int channel = Mix_PlayChannel(-1, chunk, (sound.loops) ? -1:0);

            //Free the chunk
			if (!Mix_Playing(channel))
			{
				Mix_FreeChunk(chunk);
			}
		}
		else
		{
			//Set the volume
			Mix_VolumeMusic(static_cast<int>(sound.volume * MIX_MAX_VOLUME));
			//Load the music and play it
			Mix_Music* music = Mix_LoadMUS(sound.path.c_str());
			if (!music)
			{
				std::cout << "Failed to load music: " << Mix_GetError() << "\n";
				continue;
			}

			int channel = Mix_PlayMusic(music, (sound.loops) ? -1 : 0);
			//Free the music
			if (!Mix_Playing(channel))
			{
				Mix_FreeMusic(music);
			}
		}
		m_SoundEvents.clear();
	}
}

void dae::SDL_SoundSystem::SDL_SoundSystemImpl::PlaySound(const SoundId& sound, const float volume, bool loops)
{
	std::unique_lock<std::mutex> lock(m_Mutex);

	// Notify the condition variable to wake up the sound thread
	m_CondVar.notify_one();

	Sound tempSound{};
	tempSound.path = sound;
	tempSound.volume = volume;
	tempSound.loops = loops;
	tempSound.isEffect = true;

	m_SoundEvents.push_back(tempSound);
}

void dae::SDL_SoundSystem::SDL_SoundSystemImpl::PlayMusic(const dae::SoundId& sound, const float volume, bool loops)
{
	std::unique_lock<std::mutex> lock(m_Mutex);

	// Notify the condition variable to wake up the sound thread
	m_CondVar.notify_one();

	// Create a new sound event
	Sound tempSound{};
	tempSound.path = sound;
	tempSound.volume = volume;
	tempSound.loops = loops;
	tempSound.isEffect = false;

	m_SoundEvents.push_back(tempSound);
}
