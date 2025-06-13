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
        LevelState(int levelNumber, const std::string& mode);

        void Enter() override;
		void Update() override;
        void Render() const override {};
        void Exit() override;

        int IncreaseLevelNr() { m_LevelNumber++; }
        void BlockReachedTargetColor();

        void ResetLevel();

    private:

        bool IsLevelComplete() const;
        bool IsLevelLost() const;

        void SpawnEnemies();
        void SpawnCoily();
		void SpawnSlick();
        void SpawnSam();
		void SpawnUgg();
		void SpawnWrongWay();
        //void LoadLevelFromJson(const std::string& filename);

        float m_SpawnEnemiesTimer{ 3.f };
        float m_SpawnEnemiesInterval{ 6.f };

        int m_MaxNrBlocks{ 4 };
		int m_CurrentNrBlocks{ 0 };

        int m_LevelNumber{1};
        bool m_IsLevelComplete = false;

        Scene* m_Scene{};
        std::vector<GameObject*> m_Enemies{};
    };
}