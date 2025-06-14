#include "LevelState.h"

#include "GameObject.h"
#include "Scene.h"

#include "InputManager.h"
#include "Timer.h"
#include "GameComponentsIncludes.h"
#include "EngineComponentsIncludes.h"
#include "MoveCommand.h"
#include "ChangeLevelCommand.h"

#include "GameLoop.h"
#include "EndState.h"
#include "LevelBuilder.h"

#include <random>

dae::LevelState::LevelState(int levelNumber, const std::string& mode, int prevLevelScore):
	m_LevelNumber(levelNumber),
	m_Mode(mode),
	m_PrevLevelScore(prevLevelScore)
{
	m_pScene = dae::SceneManager::GetInstance().GetScene("Level");
	if (m_pScene == nullptr)
	{
		dae::SceneManager::GetInstance().CreateScene("Level");
		m_pScene = dae::SceneManager::GetInstance().GetScene("Level");
	}

	if (m_Mode == "Pvp")
	{
		m_Pvp = true;
	}
}

void dae::LevelState::Enter()
{

	LevelBuilder::GetInstance().BuildLevel("../Data/Levels/Level"+std::to_string(m_LevelNumber)+m_Mode+".json", m_pScene);

	m_pScene->Find("Player")->GetComponent<PlayerScore>()->IncreaseScore(m_PrevLevelScore);

	auto& input = InputManager::GetInstance();

	input.AddAction("ChangeToLevel1", SDL_SCANCODE_F1, TriggerEvent::Down, std::make_unique<ChangeLevelCommand>(1, m_Mode));
	input.AddAction("ChangeToLevel2", SDL_SCANCODE_F2, TriggerEvent::Down, std::make_unique<ChangeLevelCommand>(2, m_Mode));
	input.AddAction("ChangeToLevel3", SDL_SCANCODE_F3, TriggerEvent::Down, std::make_unique<ChangeLevelCommand>(3, m_Mode));

}

void dae::LevelState::Update()
{
	SpawnEnemies();

	if (IsLevelComplete())
	{
		const int score = m_pScene->Find("Player")->GetComponent<dae::PlayerScore>()->GetScore();

		if (m_LevelNumber == 3)
		{
			GameLoop::GetInstance().ChangeState(std::make_unique<EndState>(score));
			return;
		}
		std::cout << "Level Complete!" << std::endl;
		GameLoop::GetInstance().ChangeState(std::make_unique<LevelState>(++m_LevelNumber, m_Mode, score));
		return;
	}
	if (IsLevelLost())
	{
		const int score = m_pScene->Find("Player")->GetComponent<dae::PlayerScore>()->GetScore();
		GameLoop::GetInstance().ChangeState(std::make_unique<EndState>(score));
		std::cout << "Level Lost!" << std::endl;
		return;
	}
}

void dae::LevelState::BlockReachedTargetColor()
{
	m_CurrentNrBlocks++;
}

void dae::LevelState::BlockTargetColorReverted()
{
	m_CurrentNrBlocks--;
}

void dae::LevelState::ResetLevel()
{
	auto objects = m_pScene->GetObjects();

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
		m_SpawnEnemiesTimer = m_SpawnEnemiesInterval;

		if (m_LevelNumber == 1)
		{
			SpawnCoily();

			static std::default_random_engine engine{ std::random_device{}() };
			static std::uniform_int_distribution<int> dist(0, 1);

			if (dist(engine) == 0)
			{
				SpawnWrongWay();
				return;
			}
			else
			{
				SpawnUgg();
				return;
			}
		}
		else if (m_LevelNumber == 2)
		{
			static std::default_random_engine engine{ std::random_device{}() };
			static std::uniform_int_distribution<int> dist(0, 3);

			SpawnCoily();

			if (dist(engine) == 0)
			{
				SpawnWrongWay();
				return;
			}
			else if(dist(engine) == 1)
			{
				SpawnUgg();
				return;
			}
			else if (dist(engine) == 2)
			{
				SpawnSlick();
				return;
			}
			else if (dist(engine) == 3)
			{
				SpawnSam();
				return;
			}

		}
		else if (m_LevelNumber == 3)
		{
			static std::default_random_engine engine{ std::random_device{}() };
			static std::uniform_int_distribution<int> dist(0, 3);

			SpawnCoily();

			if (dist(engine) == 0)
			{
				SpawnWrongWay();
				return;
			}
			else if (dist(engine) == 1)
			{
				SpawnUgg();
				return;
			}
			else if (dist(engine) == 2)
			{
				SpawnSlick();
				return;
			}
			else if (dist(engine) == 3)
			{
				SpawnSam();
				return;
			}
		}
	}

}

