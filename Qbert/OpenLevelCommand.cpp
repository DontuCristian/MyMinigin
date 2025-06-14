#include "OpenLevelCommand.h"
#include "GameLoop.h"
#include "LevelState.h"

void dae::OpenLevelCommand::Execute()
{
	GameLoop::GetInstance().ChangeState(std::make_unique<LevelState>(1, m_LevelMode));
}