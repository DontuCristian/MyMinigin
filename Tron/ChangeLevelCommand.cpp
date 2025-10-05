#include "ChangeLevelCommand.h"
#include "GameLoop.h"
#include "LevelState.h"
#include "SceneManager.h"

dae::ChangeLevelCommand::ChangeLevelCommand(int levelIndex,const std::string& mode):
	m_Mode{mode},
	m_Index{ levelIndex }
{

}

void dae::ChangeLevelCommand::Execute()
{
	auto* state = GameLoop::GetInstance().GetCurrentState();

	auto* levelState = dynamic_cast<LevelState*>(state);

	if (levelState)
	{
		auto scores = levelState->GetPlayerScores();

		GameLoop::GetInstance().ChangeState(std::make_unique<LevelState>(m_Index, m_Mode, scores));
	}
}
