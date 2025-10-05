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
        void AddText(const std::string& text, float x, float y, int fontSize);

        Scene* m_pScene{};
    };
}

