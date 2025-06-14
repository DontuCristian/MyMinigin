#pragma once
#include <GameState.h>
#include "Scene.h"

namespace dae
{
    class Scoreboard : public GameState
    {
    public:
        void Enter() override;
        void Update() override;
        void Render() const override {};
        void Exit() override;

    private:
        Scene* m_pScene{};
    };
}

