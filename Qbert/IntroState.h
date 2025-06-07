#pragma once
#include "GameState.h"

namespace dae
{

    class IntroState : public GameState {
    public:
        void Enter() override;
        void Update() override;
        void Render() override;
        void Exit() override;

    private:
        bool m_startGame = false;
    };
}