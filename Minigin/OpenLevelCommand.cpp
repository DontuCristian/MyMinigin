#include "OpenLevelCommand.h"
#include "GameLoop.h"
#include "LevelState.h"

void dae::OpenLevelCommand::Execute()
{
	std::vector<int> scores{};
	scores.push_back(0);

	if (m_LevelMode == "Coop" || m_LevelMode == "Pvp")
	{
		scores.push_back(0);
	}

	GameLoop::GetInstance().ChangeState(std::make_unique<LevelState>( m_LevelIndex,m_LevelMode, scores));
}