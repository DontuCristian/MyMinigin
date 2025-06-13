#include "LevelState.h"

#include "GameObject.h"
#include "Scene.h"

#include "InputManager.h"
#include "Timer.h"
#include "GameComponentsIncludes.h"
#include "EngineComponentsIncludes.h"
#include "MoveCommand.h"

#include "GameLoop.h"

#include <random>

dae::LevelState::LevelState(int levelNumber, const std::string& mode):
	m_LevelNumber(levelNumber)
{
	dae::SceneManager::GetInstance().CreateScene("Level");
	m_Scene = dae::SceneManager::GetInstance().GetScene("Level");  
}

void dae::LevelState::Enter()
{

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
			cube->AddComponent<dae::physics::Collider>(16.f, 16.f, glm::vec2{ -8.f , 0.f });
			cube->GetComponent<dae::physics::Collider>()->SetTag("Block");
			cube->AddComponent<dae::Block>();
			cube->GetComponent<dae::Block>()->SetIsEdge(row == totalRows - 1);

#ifdef _DEBUG
			std::cout << "Cube " << cubeIndex << " Position: ("
				<< x << ", " << y << ", " << ") "
				<< (isOutside ? "[OUTSIDE]\n" : "[INSIDE]\n");
#endif // _DEBUG

			m_Scene->Add(cube);

			cubeIndex++;
		}
	}

	glm::vec2 deathStartPos = startPos - glm::vec2{ offset,cubeSize };

	for (int row = 1; row < totalRows + 2; ++row) {
		for (int col = 0; col <= row; ++col) {
			bool isOutside = (col == 0) || (col == row);

			if (isOutside)
			{
				auto cube = std::make_shared<dae::GameObject>();

				float x = deathStartPos.x + (col - row * 0.5f) * cubeSize;
				float y = deathStartPos.y - (-(row-1) * (cubeSize - offset));


				if (col == 0)           // Left edge
					x -= cubeSize / 4.f;
				else if (col == row)    // Right edge
					x += cubeSize / 4.f;


				cube->GetTransform()->SetLocalPosition(x, y);
				cube->AddComponent<dae::physics::Collider>(42.f, 16.f, glm::vec2{ -8.f ,20.f }, true);
				cube->GetComponent<dae::physics::Collider>()->SetTag("KillZone");

#ifdef _DEBUG
				std::cout << "Cube " << cubeIndex << " Position: ("
					<< x << ", " << y << ", " << ") "
					<< (isOutside ? "[OUTSIDE]\n" : "[INSIDE]\n");
#endif // _DEBUG

				m_Scene->Add(cube);
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
	elevator->AddComponent<dae::physics::Collider>(32.f, 10.f, glm::vec2{ -16.f,-5.f }, true);
	elevator->GetComponent<dae::physics::Collider>()->SetTag("Elevator");
	elevator->AddComponent<dae::Platform>();
	
	m_Scene->Add(elevator);

	auto to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::FPSRenderer>();
	to->GetComponent<dae::TextRenderer>()->SetFont("Lingua.otf", 25);
	to->GetTransform()->SetLocalPosition(20.f, 30.f);
	m_Scene->Add(to);

	auto qBertHealth = std::make_shared<dae::GameObject>();
	qBertHealth->AddComponent<dae::HealthHUD>();
	qBertHealth->GetComponent<dae::HealthHUD>()->SetFont("Lingua.otf", 15);
	qBertHealth->GetComponent<dae::HealthHUD>()->SetText("Lives: 3");
	qBertHealth->GetTransform()->SetLocalPosition(30.f, 70.f);
	m_Scene->Add(qBertHealth);

	auto qBertScore = std::make_shared<dae::GameObject>();
	qBertScore->AddComponent<dae::ScoreHUD>();
	qBertScore->GetComponent<dae::ScoreHUD>()->SetFont("Lingua.otf", 15);
	qBertScore->GetComponent<dae::ScoreHUD>()->SetText("0");
	qBertScore->GetTransform()->SetLocalPosition(30.f, 90.f);
	m_Scene->Add(qBertScore);

	auto qBert = std::make_shared<dae::GameObject>();
	qBert->GetTransform()->SetLocalPosition(320, 20);
	qBert->AddComponent<dae::SpriteRenderer>();
	qBert->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("QBertMoving.png", 4, 2);
	qBert->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
	qBert->AddComponent<dae::physics::RigidBody>();
	//qBert->GetComponent<dae::physics::RigidBody>()->IsKinematic = true;
	qBert->AddComponent<dae::physics::Collider>(16.f, 16.f, glm::vec2{ -8.f,12.f });
	qBert->GetComponent<dae::physics::Collider>()->SetTag("Player");
	qBert->AddComponent<dae::DeathHandlerComponent>();
	qBert->AddComponent<dae::Health>();
	qBert->GetComponent<dae::Health>()->AddObserver(qBertHealth->GetComponent<dae::HealthHUD>());
	qBert->GetComponent<dae::Health>()->AddObserver(qBert->GetComponent<dae::DeathHandlerComponent>());
	qBert->AddComponent<dae::PlayerScore>();
	qBert->GetComponent<dae::PlayerScore>()->AddObserver(qBertScore->GetComponent<dae::ScoreHUD>());
	m_Scene->Add(qBert);

	InputManager::GetInstance().AddAction("MoveUpLeft", SDL_SCANCODE_A, dae::TriggerEvent::Down, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec2{ -1,-5.4 }, 250.f));
	InputManager::GetInstance().AddAction("MoveDownRight", SDL_SCANCODE_D, dae::TriggerEvent::Down, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec2{ 1,-2.5 }, 120.5f));
	InputManager::GetInstance().AddAction("MoveUpRight", SDL_SCANCODE_W, dae::TriggerEvent::Down, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec2{ 1,-5.4 }, 250.f));
	InputManager::GetInstance().AddAction("MoveDownLeft", SDL_SCANCODE_S, dae::TriggerEvent::Down, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec2{ -1,-2.5 }, 120.5f));


}

