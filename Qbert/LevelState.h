#pragma once
#include "GameState.h"
#include <string>


namespace dae
{
    class LevelState : public GameState {
    public:
        LevelState(int levelNumber);

        void Enter() override;
        void Update() override;
        void Render() override;
        void Exit() override;

        bool IsLevelComplete() const;
        int GetLevelNumber() const;

    private:
        void LoadLevelFromJson(const std::string& filename);

        int m_levelNumber;
        bool m_levelComplete = false;
    };
}