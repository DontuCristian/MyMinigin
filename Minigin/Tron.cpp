#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Scene.h"
//#include "GameComponentsIncludes.h"
#include "EngineComponentsIncludes.h"
#include "ServiceLocator.h"
#include "Logging_SoundSystem.h"
#include "SDL_SoundSystem.h"
#include "AABB_Physics.h"
#include "PhysicsService.h"
//#include "PlatformSolver.h"
#include "GameLoop.h"
//#include "LevelState.h"
//#include "IntroState.h"
#include "MoveCommand.h"
#include "ShootCommand.h"
#include "ChangeShootingAngle.h"
#include "TankSolver.h"
#include "BulletSolver.h"
#include "LevelBuilder.h"
#include "IntroState.h"
#include <iostream>

void load()
{
	dae::SceneManager::GetInstance().CreateScene("Game");

#ifdef _DEBUG
	dae::ServiceLocator::RegisterSoundService(std::make_unique<dae::Logging_SoundSystem>(std::make_unique<dae::SDL_SoundSystem>()));
#else
	dae::ServiceLocator::RegisterSoundService(std::make_unique<dae::SDL_SoundSystem>());
#endif

	dae::ServiceLocator::RegisterPhysicsService(std::make_unique<dae::physics::AABB_Physics>());
	dae::ServiceLocator::GetPhysicsService().AddSolver(std::make_unique<dae::physics::BulletSolver>());

	dae::GameLoop::GetInstance().ChangeState(std::make_unique<dae::IntroState>());
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}