void dae::LevelState::Update()
{
	SpawnEnemies();

	if (IsLevelComplete())
	{
		std::cout << "Level Complete!" << std::endl;
		GameLoop::GetInstance().ChangeState(std::make_unique<LevelState>(++m_LevelNumber));
		return;
	}
	if (IsLevelLost())
	{
		std::cout << "Level Lost!" << std::endl;
		return;
	}
}

void dae::LevelState::BlockReachedTargetColor()
{
	m_CurrentNrBlocks++;
}

void dae::LevelState::ResetLevel()
{
	auto objects = m_Scene->GetObjects();

	for (auto& enemy : objects)
	{
		auto it = std::find(m_Enemies.begin(), m_Enemies.end(), enemy.get());

		if(it != m_Enemies.end())
			enemy->FlagForDeletion();
	}

	m_Enemies.clear();
}

void dae::LevelState::SpawnEnemies()
{
	m_SpawnEnemiesTimer -= Timer::GetInstance().GetDeltaTime();

	if (m_SpawnEnemiesTimer <= 0)
	{
		if (m_LevelNumber == 1)
		{
			SpawnCoily();

			static std::default_random_engine engine{ std::random_device{}() };
			static std::uniform_int_distribution<int> dist(0, 1);

			if (dist(engine) == 0)
			{
				SpawnWrongWay();
			}
			else
			{
				SpawnUgg();
			}
		}
		else if (m_LevelNumber == 2)
		{
			static std::default_random_engine engine{ std::random_device{}() };
			static std::uniform_int_distribution<int> dist(0, 1);

			if (dist(engine) == 0)
			{
				SpawnWrongWay();
			}
			else
			{
				SpawnUgg();
			}

			SpawnCoily();

		}
		else if (m_LevelNumber == 3)
		{
		}


		m_SpawnEnemiesTimer = m_SpawnEnemiesInterval;
	}

}

