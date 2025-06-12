#pragma once
#include "GameState.h"

namespace dae
{
    class GameObject;

    class EndState : public GameState {
    public:
        void Enter(GameObject& obj) override;
        GameState* Update() override { return this; };
        void Render() const override {};
        void Exit() override;

    private:
        bool m_Restart = false;
    };
}