void dae::LevelState::SpawnCoily()
{
	//If Coily isn't already spawned
	if (!m_pScene->Find("Coily").get())
	{
		auto qBert = m_pScene->Find("Player");

		auto coily = std::make_shared<dae::GameObject>();
		coily->GetTransform()->SetLocalPosition(320, 60);
		coily->AddComponent<dae::SpriteRenderer>();
		coily->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("Coily.png", 5, 2);
		coily->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
		coily->AddComponent<dae::physics::RigidBody>();
		//coily->GetComponent<dae::physics::RigidBody>()->IsKinematic = true;
		coily->AddComponent<dae::physics::Collider>(16.f, 16.f, glm::vec2{ -8.f,24.f });
		coily->GetComponent<dae::physics::Collider>()->SetTag("Coily");
		if (qBert)
		{
			coily->AddComponent<dae::Coily>(qBert->GetTransform(), m_Pvp);
			coily->AddComponent<dae::EnemyDeathHandlerComponent>();
			coily->GetComponent<dae::EnemyDeathHandlerComponent>()->AddObserver(qBert->GetComponent<PlayerScore>());
		}
		m_pScene->Add(coily);

		m_Enemies.push_back(coily.get());
	}
}

void dae::LevelState::SpawnSlick()
{

	auto slick = std::make_shared<dae::GameObject>();
	slick->GetTransform()->SetLocalPosition(288, 60);
	slick->AddComponent<dae::SpriteRenderer>();
	slick->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("Slick.png", 2, 4);
	slick->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
	slick->AddComponent<dae::physics::RigidBody>();
	slick->AddComponent<dae::physics::Collider>(16.f, 16.f, glm::vec2{ -8.f,8.f });
	slick->GetComponent<dae::physics::Collider>()->SetTag("Slick");
	slick->AddComponent<dae::SamOrSlick>();
	slick->AddComponent<dae::EnemyDeathHandlerComponent>();
	m_pScene->Add(slick);

	m_Enemies.push_back(slick.get());

}


void dae::LevelState::SpawnSam()
{
	auto sam = std::make_shared<dae::GameObject>();
	sam->GetTransform()->SetLocalPosition(352, 60);
	sam->AddComponent<dae::SpriteRenderer>();
	sam->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("Sam.png", 2, 4);
	sam->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
	sam->AddComponent<dae::physics::RigidBody>();
	sam->AddComponent<dae::physics::Collider>(16.f, 16.f, glm::vec2{ -8.f,8.f });
	sam->GetComponent<dae::physics::Collider>()->SetTag("Sam");
	sam->AddComponent<dae::SamOrSlick>();
	sam->AddComponent<dae::EnemyDeathHandlerComponent>();
	m_pScene->Add(sam);

	m_Enemies.push_back(sam.get());
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
	ugg->AddComponent<dae::physics::Collider>(16.f, 16.f, glm::vec2{ -8.f,-8.f });
	ugg->GetComponent<dae::physics::Collider>()->SetTag("Ugg");
	ugg->AddComponent<dae::WrongWay>(glm::vec2{ -2.5, -2.5 }, glm::vec2{ -2.5, 2.5 });
	ugg->AddComponent<dae::EnemyDeathHandlerComponent>();
	m_pScene->Add(ugg);

	m_Enemies.push_back(ugg.get());

}

void dae::LevelState::SpawnWrongWay()
{
	auto wrongWay = std::make_shared<dae::GameObject>();
	wrongWay->GetTransform()->SetLocalPosition(560, 398);
	wrongWay->AddComponent<dae::SpriteRenderer>();
	wrongWay->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("WrongWay.png", 2, 4);
	wrongWay->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
	wrongWay->AddComponent<dae::physics::RigidBody>();
	wrongWay->GetComponent<dae::physics::RigidBody>()->Gravity = glm::vec2{ -1,0 };
	wrongWay->AddComponent<dae::physics::Collider>(16.f, 16.f, glm::vec2{ -8.f,-8.f });
	wrongWay->GetComponent<dae::physics::Collider>()->SetTag("WrongWay");
	wrongWay->AddComponent<dae::WrongWay>(glm::vec2{ 2.5, -2.5 }, glm::vec2{ 2.5, 2.5 });
	wrongWay->AddComponent<dae::EnemyDeathHandlerComponent>();
	m_pScene->Add(wrongWay);


	m_Enemies.push_back(wrongWay.get());
}

void dae::LevelState::Exit()
{
	auto& input = InputManager::GetInstance();

	input.RemoveAction("MoveUpLeft");
	input.RemoveAction("MoveUpRight");
	input.RemoveAction("MoveDownLeft");
	input.RemoveAction("MoveDownRight");
	input.RemoveAction("ChangeToLevel1");
	input.RemoveAction("ChangeToLevel2");
	input.RemoveAction("ChangeToLevel3");

	m_pScene->RemoveAll();
}

bool dae::LevelState::IsLevelComplete() const
{
	//return false;
	return m_CurrentNrBlocks>=m_MaxNrBlocks;
}

bool dae::LevelState::IsLevelLost() const
{
	auto qBert = m_pScene->Find("Player");
	if(qBert)
		return qBert->GetComponent<dae::Health>()->IsDead();

	return false;
}
