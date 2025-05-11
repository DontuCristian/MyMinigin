
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
#include "AABB_Physics.h"
#include "PhysicsService.h"
#include "PlatformSolver.h"
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

	dae::ServiceLocator::RegisterPhysicsService(std::make_unique<dae::physics::AABB_Physics>());
	dae::ServiceLocator::GetPhysicsService().AddSolver(std::make_unique<dae::physics::PlatformSolver>());

	const int totalRows = 7;
	const float cubeSize = 32.f; //Size in pixels
	const float offset = 8.f;
	int cubeIndex = 0;

	glm::vec2 startPos{ 320.f,156.f };

	for (int row = 0; row < totalRows; ++row) {
		for (int col = 0; col <= row; ++col) {
			bool isOutside = (col == 0) || (col == row);

			auto cube = std::make_shared<dae::GameObject>();

			cube->AddComponent<dae::SpriteRenderer>();
			cube->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("Level1_Blocks.png", 6, 2);

			if (isOutside)
			{
				cube->GetComponent<dae::SpriteRenderer>()->SetFrameIdx(0);
			}
			else
			{
				cube->GetComponent<dae::SpriteRenderer>()->SetFrameIdx(1);
			}

			float x = startPos.x + (col - row * 0.5f) * cubeSize;
			float y = startPos.y - (-row * (cubeSize - offset));

			cube->GetTransform()->SetLocalPosition(x, y);
			cube->AddComponent<dae::physics::Collider>();
			cube->GetComponent<dae::physics::Collider>()->Width = 8;
			cube->GetComponent<dae::physics::Collider>()->Height = 16;
			cube->GetComponent<dae::physics::Collider>()->Offset = { -4, 0 };
			cube->AddComponent<dae::Block>();

			std::cout << "Cube " << cubeIndex << " Position: ("
				<< x << ", " << y << ", " << ") "
				<< (isOutside ? "[OUTSIDE]\n" : "[INSIDE]\n");

			scene.Add(cube);

			cubeIndex++;
		}
	}

	auto to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::FPSRenderer>();
	to->GetComponent<dae::TextRenderer>()->SetFont("Lingua.otf", 25);
	to->GetTransform()->SetLocalPosition(20, 30);
	scene.Add(to);

	//auto qBertHealth = std::make_shared<dae::GameObject>();
	//qBertHealth->AddComponent<dae::HealthHUD>();
	//qBertHealth->GetComponent<dae::HealthHUD>()->SetFont("Lingua.otf", 15);
	//qBertHealth->GetComponent<dae::HealthHUD>()->SetText("HP: 100");
	//qBertHealth->GetTransform()->SetLocalPosition(30, 70);
	//scene.Add(qBertHealth);

	auto qBertScore = std::make_shared<dae::GameObject>();
	qBertScore->AddComponent<dae::ScoreHUD>();
	qBertScore->GetComponent<dae::ScoreHUD>()->SetFont("Lingua.otf", 15);
	qBertScore->GetComponent<dae::ScoreHUD>()->SetText("0");
	qBertScore->GetTransform()->SetLocalPosition(30, 90);
	scene.Add(qBertScore);

	auto qBert = std::make_shared<dae::GameObject>();
	qBert->GetTransform()->SetLocalPosition(320, 100);
	qBert->AddComponent<dae::SpriteRenderer>();
	qBert->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("QBertMoving.png", 4, 2);
	qBert->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
	//qBert->AddComponent<dae::Health>();
	//qBert->GetComponent<dae::Health>()->AddObserver(qBertHealth->GetComponent<dae::HealthHUD>());
	qBert->AddComponent<dae::physics::RigidBody>();
	qBert->AddComponent<dae::physics::Collider>();
	qBert->GetComponent<dae::physics::Collider>()->Width = 8;
	qBert->GetComponent<dae::physics::Collider>()->Height = 16;
	qBert->GetComponent<dae::physics::Collider>()->Offset = { -4, 0 };
	qBert->AddComponent<dae::PlayerScore>();
	qBert->GetComponent<dae::PlayerScore>()->AddObserver(qBertScore->GetComponent<dae::ScoreHUD>());
	scene.Add(qBert);

	input.GetInstance().AddAction("MoveLeft", SDL_SCANCODE_A, dae::TriggerEvent::Down, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec2{ -1,-5.4 }, 187.f));
	input.GetInstance().AddAction("MoveRight", SDL_SCANCODE_D, dae::TriggerEvent::Down, std::make_unique<dae::MoveCommand>(*qBert.get(),glm::vec2{ 1,-2.5 }, 92.5f));
	input.GetInstance().AddAction("MoveUp", SDL_SCANCODE_W, dae::TriggerEvent::Down, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec2{ 1,-5.4 }, 187.f));
	input.GetInstance().AddAction("MoveDown", SDL_SCANCODE_S, dae::TriggerEvent::Down, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec2{ -1,-2.5 }, 92.5f));
	input.GetInstance().AddAction("TakeDamage", SDL_SCANCODE_C, dae::TriggerEvent::Down, std::make_unique<dae::TakeDamageCommand>(*qBert.get(), 20),0);

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

	//input.GetInstance().AddAction("MoveLeft", XINPUT_GAMEPAD_DPAD_LEFT, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*snake.get(), glm::vec2{ -1,-5.4 }, 187.f),1);
	//input.GetInstance().AddAction("MoveRight", XINPUT_GAMEPAD_DPAD_RIGHT, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*snake.get(), glm::vec2{ 1,-2.5 }, 92.5f),1);
	//input.GetInstance().AddAction("MoveUp", XINPUT_GAMEPAD_DPAD_UP, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*snake.get(), glm::vec2{ 1,-5 }, 187.f),1);
	//input.GetInstance().AddAction("MoveDown", XINPUT_GAMEPAD_DPAD_DOWN, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*snake.get(), glm::vec2{ -1,-2.5 }, 92.5f),1);
	//input.GetInstance().AddAction("TakeDamage", XINPUT_GAMEPAD_X, dae::TriggerEvent::Down, std::make_unique<dae::TakeDamageCommand>(*snake.get(), 30), 1);


}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}

