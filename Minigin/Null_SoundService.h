#pragma once
#include "SoundService.h"
namespace dae
{
	class Null_SoundService : public SoundService
	{
	public:
		Null_SoundService() = default;
		~Null_SoundService() override = default;


		void PlaySound(const SoundId&, const float, bool) override
		{
			// No operation
		}
		void PlayMusic(const SoundId&, const float, bool) override
		{
			// No operation
		}

		void Update() override
		{
			// No operation
		}
	};
}

