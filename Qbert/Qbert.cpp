
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
	const float cubeSize = 64.f; //Size in pixels
	const float offset = 16.f;
	int cubeIndex = 0;
	
	glm::vec2 startPos{ 320.f,110.f };
	
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
			cube->AddComponent<dae::physics::Collider>(16.f , 32.f, glm::vec2{-8.f ,-16.f });
			cube->GetComponent<dae::physics::Collider>()->SetTag("Block");
			cube->AddComponent<dae::Block>();
			cube->GetComponent<dae::Block>()->SetIsEdge(row == totalRows-1);
	
#ifdef _DEBUG
			std::cout << "Cube " << cubeIndex << " Position: ("
				<< x << ", " << y << ", " << ") "
				<< (isOutside ? "[OUTSIDE]\n" : "[INSIDE]\n");
#endif // _DEBUG
	
			scene.Add(cube);
	
			cubeIndex++;
		}
	}
	
	glm::vec2 deathStartPos = startPos - glm::vec2{offset,cubeSize};
	
	for (int row = 1; row < totalRows + 1; ++row) {
		for (int col = 0; col <= row; ++col) {
			bool isOutside = (col == 0) || (col == row);
	
			if (isOutside)
			{
				auto cube = std::make_shared<dae::GameObject>();
	
				float x = deathStartPos.x + (col - row * 0.5f) * cubeSize;
				float y = deathStartPos.y - (-row * (cubeSize - offset));


				if (col == 0)           // Left edge
					x -= cubeSize / 1.5f;
				else if (col == row)    // Right edge
					x += cubeSize / 1.5f;


				cube->GetTransform()->SetLocalPosition(x, y);
				cube->AddComponent<dae::physics::Collider>(42.f, 16.f, glm::vec2{-8.f ,20.f },true);
				cube->GetComponent<dae::physics::Collider>()->SetTag("KillZone");
	
#ifdef _DEBUG
				std::cout << "Cube " << cubeIndex << " Position: ("
					<< x << ", " << y << ", " << ") "
					<< (isOutside ? "[OUTSIDE]\n" : "[INSIDE]\n");
#endif // _DEBUG
	
				scene.Add(cube);
			}
	
			cubeIndex++;
		}
	}

	//auto gameObject = std::make_shared<dae::GameObject>();
	//gameObject->GetTransform()->SetLocalPosition(320.f, 250.f);
	//gameObject->AddComponent<dae::physics::Collider>();
	//gameObject->GetComponent<dae::physics::Collider>()->Width = 40;
	//gameObject->GetComponent<dae::physics::Collider>()->Height = 16;
	//scene.Add(gameObject);

	//gameObject = std::make_shared<dae::GameObject>();
	//gameObject->GetTransform()->SetLocalPosition(320.f, 230.f);
	//gameObject->AddComponent<dae::physics::Collider>();
	//gameObject->GetComponent<dae::physics::Collider>()->Width = 40;
	//gameObject->GetComponent<dae::physics::Collider>()->Height = 16;
	//gameObject->GetComponent<dae::physics::Collider>()->IsTrigger = true;
	//scene.Add(gameObject);

	auto elevator = std::make_shared<dae::GameObject>();
	elevator->GetTransform()->SetLocalPosition(startPos.x - 160, startPos.y + 150);
	elevator->AddComponent<dae::SpriteRenderer>();
	elevator->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("Level1_Elevator.png", 1, 4);
	elevator->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.15f);
	elevator->AddComponent<dae::physics::Collider>(32.f, 10.f, glm::vec2{-16.f,-5.f },true);
	elevator->GetComponent<dae::physics::Collider>()->SetTag("Elevator");
	elevator->AddComponent<dae::Platform>();

	scene.Add(elevator);

	auto to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::FPSRenderer>();
	to->GetComponent<dae::TextRenderer>()->SetFont("Lingua.otf", 25);
	to->GetTransform()->SetLocalPosition(20.f, 30.f);
	scene.Add(to);

	auto qBertHealth = std::make_shared<dae::GameObject>();
	qBertHealth->AddComponent<dae::HealthHUD>();
	qBertHealth->GetComponent<dae::HealthHUD>()->SetFont("Lingua.otf", 15);
	qBertHealth->GetComponent<dae::HealthHUD>()->SetText("Lives: 3");
	qBertHealth->GetTransform()->SetLocalPosition(30.f, 70.f);
	scene.Add(qBertHealth);

	auto qBertScore = std::make_shared<dae::GameObject>();
	qBertScore->AddComponent<dae::ScoreHUD>();
	qBertScore->GetComponent<dae::ScoreHUD>()->SetFont("Lingua.otf", 15);
	qBertScore->GetComponent<dae::ScoreHUD>()->SetText("0");
	qBertScore->GetTransform()->SetLocalPosition(30.f, 90.f);
	scene.Add(qBertScore);

	auto qBert = std::make_shared<dae::GameObject>();
	qBert->GetTransform()->SetLocalPosition(320, 60);
	qBert->AddComponent<dae::SpriteRenderer>();
	qBert->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("QBertMoving.png", 4, 2);
	qBert->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
	qBert->AddComponent<dae::physics::RigidBody>();
	qBert->AddComponent<dae::physics::Collider>(16.f, 16.f, glm::vec2{ -8.f,-6.f });
	qBert->GetComponent<dae::physics::Collider>()->SetTag("Player");
	qBert->AddComponent<dae::DeathHandlerComponent>();
	qBert->AddComponent<dae::Health>();
	qBert->GetComponent<dae::Health>()->AddObserver(qBertHealth->GetComponent<dae::HealthHUD>());
	qBert->GetComponent<dae::Health>()->AddObserver(qBert->GetComponent<dae::DeathHandlerComponent>());
	qBert->AddComponent<dae::PlayerScore>();
	qBert->GetComponent<dae::PlayerScore>()->AddObserver(qBertScore->GetComponent<dae::ScoreHUD>());
	scene.Add(qBert);

	auto redBall = std::make_shared<dae::GameObject>();
	redBall->GetTransform()->SetLocalPosition(280, 120);
	redBall->AddComponent<dae::SpriteRenderer>();
	redBall->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("Red_Ball.png", 1, 2);
	redBall->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
	redBall->AddComponent<dae::physics::RigidBody>();
	redBall->GetComponent<dae::physics::RigidBody>()->Gravity = glm::vec2{ 1,0 };
	//redBall->GetComponent<dae::physics::RigidBody>()->IsKinematic = true;
	redBall->AddComponent<dae::physics::Collider>(16.f, 16.f, glm::vec2{-8.f,-6.f });
	redBall->GetComponent<dae::physics::Collider>()->SetTag("WrongWay");
	redBall->AddComponent<dae::WrongWay>();
	scene.Add(redBall);

	auto coily = std::make_shared<dae::GameObject>();
	coily->GetTransform()->SetLocalPosition(320, 60);
	coily->AddComponent<dae::SpriteRenderer>();
	coily->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("Coily.png", 5, 2);
	coily->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
	coily->AddComponent<dae::physics::RigidBody>();
	coily->AddComponent<dae::physics::Collider>(16.f, 16.f, glm::vec2{ -8.f,8.f });
	coily->GetComponent<dae::physics::Collider>()->SetTag("Coily");
	coily->AddComponent<dae::Coily>(qBert->GetTransform());
	scene.Add(coily);

	input.AddAction("MoveUpLeft",  SDL_SCANCODE_A, dae::TriggerEvent::Down, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec2{ -1,-5.4 }, 250.f));
	input.AddAction("MoveDownRight", SDL_SCANCODE_D, dae::TriggerEvent::Down, std::make_unique<dae::MoveCommand>(*qBert.get(),glm::vec2{ 1,-2.5 }, 120.5f));
	input.AddAction("MoveUpRight", SDL_SCANCODE_W, dae::TriggerEvent::Down, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec2{ 1,-5.4 }, 250.f));
	input.AddAction("MoveDownLeft", SDL_SCANCODE_S, dae::TriggerEvent::Down, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec2{ -1,-2.5 }, 120.5f));

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

