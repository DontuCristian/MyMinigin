#pragma once
#include "SoundService.h"
#include "Null_SoundService.h"
#include "PhysicsService.h"
#include "Null_PhysicsService.h"
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
		static physics::PhysicsService& GetPhysicsService()
		{
			return *m_pPhysicsService;
		}

		static void RegisterSoundService(std::unique_ptr<SoundService>&& pSoundService)
		{
			m_pSoundService = pSoundService == nullptr ? std::make_unique<Null_SoundService>() : std::move(pSoundService);
		}

		static void RegisterPhysicsService(std::unique_ptr<physics::PhysicsService>&& pPhysicsService)
		{
			m_pPhysicsService = pPhysicsService == nullptr ? std::make_unique<physics::Null_PhysicsService>() : std::move(pPhysicsService);
		}

	private:

		inline static std::unique_ptr<SoundService> m_pSoundService = std::make_unique<Null_SoundService>();
		inline static std::unique_ptr<physics::PhysicsService> m_pPhysicsService = std::make_unique<physics::Null_PhysicsService>();
	};
}

