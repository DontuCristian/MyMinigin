#include "GameLoop.h"
#include "GameState.h"

dae::GameLoop::~GameLoop() {
    if (m_pCurrentState)
        m_pCurrentState->Exit();
}

void dae::GameLoop::ChangeState(std::unique_ptr<GameState> newState) {
    if (m_pCurrentState)
        m_pCurrentState->Exit();

    m_pCurrentState = std::move(newState);

    if (m_pCurrentState)
        m_pCurrentState->Enter();
}

dae::GameState* dae::GameLoop::GetCurrentState() const {
    return m_pCurrentState.get();
}