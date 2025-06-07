#include "Singleton.h"
#include <memory>

//This class will contain the GameStates and manage the transitions between them
namespace dae
{
    class GameState;

    class GameLoop {
    public:
        GameLoop()= default;
        ~GameLoop();

		GameLoop(const GameLoop&) = delete;
		GameLoop(GameLoop&&) = delete;
		GameLoop& operator=(const GameLoop&) = delete;
		GameLoop& operator=(GameLoop&&) = delete;

        void ChangeState(std::unique_ptr<GameState> newState);
        GameState* GetCurrentState() const;

    private:
        std::unique_ptr<GameState> m_pCurrentState;
    };
}