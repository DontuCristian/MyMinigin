#include "GameLoop.h"
#include "GameState.h"

dae::GameLoop::~GameLoop() 
{
    if (m_pCurrentState)
        m_pCurrentState->Exit();
}

void dae::GameLoop::Update()
{
    if (m_pCurrentState && m_ChangeState)
    {
        m_pCurrentState->Enter();
        m_ChangeState = false;
    }

    m_pCurrentState->Update();
}

void dae::GameLoop::ChangeState(std::unique_ptr<GameState> newState) 
{
    if (m_pCurrentState)
        m_pCurrentState->Exit();

    m_pCurrentState = std::move(newState);

    m_ChangeState = true;
}

dae::GameState* dae::GameLoop::GetCurrentState() const {
    return m_pCurrentState.get();
}