#include "LevelState.h"

#include "GameObject.h"
#include "SceneManager.h"

#include "InputManager.h"
#include "Timer.h"
#include "EngineComponentsIncludes.h"
#include "MoveCommand.h"
#include "ChangeLevelCommand.h"
#include "PlayerScore.h"

#include "GameLoop.h"
#include "EndState.h"
#include "LevelBuilder.h"
#include "Health.h"

#include <random>
#include "EndState2Players.h"

dae::LevelState::LevelState(int levelIndex, const std::string& mode, std::vector<int> prevLevelScore) :
	m_Mode(mode),
	m_PrevLevelScores(prevLevelScore),
	m_LevelNumber(levelIndex)
{
	m_pScene = &dae::SceneManager::GetInstance().GetScene("Game");
}

void dae::LevelState::Enter()
{
	auto level = LevelBuilder::GetInstance().LoadLevel("../Data/Tron/Levels/Level" + std::to_string(m_LevelNumber) + m_Mode + ".json");
	LevelBuilder::GetInstance().BuildLevel(level, m_pScene->GetName());

	m_LevelNumber++;
	m_LevelNumber = m_LevelNumber % 3;

	std::vector<GameObject*> players;

	if (m_Mode != "Pvp")
	{
		players = m_pScene->FindAll("Player1");
	}
	if (m_Mode == "Pvp")
	{
		players.push_back(m_pScene->Find("Player1"));
		players.push_back(m_pScene->Find("Player2"));
	}

	for (int idx{}; idx < players.size(); ++idx)
	{
		players[idx]->GetComponent<PlayerScore>()->IncreaseScore(m_PrevLevelScores[idx]);
	}

	auto& input = InputManager::GetInstance();

	input.AddAction("GoToNextLevel", SDL_SCANCODE_F1, TriggerEvent::Down, std::make_unique<ChangeLevelCommand>(m_LevelNumber,m_Mode));

}

void dae::LevelState::Update()
{
	std::vector<int> scores{};


	std::vector<GameObject*> players;

	if (m_Mode != "Pvp")
	{
		players = m_pScene->FindAll("Player1");
	}
	if (m_Mode == "Pvp")
	{
		players.push_back(m_pScene->Find("Player1"));
		players.push_back(m_pScene->Find("Player2"));
	}

	for (auto& player : players)
	{
		scores.push_back(player->GetComponent<PlayerScore>()->GetScore());
	}
	
	if (LevelBuilder::GetInstance().IsLevelFinished())
	{
		if (m_LevelNumber == 3)
		{
			if (m_Mode == "Coop" || m_Mode == "Pvp")
			{
				GameLoop::GetInstance().ChangeState(std::make_unique<EndState2Players>(scores,m_Mode));
				return;
			}

			GameLoop::GetInstance().ChangeState(std::make_unique<EndState>(scores[0]));
			return;
		}

		m_LevelNumber++;
		m_LevelNumber = m_LevelNumber % 3;

		std::cout << "Level Complete!" << std::endl;
		GameLoop::GetInstance().ChangeState(std::make_unique<LevelState>(m_LevelNumber,m_Mode, scores));
		return;
	}


	bool allDead = true;
	for (auto& player : players)
	{
		if (!player->GetComponent<Health>()->IsDead())
		{
			allDead = false;
			break; // at least one player alive, no need to check further
		}
	}

	if(allDead)
	{

		if (m_Mode == "Coop" || m_Mode == "Pvp")
		{
			GameLoop::GetInstance().ChangeState(std::make_unique<EndState2Players>(scores, m_Mode));
			std::cout << "Level Lost!" << std::endl;
			return;
		}

		GameLoop::GetInstance().ChangeState(std::make_unique<EndState>(scores[0]));
		std::cout << "Level Lost!" << std::endl;
		return;
	}
}

void dae::LevelState::Exit()
{
	auto& input = InputManager::GetInstance();

	input.RemoveAction("MoveLeft0");
	input.RemoveAction("MoveRight0");
	input.RemoveAction("MoveDown0");
	input.RemoveAction("MoveUp0");
	input.RemoveAction("Shoot0");
	input.RemoveAction("DecreaseShootingAngle0");
	input.RemoveAction("IncreaseShootingAngle0");

	input.RemoveAction("MoveLeft1");
	input.RemoveAction("MoveRight1");
	input.RemoveAction("MoveDown1");
	input.RemoveAction("MoveUp1");
	input.RemoveAction("Shoot1");
	input.RemoveAction("DecreaseShootingAngle1");
	input.RemoveAction("IncreaseShootingAngle1");

	input.RemoveAction("GoToNextLevel");

	m_pScene->RemoveAll();
}

std::vector<int> dae::LevelState::GetPlayerScores() const
{
	std::vector<int> scores{};
	auto players = m_pScene->FindAll("Player1");

	if (m_Mode != "Pvp")
	{
		for (auto& player : players)
		{
			scores.push_back(player->GetComponent<PlayerScore>()->GetScore());
		}

		return scores;
	}
	if (m_Mode == "Pvp")
	{
		scores.push_back(m_pScene->Find("Player1")->GetComponent<PlayerScore>()->GetScore());
		scores.push_back(m_pScene->Find("Player2")->GetComponent<PlayerScore>()->GetScore());
	}

	return scores;
}
