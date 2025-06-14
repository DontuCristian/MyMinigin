#include "ChangeLevelCommand.h"
#include "GameLoop.h"
#include "LevelState.h"

dae::ChangeLevelCommand::ChangeLevelCommand(int index, const std::string& mode):
	m_Index{index},
	m_Mode{mode}
{

}

void dae::ChangeLevelCommand::Execute()
{
	GameLoop::GetInstance().ChangeState(std::make_unique<LevelState>(m_Index,m_Mode));
}
