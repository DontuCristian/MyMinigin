#include "SDL_SoundSystem.h"
#include <SDL_mixer.h>
#include <SDL.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <queue>


// This code is a simple sound system using SDL2 and SDL_mixer. It allows you to play sound effects and music, manage their volume, and handle multiple sound events in a separate thread. The implementation uses mutexes and condition variables to ensure thread safety when accessing the sound events map.

class dae::SDL_SoundSystem::SDL_SoundSystemImpl
{
public:
	SDL_SoundSystemImpl();
	~SDL_SoundSystemImpl();
	void PlaySound(const std::string& path, dae::SoundId sound, const float volume, bool loops = false);
	void PlayMusic(const std::string& path, dae::SoundId sound, const float volume, bool loops = false);

	void Update();

	void Cleanup();

	static void HandleSoundEndings(int channel);

private:
	std::queue<std::pair<SoundId, Sound>> m_SoundEvents;
	std::mutex m_Mutex;
	std::condition_variable m_CondVar;
	std::thread m_SoundThread;

	bool m_IsRunning{ true };
};

dae::SDL_SoundSystem::SDL_SoundSystem() :
	m_pImpl(std::make_unique<SDL_SoundSystemImpl>())
{
}

dae::SDL_SoundSystem::~SDL_SoundSystem()
{
	//This needs to be called before the m_pImpl's destructor
	m_pImpl->Cleanup();
}

void dae::SDL_SoundSystem::PlaySound(const std::string& path, SoundId sound, const float volume, bool loops)
{
	m_pImpl->PlaySound(path, sound, volume, loops);
}

void dae::SDL_SoundSystem::PlayMusic(const std::string& path, SoundId sound, const float volume, bool loops)
{
	m_pImpl->PlayMusic(path, sound, volume, loops);
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
	m_SoundThread = std::thread([this]()
		{
			Update();		
		});
}

dae::SDL_SoundSystem::SDL_SoundSystemImpl::~SDL_SoundSystemImpl()
{
	if (m_SoundThread.joinable()) {
		m_SoundThread.join();
	}
}

void dae::SDL_SoundSystem::SDL_SoundSystemImpl::Update()
{
	while (m_IsRunning)
	{
		Mix_ChannelFinished(&dae::SDL_SoundSystem::SDL_SoundSystemImpl::HandleSoundEndings);

		std::unique_lock<std::mutex> lock(m_Mutex);

		// Wait for a sound event to be added
		m_CondVar.wait(lock, [this] { return !m_SoundEvents.empty(); });

		auto sound = m_SoundEvents.front();
		m_SoundEvents.pop();

		if (sound.second.isEffect)
		{
			//Set the volume
			Mix_Volume(-1, static_cast<int>(sound.second.volume * MIX_MAX_VOLUME));

			//Load the audio chunk and play it
			Mix_Chunk* chunk = Mix_LoadWAV(sound.second.path.c_str());
			if (!chunk)
			{
				std::cout << "Failed to load sound effect: " << Mix_GetError() << "\n";
				continue;
			}
			sound.second.channel = Mix_PlayChannel(-1, chunk, (sound.second.loops) ? -1 : 0);
		}
		else
		{
			//Set the volume
			Mix_VolumeMusic(static_cast<int>(sound.second.volume * MIX_MAX_VOLUME));
			//Load the music and play it
			Mix_Music* music = Mix_LoadMUS(sound.second.path.c_str());
			if (!music)
			{
				std::cout << "Failed to load music: " << Mix_GetError() << "\n";
				continue;
			}

			sound.second.channel = Mix_PlayMusic(music, (sound.second.loops) ? -1 : 0);

			//Free the music
			if (!Mix_Playing(sound.second.channel))
			{
				Mix_FreeMusic(music);
			}
		}

	}
}
void dae::SDL_SoundSystem::SDL_SoundSystemImpl::PlaySound(const std::string& path, SoundId sound, const float volume, bool loops)
{
	std::unique_lock<std::mutex> lock(m_Mutex);

	// Notify the condition variable to wake up the sound thread
	m_CondVar.notify_one();

	Sound tempSound{};
	tempSound.path = path;
	tempSound.volume = volume;
	tempSound.loops = loops;
	tempSound.isEffect = true;

	m_SoundEvents.push({ sound, tempSound });
}

void dae::SDL_SoundSystem::SDL_SoundSystemImpl::PlayMusic(const std::string& path, dae::SoundId sound, const float volume, bool loops)
{
	std::unique_lock<std::mutex> lock(m_Mutex);

	// Notify the condition variable to wake up the sound thread
	m_CondVar.notify_one();

	// Create a new sound event
	Sound tempSound{};
	tempSound.path = path;
	tempSound.volume = volume;
	tempSound.loops = loops;
	tempSound.isEffect = false;

	m_SoundEvents.push({ sound, tempSound });
}


void dae::SDL_SoundSystem::SDL_SoundSystemImpl::HandleSoundEndings(int channel)
{
	Mix_FreeChunk(Mix_GetChunk(channel));
	std::cout << "Sound: "<< Mix_GetChunk(channel) <<" ended on channel : " << channel << "\n";
}

void dae::SDL_SoundSystem::SDL_SoundSystemImpl::Cleanup()
{
	m_IsRunning = false;
	m_CondVar.notify_one();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
	std::cout << "Sound system cleaned up\n";
}