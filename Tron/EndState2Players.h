#pragma once
#include <GameState.h>
#include <vector>
#include <string>

namespace dae
{
	class GameObject;
	class Scene;

    class EndState2Players : public GameState
    {
        public:
            EndState2Players(const std::vector<int>& playerScores, const std::string& mode);

            void Enter() override;
            void Update() override;
            void Render() const override;
            void Exit() override;

        private:
            std::vector<int> m_Scores;
			std::string m_Mode;

            Scene* m_pScene{};
    
    };
}

