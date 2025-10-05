#pragma once
#include "GameState.h"
#include <string>
#include <vector>
#include <memory>


namespace dae
{
    class GameObject;
    class Scene;

    class LevelState : public GameState 
    {

    public:
        LevelState(int levelIndex, const std::string& mode, std::vector<int> prevLevelScore);

        void Enter() override;
		void Update() override;
        void Render() const override {};
        void Exit() override;

        std::vector<int> GetPlayerScores() const;

    private:

        int m_LevelNumber{0};
		std::vector<int> m_PrevLevelScores{};

		std::string m_Mode;

        Scene* m_pScene{};
    };
}