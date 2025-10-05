#pragma once
#include "SoundService.h"
namespace dae
{
	class Null_SoundService : public SoundService
	{
	public:
		Null_SoundService() = default;
		~Null_SoundService() override = default;


		void PlaySound(const std::string&, SoundId, const float, bool) override { /*No operation*/ }
		void PlayMusic(const std::string&, SoundId, const float, bool) override { /*No operation*/ }
	};
}

