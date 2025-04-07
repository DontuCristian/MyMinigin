#pragma once
#include "SoundService.h"
#include "Null_SoundService.h"
#include <memory>

namespace dae
{
	class ServiceLocator final
	{
	public:
		static SoundService& GetSoundService()
		{
			return *m_pSoundService;
		}

		static void RegisterSoundService(std::unique_ptr<SoundService>&& pSoundService)
		{
			m_pSoundService = pSoundService == nullptr ? std::make_unique<Null_SoundService>() : std::move(pSoundService);
		}

	private:

		inline static std::unique_ptr<SoundService> m_pSoundService = std::make_unique<Null_SoundService>();
	};
}

