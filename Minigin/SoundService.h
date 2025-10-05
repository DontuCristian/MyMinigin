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

	};
}