void dae::LevelState::SpawnCoily()
{
	//If Coily isn't already spawned
	if (!m_Scene->Find("CoilySnake").get())
	{
		auto qBert = m_Scene->Find("Player");

		auto coily = std::make_shared<dae::GameObject>();
		coily->GetTransform()->SetLocalPosition(320, 60);
		coily->AddComponent<dae::SpriteRenderer>();
		coily->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("Coily.png", 5, 2);
		coily->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
		coily->AddComponent<dae::physics::RigidBody>();
		//coily->GetComponent<dae::physics::RigidBody>()->IsKinematic = true;
		coily->AddComponent<dae::physics::Collider>(16.f, 16.f, glm::vec2{ -8.f,24.f });
		coily->GetComponent<dae::physics::Collider>()->SetTag("CoilySnake");
		if(qBert)
			coily->AddComponent<dae::Coily>(qBert->GetTransform());
		coily->AddComponent<dae::EnemyDeathHandlerComponent>();
		m_Scene->Add(coily);

		m_Enemies.push_back(coily.get());
	}
}

void dae::LevelState::SpawnSlick()
{
}


void dae::LevelState::SpawnSam()
{
}

void dae::LevelState::SpawnUgg()
{
	auto ugg = std::make_shared<dae::GameObject>();
	ugg->GetTransform()->SetLocalPosition(30, 398);
	ugg->AddComponent<dae::SpriteRenderer>();
	ugg->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("Ugg.png", 2, 4);
	ugg->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
	ugg->AddComponent<dae::physics::RigidBody>();
	ugg->GetComponent<dae::physics::RigidBody>()->Gravity = glm::vec2{ 1,0 };
	//redBall->GetComponent<dae::physics::RigidBody>()->IsKinematic = true;
	ugg->AddComponent<dae::physics::Collider>(16.f, 16.f, glm::vec2{ -8.f,-8.f });
	ugg->GetComponent<dae::physics::Collider>()->SetTag("Ugg");
	ugg->AddComponent<dae::WrongWay>(glm::vec2{ -2.5, -2.5 }, glm::vec2{ -2.5, 2.5 });
	ugg->AddComponent<dae::EnemyDeathHandlerComponent>();
	m_Scene->Add(ugg);

	m_Enemies.push_back(ugg.get());

}

void dae::LevelState::SpawnWrongWay()
{
	auto wrongWay = std::make_shared<dae::GameObject>();
	wrongWay->GetTransform()->SetLocalPosition(520, 398);
	wrongWay->AddComponent<dae::SpriteRenderer>();
	wrongWay->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("WrongWay.png", 2, 4);
	wrongWay->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
	wrongWay->AddComponent<dae::physics::RigidBody>();
	wrongWay->GetComponent<dae::physics::RigidBody>()->Gravity = glm::vec2{ -1,0 };
	//redBall->GetComponent<dae::physics::RigidBody>()->IsKinematic = true;
	wrongWay->AddComponent<dae::physics::Collider>(16.f, 16.f, glm::vec2{ -8.f,-8.f });
	wrongWay->GetComponent<dae::physics::Collider>()->SetTag("WrongWay");
	wrongWay->AddComponent<dae::WrongWay>(glm::vec2{ 2.5, -2.5 }, glm::vec2{ 2.5, 2.5 });
	wrongWay->AddComponent<dae::EnemyDeathHandlerComponent>();
	m_Scene->Add(wrongWay);


	m_Enemies.push_back(wrongWay.get());
}

void dae::LevelState::Exit()
{
	m_Scene->RemoveAll();
}

bool dae::LevelState::IsLevelComplete() const
{
	return m_CurrentNrBlocks>=m_MaxNrBlocks;
}

bool dae::LevelState::IsLevelLost() const
{
	auto qBert = m_Scene->Find("Player");
	if(qBert)
		return qBert->GetComponent<dae::Health>()->IsDead();

	return false;
}
