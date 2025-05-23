#pragma once
#include <string>

namespace dae
{
	using SoundId = uint16_t;

	class SoundService
	{

	public:
		SoundService() = default;
		virtual ~SoundService() = default;

		virtual void PlaySound(const std::string& path,SoundId id, const float volume, bool loops = false) = 0;
		virtual void PlayMusic(const std::string& path,SoundId id, const float volume, bool loops = false) = 0;
		virtual void StopSound( SoundId id) = 0;
		virtual void StopMusic( SoundId id) = 0;
		virtual void PauseSound( SoundId id) = 0;
		virtual void PauseMusic( SoundId id) = 0;

	};
}

