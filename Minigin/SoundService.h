#pragma once
#include <string>

namespace dae
{
	using SoundId = std::string;

	class SoundService
	{

	public:
		SoundService() = default;
		virtual ~SoundService() = default;

		virtual void PlaySound(const SoundId& path, const float volume, bool loops = false) = 0;
		virtual void PlayMusic(const SoundId& path, const float volume, bool loops = false) = 0;

		virtual void Update() = 0;
	};
}

