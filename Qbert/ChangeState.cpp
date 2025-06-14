#include "ChangeState.h"
#include "GameLoop.h"

dae::ChangeState::ChangeState(std::unique_ptr<GameState> state):
	m_State(std::move(state))
{
}

void dae::ChangeState::Execute()
{
	GameLoop::GetInstance().ChangeState(std::move(m_State));
}