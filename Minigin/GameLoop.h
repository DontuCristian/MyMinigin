#include "Singleton.h"
#include "GameState.h"
#include <memory>

//This class will contain the GameStates and manage the transitions between them
namespace dae
{
    class GameLoop: public Singleton<GameLoop>
    {
    public:
        GameLoop()= default;
        ~GameLoop();

		GameLoop(const GameLoop&) = delete;
		GameLoop(GameLoop&&) = delete;
		GameLoop& operator=(const GameLoop&) = delete;
		GameLoop& operator=(GameLoop&&) = delete;

        void Update();
        void ChangeState(std::unique_ptr<GameState> newState);
        GameState* GetCurrentState() const;

    private:
        std::unique_ptr<GameState> m_pCurrentState;

        bool m_ChangeState{ false };
    };
}