#pragma once
#include "GameState.h"
#include "Scene.h"

namespace dae
{
    class GameObject;

    class EndState : public GameState {
    public:
        EndState(int score);

        void Enter() override;
        void Update() override;
        void Render() const override;
        void Exit() override;

    private:
        int m_Score{};

        Scene* m_pScene{};
    };
}
