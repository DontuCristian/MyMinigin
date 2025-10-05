#pragma once
#include "GameState.h"
#include "Scene.h"

namespace dae
{
    class GameObject;

    class IntroState : public GameState {
    public:
        void Enter() override;
        void Update() override;
        void Render() const override {};
        void Exit() override;

    private:
        Scene* m_pScene{};
    };
}