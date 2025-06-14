
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
#include "GameComponentsIncludes.h"
#include "EngineComponentsIncludes.h"
#include "ServiceLocator.h"
#include "Logging_SoundSystem.h"
#include "SDL_SoundSystem.h"
#include "AABB_Physics.h"
#include "PhysicsService.h"
#include "PlatformSolver.h"
#include "GameLoop.h"
#include "LevelState.h"
#include "IntroState.h"
#include <iostream>

void load()
{
	//auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	//auto& input = dae::InputManager::GetInstance();

#ifdef _DEBUG
	dae::ServiceLocator::RegisterSoundService(std::make_unique<dae::Logging_SoundSystem>(std::make_unique<dae::SDL_SoundSystem>()));
#else
	dae::ServiceLocator::RegisterSoundService(std::make_unique<dae::SDL_SoundSystem>());
#endif

	dae::ServiceLocator::RegisterPhysicsService(std::make_unique<dae::physics::AABB_Physics>());
	dae::ServiceLocator::GetPhysicsService().AddSolver(std::make_unique<dae::physics::PlatformSolver>());

	dae::GameLoop::GetInstance().ChangeState(std::make_unique<dae::IntroState>());


	//auto snakeHealth = std::make_shared<dae::GameObject>();
	//snakeHealth->AddComponent<dae::HealthHUD>();
	//snakeHealth->GetComponent<dae::HealthHUD>()->SetFont("Lingua.otf", 15);
	//snakeHealth->GetComponent<dae::HealthHUD>()->SetText("HP: 100");
	//snakeHealth->GetTransform()->SetLocalPosition(30, 150);
	//scene.Add(snakeHealth);
	//
	//auto snakeScore = std::make_shared<dae::GameObject>();
	//snakeScore->AddComponent<dae::ScoreHUD>();
	//snakeScore->GetComponent<dae::ScoreHUD>()->SetFont("Lingua.otf", 15);
	//snakeScore->GetComponent<dae::ScoreHUD>()->SetText("Score: 0");
	//snakeScore->GetTransform()->SetLocalPosition(30, 170);
	//scene.Add(snakeScore);
	//
	//auto snake = std::make_shared<dae::GameObject>();
	//snake->GetTransform()->SetLocalPosition(130, 100);
	//snake->AddComponent<dae::SpriteRenderer>();
	//snake->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("Snake.png", 1, 8);
	//snake->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
	//snake->AddComponent<dae::Health>();
	//snake->GetComponent<dae::Health>()->AddObserver(snakeHealth->GetComponent<dae::HealthHUD>());
	//snake->AddComponent<dae::PlayerScore>();
	//snake->GetComponent<dae::PlayerScore>()->AddObserver(snakeScore->GetComponent<dae::ScoreHUD>());
	//scene.Add(snake);

	//input.AddAction("MoveLeft", XINPUT_GAMEPAD_DPAD_LEFT, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*snake.get(), glm::vec2{ -1,-5.4 }, 187.f),1);
	//input.AddAction("MoveRight", XINPUT_GAMEPAD_DPAD_RIGHT, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*snake.get(), glm::vec2{ 1,-2.5 }, 92.5f),1);
	//input.AddAction("MoveUp", XINPUT_GAMEPAD_DPAD_UP, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*snake.get(), glm::vec2{ 1,-5 }, 187.f),1);
	//input.AddAction("MoveDown", XINPUT_GAMEPAD_DPAD_DOWN, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*snake.get(), glm::vec2{ -1,-2.5 }, 92.5f),1);
	//input.AddAction("TakeDamage", XINPUT_GAMEPAD_X, dae::TriggerEvent::Down, std::make_unique<dae::TakeDamageCommand>(*snake.get(), 30), 1);


}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}

