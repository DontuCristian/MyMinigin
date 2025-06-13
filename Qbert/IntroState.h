#pragma once
#include "GameState.h"
#include "Scene.h"

namespace dae
{
    class GameObject;

    class IntroState : public GameState {
    public:
        void Enter() override;
        void Update() override { return; };
        void Render() const override {};
        void Exit() override;

    private:
        bool m_startGame = false;

        Scene* m_Scene{};
    };
}