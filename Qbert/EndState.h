#pragma once
#include "GameState.h"

namespace dae
{
    class EndState : public GameState {
    public:
        void Enter() override;
        void Update() override;
        void Render() override;
        void Exit() override;

    private:
        bool m_restart = false;
    };
}
