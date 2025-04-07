
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
#include "GameCommands.h"
#include "GameComponentsIncludes.h"
#include "EngineComponentsIncludes.h"
#include "ServiceLocator.h"
#include "Logging_SoundSystem.h"
#include "SDL_SoundSystem.h"
#include <iostream>

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& input = dae::InputManager::GetInstance();

#ifdef _DEBUG
	dae::ServiceLocator::RegisterSoundService(std::make_unique<dae::Logging_SoundSystem>(std::make_unique<dae::SDL_SoundSystem>()));
#else
	dae::ServiceLocator::RegisterSoundService(std::make_unique<dae::SDL_SoundSystem>());
#endif


	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureRenderer>();
	go->GetComponent<dae::TextureRenderer>()->SetTexture("background.tga");
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureRenderer>();
	go->GetComponent<dae::TextureRenderer>()->SetTexture("logo.tga");
	go->GetTransform()->SetLocalPosition(216, 180, 0);
	scene.Add(go);

	auto to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::TextRenderer>();
	to->GetComponent<dae::TextRenderer>()->SetFont("Lingua.otf", 36);
	to->GetComponent<dae::TextRenderer>()->SetText("Programming 4 Assignment");
	to->GetTransform()->SetLocalPosition(120, 20, 0);
	scene.Add(to);

	to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::FPSRenderer>();
	to->GetComponent<dae::TextRenderer>()->SetFont("Lingua.otf", 25);
	to->GetTransform()->SetLocalPosition(20, 30, 0);
	scene.Add(to);

	auto qBertHealth = std::make_shared<dae::GameObject>();
	qBertHealth->AddComponent<dae::HealthHUD>();
	qBertHealth->GetComponent<dae::HealthHUD>()->SetFont("Lingua.otf", 15);
	qBertHealth->GetComponent<dae::HealthHUD>()->SetText("HP: 100");
	qBertHealth->GetTransform()->SetLocalPosition(30, 70, 0);
	scene.Add(qBertHealth);

	auto qBertScore = std::make_shared<dae::GameObject>();
	qBertScore->AddComponent<dae::ScoreHUD>();
	qBertScore->GetComponent<dae::ScoreHUD>()->SetFont("Lingua.otf", 15);
	qBertScore->GetComponent<dae::ScoreHUD>()->SetText("Score: 0");
	qBertScore->GetTransform()->SetLocalPosition(30, 90, 0);
	scene.Add(qBertScore);

	auto qBert = std::make_shared<dae::GameObject>();
	qBert->GetTransform()->SetLocalPosition(200, 100, 0);
	qBert->AddComponent<dae::SpriteRenderer>();
	qBert->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("QBert1.png", 1, 8);
	qBert->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
	qBert->AddComponent<dae::Health>();
	qBert->GetComponent<dae::Health>()->AddObserver(qBertHealth->GetComponent<dae::HealthHUD>());
	qBert->AddComponent<dae::Score>();
	qBert->GetComponent<dae::Score>()->AddObserver(qBertScore->GetComponent<dae::ScoreHUD>());
	scene.Add(qBert);

	input.GetInstance().AddAction("MoveLeft", SDL_SCANCODE_A, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec3{ -1,0,0 }, 200.f));
	input.GetInstance().AddAction("MoveRight", SDL_SCANCODE_D, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec3{ 1,0,0 }, 200.f));
	input.GetInstance().AddAction("MoveUp", SDL_SCANCODE_W, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec3{ 0,-1,0 }, 200.f));
	input.GetInstance().AddAction("MoveDown", SDL_SCANCODE_S, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec3{ 0,1,0 }, 200.f));
	input.GetInstance().AddAction("TakeDamage", SDL_SCANCODE_C, dae::TriggerEvent::Down, std::make_unique<dae::TakeDamageCommand>(*qBert.get(), 20));
	input.GetInstance().AddAction("IncreaseScore", SDL_SCANCODE_Z, dae::TriggerEvent::Down, std::make_unique<dae::IncreaseScoreCommand>(*qBert.get(), 100));
	input.GetInstance().AddAction("IncreaseScore", SDL_SCANCODE_X, dae::TriggerEvent::Down, std::make_unique<dae::IncreaseScoreCommand>(*qBert.get(), 10));

	auto snakeHealth = std::make_shared<dae::GameObject>();
	snakeHealth->AddComponent<dae::HealthHUD>();
	snakeHealth->GetComponent<dae::HealthHUD>()->SetFont("Lingua.otf", 15);
	snakeHealth->GetComponent<dae::HealthHUD>()->SetText("HP: 100");
	snakeHealth->GetTransform()->SetLocalPosition(30, 150, 0);
	scene.Add(snakeHealth);

	auto snakeScore = std::make_shared<dae::GameObject>();
	snakeScore->AddComponent<dae::ScoreHUD>();
	snakeScore->GetComponent<dae::ScoreHUD>()->SetFont("Lingua.otf", 15);
	snakeScore->GetComponent<dae::ScoreHUD>()->SetText("Score: 0");
	snakeScore->GetTransform()->SetLocalPosition(30, 170, 0);
	scene.Add(snakeScore);

	auto snake = std::make_shared<dae::GameObject>();
	snake->GetTransform()->SetLocalPosition(130, 100, 0);
	snake->AddComponent<dae::SpriteRenderer>();
	snake->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("Snake.png", 1, 8);
	snake->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
	snake->AddComponent<dae::Health>();
	snake->GetComponent<dae::Health>()->AddObserver(snakeHealth->GetComponent<dae::HealthHUD>());
	snake->AddComponent<dae::Score>();
	snake->GetComponent<dae::Score>()->AddObserver(snakeScore->GetComponent<dae::ScoreHUD>());
	scene.Add(snake);

	input.GetInstance().AddAction("MoveLeft", XINPUT_GAMEPAD_DPAD_LEFT, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*snake.get(), glm::vec3{ -1,0,0 }, 200.f), 0);
	input.GetInstance().AddAction("MoveRight", XINPUT_GAMEPAD_DPAD_RIGHT, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*snake.get(), glm::vec3{ 1,0,0 }, 200.f), 0);
	input.GetInstance().AddAction("MoveUp", XINPUT_GAMEPAD_DPAD_UP, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*snake.get(), glm::vec3{ 0,-1,0 }, 200.f), 0);
	input.GetInstance().AddAction("MoveDown", XINPUT_GAMEPAD_DPAD_DOWN, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*snake.get(), glm::vec3{ 0,1,0 }, 200.f), 0);
	input.GetInstance().AddAction("TakeDamage", XINPUT_GAMEPAD_X, dae::TriggerEvent::Down, std::make_unique<dae::TakeDamageCommand>(*snake.get(), 30), 0);
	input.GetInstance().AddAction("IncreaseScore", XINPUT_GAMEPAD_A, dae::TriggerEvent::Down, std::make_unique<dae::IncreaseScoreCommand>(*snake.get(), 100), 0);
	input.GetInstance().AddAction("IncreaseScore", XINPUT_GAMEPAD_B, dae::TriggerEvent::Down, std::make_unique<dae::IncreaseScoreCommand>(*snake.get(), 10), 0);


}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}

