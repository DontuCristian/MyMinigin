#pragma once
#include "GameState.h"

namespace dae
{
    class GameObject;

    class EndState : public GameState {
    public:
        void Enter() override;
        void Update() override { return; };
        void Render() const override {};
        void Exit() override;

    private:
        bool m_Restart = false;
    };
}
